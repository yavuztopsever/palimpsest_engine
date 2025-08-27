/**************************************************************************/
/*  dialogue_manager.cpp                                                  */
/**************************************************************************/
/*                         This file is part of:                          */
/*                        PALIMPSEST ENGINE                               */
/**************************************************************************/

#include "dialogue_manager.h"
#include "core/object/class_db.h"
#include "core/io/file_access.h"
#include "core/string/print_string.h"

DialogueManager::DialogueManager() {
	current_dialogue = nullptr;
	global_consciousness_effect = 0.0f;
}

DialogueManager::~DialogueManager() {
	clear_all_dialogues();
}

void DialogueManager::_bind_methods() {
	// Dialogue management
	ClassDB::bind_method(D_METHOD("register_dialogue", "id", "dialogue"), &DialogueManager::register_dialogue);
	ClassDB::bind_method(D_METHOD("unregister_dialogue", "id"), &DialogueManager::unregister_dialogue);
	ClassDB::bind_method(D_METHOD("has_dialogue", "id"), &DialogueManager::has_dialogue);
	
	// Flow control
	ClassDB::bind_method(D_METHOD("start_dialogue", "dialogue_id"), &DialogueManager::start_dialogue);
	ClassDB::bind_method(D_METHOD("continue_dialogue", "next_id"), &DialogueManager::continue_dialogue);
	ClassDB::bind_method(D_METHOD("end_current_dialogue"), &DialogueManager::end_current_dialogue);
	ClassDB::bind_method(D_METHOD("is_in_dialogue"), &DialogueManager::is_in_dialogue);
	
	// Yarn integration
	ClassDB::bind_method(D_METHOD("load_yarn_file", "file_path"), &DialogueManager::load_yarn_file);
	ClassDB::bind_method(D_METHOD("start_yarn_dialogue", "yarn_id", "start_node"), &DialogueManager::start_yarn_dialogue, DEFVAL(""));
	ClassDB::bind_method(D_METHOD("set_yarn_variable", "yarn_id", "variable", "value"), &DialogueManager::set_yarn_variable);
	ClassDB::bind_method(D_METHOD("get_yarn_variable", "yarn_id", "variable"), &DialogueManager::get_yarn_variable);
	
	// Global variables
	ClassDB::bind_method(D_METHOD("set_global_variable", "name", "value"), &DialogueManager::set_global_variable);
	ClassDB::bind_method(D_METHOD("get_global_variable", "name", "default"), &DialogueManager::get_global_variable, DEFVAL(Variant()));
	ClassDB::bind_method(D_METHOD("has_global_variable", "name"), &DialogueManager::has_global_variable);
	
	// Speaker management
	ClassDB::bind_method(D_METHOD("set_current_speaker", "speaker"), &DialogueManager::set_current_speaker);
	ClassDB::bind_method(D_METHOD("get_current_speaker"), &DialogueManager::get_current_speaker);
	
	// History
	ClassDB::bind_method(D_METHOD("add_to_conversation_history", "entry"), &DialogueManager::add_to_conversation_history);
	ClassDB::bind_method(D_METHOD("get_conversation_history"), &DialogueManager::get_conversation_history);
	ClassDB::bind_method(D_METHOD("clear_conversation_history"), &DialogueManager::clear_conversation_history);
	
	// Consciousness
	ClassDB::bind_method(D_METHOD("apply_global_consciousness_effect", "intensity"), &DialogueManager::apply_global_consciousness_effect);
	ClassDB::bind_method(D_METHOD("get_global_consciousness_effect"), &DialogueManager::get_global_consciousness_effect);
	ClassDB::bind_method(D_METHOD("reset_global_consciousness_effect"), &DialogueManager::reset_global_consciousness_effect);
	
	// Utility
	ClassDB::bind_method(D_METHOD("get_registered_dialogue_ids"), &DialogueManager::get_registered_dialogue_ids);
	ClassDB::bind_method(D_METHOD("clear_all_dialogues"), &DialogueManager::clear_all_dialogues);
	
	// Save/Load
	ClassDB::bind_method(D_METHOD("save_state"), &DialogueManager::save_state);
	ClassDB::bind_method(D_METHOD("load_state", "state"), &DialogueManager::load_state);
	
	// Properties
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "current_speaker"), "set_current_speaker", "get_current_speaker");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "global_consciousness_effect", PROPERTY_HINT_RANGE, "-10.0,10.0"), "", "get_global_consciousness_effect");
	
	// Signals
	ADD_SIGNAL(MethodInfo("dialogue_started", PropertyInfo(Variant::STRING, "dialogue_id")));
	ADD_SIGNAL(MethodInfo("dialogue_ended"));
	ADD_SIGNAL(MethodInfo("dialogue_continued", PropertyInfo(Variant::STRING, "next_id")));
	ADD_SIGNAL(MethodInfo("speaker_changed", PropertyInfo(Variant::STRING, "new_speaker")));
	ADD_SIGNAL(MethodInfo("global_consciousness_effect_changed", PropertyInfo(Variant::FLOAT, "new_level")));
}

void DialogueManager::register_dialogue(const String &p_id, DialogueNode* p_dialogue) {
	if (p_dialogue == nullptr) {
		print_error("DialogueManager: Cannot register null dialogue");
		return;
	}
	
	registered_dialogues[p_id] = p_dialogue;
	
	// Connect to dialogue signals
	if (!p_dialogue->is_connected("dialogue_ended", callable_mp(this, &DialogueManager::_on_dialogue_ended))) {
		p_dialogue->connect("dialogue_ended", callable_mp(this, &DialogueManager::_on_dialogue_ended));
	}
	if (!p_dialogue->is_connected("response_selected", callable_mp(this, &DialogueManager::_on_response_selected))) {
		p_dialogue->connect("response_selected", callable_mp(this, &DialogueManager::_on_response_selected));
	}
	if (!p_dialogue->is_connected("consciousness_effect_applied", callable_mp(this, &DialogueManager::_on_consciousness_effect_applied))) {
		p_dialogue->connect("consciousness_effect_applied", callable_mp(this, &DialogueManager::_on_consciousness_effect_applied));
	}
}

void DialogueManager::unregister_dialogue(const String &p_id) {
	if (registered_dialogues.has(p_id)) {
		DialogueNode* dialogue = registered_dialogues[p_id];
		
		// Disconnect signals
		if (dialogue->is_connected("dialogue_ended", callable_mp(this, &DialogueManager::_on_dialogue_ended))) {
			dialogue->disconnect("dialogue_ended", callable_mp(this, &DialogueManager::_on_dialogue_ended));
		}
		if (dialogue->is_connected("response_selected", callable_mp(this, &DialogueManager::_on_response_selected))) {
			dialogue->disconnect("response_selected", callable_mp(this, &DialogueManager::_on_response_selected));
		}
		if (dialogue->is_connected("consciousness_effect_applied", callable_mp(this, &DialogueManager::_on_consciousness_effect_applied))) {
			dialogue->disconnect("consciousness_effect_applied", callable_mp(this, &DialogueManager::_on_consciousness_effect_applied));
		}
		
		registered_dialogues.erase(p_id);
	}
}

DialogueNode* DialogueManager::get_dialogue(const String &p_id) const {
	if (registered_dialogues.has(p_id)) {
		return registered_dialogues[p_id];
	}
	return nullptr;
}

bool DialogueManager::has_dialogue(const String &p_id) const {
	return registered_dialogues.has(p_id);
}

bool DialogueManager::start_dialogue(const String &p_dialogue_id) {
	if (current_dialogue != nullptr) {
		end_current_dialogue();
	}
	
	DialogueNode* dialogue = get_dialogue(p_dialogue_id);
	if (dialogue == nullptr) {
		print_error("DialogueManager: Dialogue with ID '" + p_dialogue_id + "' not found");
		return false;
	}
	
	current_dialogue = dialogue;
	current_dialogue->start_dialogue();
	
	// Update speaker from dialogue data
	DialogueNode::DialogueData data = current_dialogue->get_dialogue_data();
	if (!data.speaker_name.is_empty()) {
		set_current_speaker(data.speaker_name);
	}
	
	emit_signal("dialogue_started", p_dialogue_id);
	return true;
}

bool DialogueManager::continue_dialogue(const String &p_next_id) {
	if (current_dialogue == nullptr) {
		return start_dialogue(p_next_id);
	}
	
	// End current dialogue and start next one
	end_current_dialogue();
	bool success = start_dialogue(p_next_id);
	
	if (success) {
		emit_signal("dialogue_continued", p_next_id);
	}
	
	return success;
}

void DialogueManager::end_current_dialogue() {
	if (current_dialogue != nullptr) {
		current_dialogue->end_dialogue();
		current_dialogue = nullptr;
		emit_signal("dialogue_ended");
	}
}

DialogueNode* DialogueManager::get_current_dialogue() const {
	return current_dialogue;
}

bool DialogueManager::is_in_dialogue() const {
	return current_dialogue != nullptr && current_dialogue->is_dialogue_active();
}

bool DialogueManager::load_yarn_file(const String &p_file_path) {
	Ref<FileAccess> file = FileAccess::open(p_file_path, FileAccess::READ);
	if (file.is_null()) {
		print_error("DialogueManager: Could not open Yarn file: " + p_file_path);
		return false;
	}
	
	String content = file->get_as_text();
	file->close();
	
	// Generate yarn ID from file path
	String yarn_id = p_file_path.get_file().get_basename();
	
	return parse_yarn_json(content, yarn_id);
}

bool DialogueManager::start_yarn_dialogue(const String &p_yarn_id, const String &p_start_node) {
	if (!yarn_dialogues.has(p_yarn_id)) {
		print_error("DialogueManager: Yarn dialogue '" + p_yarn_id + "' not loaded");
		return false;
	}
	
	YarnDialogueData yarn_data = yarn_dialogues[p_yarn_id];
	String start_node = p_start_node.is_empty() ? yarn_data.start_node_id : p_start_node;
	
	return start_dialogue(p_yarn_id + "_" + start_node);
}

void DialogueManager::set_yarn_variable(const String &p_yarn_id, const String &p_variable, const Variant &p_value) {
	if (yarn_dialogues.has(p_yarn_id)) {
		yarn_dialogues[p_yarn_id].variables[p_variable] = p_value;
	}
}

Variant DialogueManager::get_yarn_variable(const String &p_yarn_id, const String &p_variable) const {
	if (yarn_dialogues.has(p_yarn_id)) {
		const HashMap<String, Variant> &vars = yarn_dialogues[p_yarn_id].variables;
		if (vars.has(p_variable)) {
			return vars[p_variable];
		}
	}
	return Variant();
}

void DialogueManager::set_global_variable(const String &p_name, const Variant &p_value) {
	global_variables[p_name] = p_value;
	
	// Also set in current dialogue if active
	if (current_dialogue != nullptr) {
		current_dialogue->set_variable(p_name, p_value);
	}
}

Variant DialogueManager::get_global_variable(const String &p_name, const Variant &p_default) const {
	if (global_variables.has(p_name)) {
		return global_variables[p_name];
	}
	return p_default;
}

bool DialogueManager::has_global_variable(const String &p_name) const {
	return global_variables.has(p_name);
}

void DialogueManager::set_current_speaker(const String &p_speaker) {
	if (current_speaker != p_speaker) {
		current_speaker = p_speaker;
		emit_signal("speaker_changed", p_speaker);
	}
}

String DialogueManager::get_current_speaker() const {
	return current_speaker;
}

void DialogueManager::add_to_conversation_history(const String &p_entry) {
	conversation_history.push_back(p_entry);
	
	// Limit history size
	const int MAX_HISTORY_SIZE = 1000;
	if (conversation_history.size() > MAX_HISTORY_SIZE) {
		conversation_history.remove_at(0);
	}
}

Vector<String> DialogueManager::get_conversation_history() const {
	return conversation_history;
}

void DialogueManager::clear_conversation_history() {
	conversation_history.clear();
}

void DialogueManager::apply_global_consciousness_effect(float p_intensity) {
	global_consciousness_effect += p_intensity;
	global_consciousness_effect = CLAMP(global_consciousness_effect, -20.0f, 20.0f);
	
	emit_signal("global_consciousness_effect_changed", global_consciousness_effect);
}

float DialogueManager::get_global_consciousness_effect() const {
	return global_consciousness_effect;
}

void DialogueManager::reset_global_consciousness_effect() {
	global_consciousness_effect = 0.0f;
	emit_signal("global_consciousness_effect_changed", 0.0f);
}

PackedStringArray DialogueManager::get_registered_dialogue_ids() const {
	PackedStringArray ids;
	for (const KeyValue<String, DialogueNode*> &E : registered_dialogues) {
		ids.push_back(E.key);
	}
	return ids;
}

void DialogueManager::clear_all_dialogues() {
	for (const KeyValue<String, DialogueNode*> &E : registered_dialogues) {
		// Disconnect signals before clearing
		unregister_dialogue(E.key);
	}
	registered_dialogues.clear();
	current_dialogue = nullptr;
}

Dictionary DialogueManager::save_state() const {
	Dictionary state;
	state["global_variables"] = global_variables;
	state["conversation_history"] = conversation_history;
	state["global_consciousness_effect"] = global_consciousness_effect;
	state["current_speaker"] = current_speaker;
	
	// Save yarn dialogue states
	Dictionary yarn_state;
	for (const KeyValue<String, YarnDialogueData> &E : yarn_dialogues) {
		Dictionary yarn_data;
		// Convert HashMap to Dictionary
		Dictionary variables_dict;
		for (const KeyValue<String, Variant> &var : E.value.variables) {
			variables_dict[var.key] = var.value;
		}
		yarn_data["variables"] = variables_dict;
		yarn_data["file_path"] = E.value.yarn_file_path;
		yarn_data["start_node"] = E.value.start_node_id;
		yarn_state[E.key] = yarn_data;
	}
	state["yarn_dialogues"] = yarn_state;
	
	return state;
}

void DialogueManager::load_state(const Dictionary &p_state) {
	if (p_state.has("global_variables")) {
		global_variables = p_state["global_variables"];
	}
	if (p_state.has("conversation_history")) {
		conversation_history = p_state["conversation_history"];
	}
	if (p_state.has("global_consciousness_effect")) {
		global_consciousness_effect = p_state["global_consciousness_effect"];
	}
	if (p_state.has("current_speaker")) {
		current_speaker = p_state["current_speaker"];
	}
	
	// Load yarn dialogue states
	if (p_state.has("yarn_dialogues")) {
		Dictionary yarn_state = p_state["yarn_dialogues"];
		for (const Variant* E = yarn_state.next(); E; E = yarn_state.next(E)) {
			String yarn_id = *E;
			Dictionary yarn_data = yarn_state[yarn_id];
			
			YarnDialogueData data;
			// Convert Dictionary to HashMap
			Dictionary variables_dict = yarn_data.get("variables", Dictionary());
			data.variables.clear();
			for (const Variant* key = variables_dict.next(); key; key = variables_dict.next(key)) {
				data.variables[*key] = variables_dict[*key];
			}
			data.yarn_file_path = yarn_data.get("file_path", "");
			data.start_node_id = yarn_data.get("start_node", "");
			
			yarn_dialogues[yarn_id] = data;
		}
	}
}

bool DialogueManager::parse_yarn_json(const String &p_json_content, const String &p_yarn_id) {
	// Simplified Yarn parser - in a real implementation, this would be more robust
	JSON json;
	Error err = json.parse(p_json_content);
	if (err != OK) {
		print_error("DialogueManager: Failed to parse Yarn JSON: " + json.get_error_message());
		return false;
	}
	
	Variant parsed = json.get_data();
	if (parsed.get_type() != Variant::ARRAY) {
		print_error("DialogueManager: Yarn JSON must be an array of nodes");
		return false;
	}
	
	Array nodes = parsed;
	YarnDialogueData yarn_data;
	yarn_data.yarn_file_path = "";
	yarn_data.start_node_id = "";
	
	// Convert each Yarn node to a DialogueNode
	for (int i = 0; i < nodes.size(); i++) {
		Dictionary node = nodes[i];
		if (node.has("title")) {
			String node_title = node["title"];
			String dialogue_id = p_yarn_id + "_" + node_title;
			
			// Create DialogueNode
			DialogueNode* dialogue = memnew(DialogueNode);
			DialogueNode::DialogueData data = convert_yarn_node_to_dialogue(node);
			dialogue->set_dialogue_data(data);
			dialogue->set_dialogue_id(dialogue_id);
			
			register_dialogue(dialogue_id, dialogue);
			
			// Set first node as start node
			if (yarn_data.start_node_id.is_empty()) {
				yarn_data.start_node_id = node_title;
			}
		}
	}
	
	yarn_dialogues[p_yarn_id] = yarn_data;
	return true;
}

DialogueNode::DialogueData DialogueManager::convert_yarn_node_to_dialogue(const Dictionary &p_yarn_node) {
	DialogueNode::DialogueData data;
	
	if (p_yarn_node.has("body")) {
		data.dialogue_text = p_yarn_node["body"];
	}
	
	// Parse speaker from tags or body
	if (p_yarn_node.has("tags")) {
		Array tags = p_yarn_node["tags"];
		for (int i = 0; i < tags.size(); i++) {
			String tag = tags[i];
			if (tag.begins_with("speaker:")) {
				data.speaker_name = tag.substr(8);
				break;
			}
		}
	}
	
	// Convert options to responses (simplified)
	// In a full implementation, this would parse Yarn's option syntax
	
	return data;
}

DialogueNode::DialogueResponse DialogueManager::convert_yarn_option_to_response(const Dictionary &p_yarn_option) {
	DialogueNode::DialogueResponse response;
	
	if (p_yarn_option.has("text")) {
		response.text = p_yarn_option["text"];
	}
	if (p_yarn_option.has("destination")) {
		response.next_dialogue_id = p_yarn_option["destination"];
	}
	
	return response;
}

void DialogueManager::_on_dialogue_ended() {
	current_dialogue = nullptr;
}

void DialogueManager::_on_response_selected(int p_response_index) {
	if (current_dialogue != nullptr) {
		DialogueNode::DialogueResponse response = current_dialogue->get_response(p_response_index);
		
		// Add to conversation history
		String history_entry = current_speaker + ": " + response.text;
		add_to_conversation_history(history_entry);
		
		// Handle dialogue transitions
		if (!response.next_dialogue_id.is_empty()) {
			continue_dialogue(response.next_dialogue_id);
		}
	}
}

void DialogueManager::_on_consciousness_effect_applied(float p_intensity) {
	apply_global_consciousness_effect(p_intensity * 0.5f); // Dampen individual effects for global
}