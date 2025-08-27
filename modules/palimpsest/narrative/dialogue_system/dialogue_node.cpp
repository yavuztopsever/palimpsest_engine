/**************************************************************************/
/*  dialogue_node.cpp                                                     */
/**************************************************************************/
/*                         This file is part of:                          */
/*                        PALIMPSEST ENGINE                               */
/**************************************************************************/

#include "dialogue_node.h"
#include "core/object/class_db.h"
#include "core/io/resource_loader.h"

DialogueNode::DialogueNode() {
	dialogue_id = "";
	is_active = false;
	reality_distortion_accumulator = 0.0f;
}

DialogueNode::~DialogueNode() {
}

void DialogueNode::_bind_methods() {
	// Core dialogue methods
	ClassDB::bind_method(D_METHOD("set_dialogue_id", "id"), &DialogueNode::set_dialogue_id);
	ClassDB::bind_method(D_METHOD("get_dialogue_id"), &DialogueNode::get_dialogue_id);
	
	ClassDB::bind_method(D_METHOD("start_dialogue"), &DialogueNode::start_dialogue);
	ClassDB::bind_method(D_METHOD("select_response", "response_index"), &DialogueNode::select_response);
	ClassDB::bind_method(D_METHOD("end_dialogue"), &DialogueNode::end_dialogue);
	ClassDB::bind_method(D_METHOD("is_dialogue_active"), &DialogueNode::is_dialogue_active);
	
	// Response management
	ClassDB::bind_method(D_METHOD("get_response_count"), &DialogueNode::get_response_count);
	ClassDB::bind_method(D_METHOD("can_select_response", "response_index"), &DialogueNode::can_select_response);
	
	// Variable management
	ClassDB::bind_method(D_METHOD("set_variable", "name", "value"), &DialogueNode::set_variable);
	ClassDB::bind_method(D_METHOD("get_variable", "name", "default"), &DialogueNode::get_variable, DEFVAL(Variant()));
	ClassDB::bind_method(D_METHOD("has_variable", "name"), &DialogueNode::has_variable);
	
	// Skill checks
	ClassDB::bind_method(D_METHOD("evaluate_skill_check", "skill", "threshold"), &DialogueNode::evaluate_skill_check);
	
	// Consciousness effects
	ClassDB::bind_method(D_METHOD("apply_consciousness_effect", "intensity"), &DialogueNode::apply_consciousness_effect);
	ClassDB::bind_method(D_METHOD("get_reality_distortion"), &DialogueNode::get_reality_distortion);
	ClassDB::bind_method(D_METHOD("reset_reality_distortion"), &DialogueNode::reset_reality_distortion);
	
	// History
	ClassDB::bind_method(D_METHOD("clear_history"), &DialogueNode::clear_history);
	
	// Properties
	ClassDB::bind_method(D_METHOD("_set_dialogue_data_dict", "data"), &DialogueNode::_set_dialogue_data_dict);
	ClassDB::bind_method(D_METHOD("_get_dialogue_data_dict"), &DialogueNode::_get_dialogue_data_dict);
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "dialogue_data"), "_set_dialogue_data_dict", "_get_dialogue_data_dict");
	
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "dialogue_id"), "set_dialogue_id", "get_dialogue_id");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "reality_distortion", PROPERTY_HINT_RANGE, "0.0,10.0"), "", "get_reality_distortion");
	
	// Signals
	ADD_SIGNAL(MethodInfo("dialogue_started", PropertyInfo(Variant::STRING, "dialogue_id")));
	ADD_SIGNAL(MethodInfo("response_selected", PropertyInfo(Variant::INT, "response_index")));
	ADD_SIGNAL(MethodInfo("dialogue_ended"));
	ADD_SIGNAL(MethodInfo("skill_check_attempted", PropertyInfo(Variant::STRING, "skill"), PropertyInfo(Variant::BOOL, "success")));
	ADD_SIGNAL(MethodInfo("consciousness_effect_applied", PropertyInfo(Variant::FLOAT, "intensity")));
	
	// Enums
	BIND_ENUM_CONSTANT(RESPONSE_NORMAL);
	BIND_ENUM_CONSTANT(RESPONSE_SKILL_CHECK);
	BIND_ENUM_CONSTANT(RESPONSE_THOUGHT);
	BIND_ENUM_CONSTANT(RESPONSE_BUREAUCRATIC);
	BIND_ENUM_CONSTANT(RESPONSE_MEMORY);
	BIND_ENUM_CONSTANT(RESPONSE_LOCKED);
}

void DialogueNode::set_dialogue_id(const String &p_id) {
	dialogue_id = p_id;
}

String DialogueNode::get_dialogue_id() const {
	return dialogue_id;
}

void DialogueNode::set_dialogue_data(const DialogueData &p_data) {
	current_dialogue = p_data;
}

DialogueNode::DialogueData DialogueNode::get_dialogue_data() const {
	return current_dialogue;
}

void DialogueNode::_set_dialogue_data_dict(const Dictionary &p_dict) {
	DialogueData data;
	data.speaker_name = p_dict.get("speaker_name", "");
	data.speaker_portrait = p_dict.get("speaker_portrait", "");
	data.dialogue_text = p_dict.get("dialogue_text", "");
	data.background_music = p_dict.get("background_music", "");
	data.ambient_sound = p_dict.get("ambient_sound", "");
	data.location_description = p_dict.get("location_description", "");
	data.consciousness_effect = p_dict.get("consciousness_effect", 0.0f);
	
	// Handle responses array if present
	if (p_dict.has("responses")) {
		Array responses_array = p_dict.get("responses", Array());
		data.responses.clear();
		for (int i = 0; i < responses_array.size(); i++) {
			Dictionary response_dict = responses_array[i];
			DialogueResponse response;
			response.text = response_dict.get("text", "");
			response.type = (ResponseType)(int)response_dict.get("type", RESPONSE_NORMAL);
			response.skill_requirement = response_dict.get("skill_requirement", "");
			response.skill_threshold = response_dict.get("skill_threshold", 0);
			response.condition_variable = response_dict.get("condition_variable", "");
			response.next_dialogue_id = response_dict.get("next_dialogue_id", "");
			response.is_exit = response_dict.get("is_exit", false);
			response.metadata = response_dict.get("metadata", Dictionary());
			data.responses.push_back(response);
		}
	}
	
	set_dialogue_data(data);
}

Dictionary DialogueNode::_get_dialogue_data_dict() const {
	Dictionary dict;
	dict["speaker_name"] = current_dialogue.speaker_name;
	dict["speaker_portrait"] = current_dialogue.speaker_portrait;
	dict["dialogue_text"] = current_dialogue.dialogue_text;
	dict["background_music"] = current_dialogue.background_music;
	dict["ambient_sound"] = current_dialogue.ambient_sound;
	dict["location_description"] = current_dialogue.location_description;
	dict["consciousness_effect"] = current_dialogue.consciousness_effect;
	
	// Convert responses to array of dictionaries
	Array responses_array;
	for (int i = 0; i < current_dialogue.responses.size(); i++) {
		const DialogueResponse &response = current_dialogue.responses[i];
		Dictionary response_dict;
		response_dict["text"] = response.text;
		response_dict["type"] = (int)response.type;
		response_dict["skill_requirement"] = response.skill_requirement;
		response_dict["skill_threshold"] = response.skill_threshold;
		response_dict["condition_variable"] = response.condition_variable;
		response_dict["next_dialogue_id"] = response.next_dialogue_id;
		response_dict["is_exit"] = response.is_exit;
		response_dict["metadata"] = response.metadata;
		responses_array.push_back(response_dict);
	}
	dict["responses"] = responses_array;
	
	return dict;
}

void DialogueNode::add_response(const DialogueResponse &p_response) {
	current_dialogue.responses.push_back(p_response);
}

void DialogueNode::remove_response(int p_index) {
	if (p_index >= 0 && p_index < current_dialogue.responses.size()) {
		current_dialogue.responses.remove_at(p_index);
	}
}

DialogueNode::DialogueResponse DialogueNode::get_response(int p_index) const {
	if (p_index >= 0 && p_index < current_dialogue.responses.size()) {
		return current_dialogue.responses[p_index];
	}
	return DialogueResponse();
}

int DialogueNode::get_response_count() const {
	return current_dialogue.responses.size();
}

void DialogueNode::set_variable(const String &p_name, const Variant &p_value) {
	variables[p_name] = p_value;
}

Variant DialogueNode::get_variable(const String &p_name, const Variant &p_default) const {
	if (variables.has(p_name)) {
		return variables[p_name];
	}
	return p_default;
}

bool DialogueNode::has_variable(const String &p_name) const {
	return variables.has(p_name);
}

void DialogueNode::start_dialogue() {
	if (!is_active) {
		is_active = true;
		add_to_history(current_dialogue);
		emit_signal("dialogue_started", dialogue_id);
		
		// Convert DialogueData to Dictionary for virtual call
		Dictionary dialogue_dict;
		dialogue_dict["speaker_name"] = current_dialogue.speaker_name;
		dialogue_dict["speaker_portrait"] = current_dialogue.speaker_portrait;
		dialogue_dict["dialogue_text"] = current_dialogue.dialogue_text;
		dialogue_dict["background_music"] = current_dialogue.background_music;
		dialogue_dict["ambient_sound"] = current_dialogue.ambient_sound;
		dialogue_dict["location_description"] = current_dialogue.location_description;
		dialogue_dict["consciousness_effect"] = current_dialogue.consciousness_effect;
		
		GDVIRTUAL_CALL(_on_dialogue_started, dialogue_id, dialogue_dict);
	}
}

void DialogueNode::select_response(int p_response_index) {
	if (!is_active || p_response_index < 0 || p_response_index >= current_dialogue.responses.size()) {
		return;
	}
	
	DialogueResponse response = current_dialogue.responses[p_response_index];
	
	// Check if response can be selected
	if (!can_select_response(p_response_index)) {
		return;
	}
	
	// Handle skill checks
	if (response.type == RESPONSE_SKILL_CHECK) {
		bool success = evaluate_skill_check(response.skill_requirement, response.skill_threshold);
		emit_signal("skill_check_attempted", response.skill_requirement, success);
		GDVIRTUAL_CALL(_on_skill_check_attempted, response.skill_requirement, success);
		
		if (!success) {
			// Could add failure handling here
			return;
		}
	}
	
	// Apply consciousness effects for certain response types
	float consciousness_intensity = 0.0f;
	switch (response.type) {
		case RESPONSE_THOUGHT:
			consciousness_intensity = 0.5f;
			break;
		case RESPONSE_MEMORY:
			consciousness_intensity = 1.0f;
			break;
		case RESPONSE_BUREAUCRATIC:
			consciousness_intensity = -0.3f; // Stabilizing effect
			break;
		default:
			break;
	}
	
	if (consciousness_intensity != 0.0f) {
		apply_consciousness_effect(consciousness_intensity);
	}
	
	emit_signal("response_selected", p_response_index);
	GDVIRTUAL_CALL(_on_response_selected, p_response_index);
	
	// Handle dialogue flow
	if (response.is_exit || response.next_dialogue_id.is_empty()) {
		end_dialogue();
	} else {
		// Continue to next dialogue (would be handled by DialogueManager)
		set_dialogue_id(response.next_dialogue_id);
	}
}

void DialogueNode::end_dialogue() {
	if (is_active) {
		is_active = false;
		emit_signal("dialogue_ended");
		GDVIRTUAL_CALL(_on_dialogue_ended);
	}
}

bool DialogueNode::is_dialogue_active() const {
	return is_active;
}

bool DialogueNode::evaluate_skill_check(const String &p_skill, int p_threshold) const {
	// This is a simplified skill check - in a real implementation,
	// this would interface with a character stats system
	
	// For now, we'll use a basic random check with some bias
	// towards success based on threshold difficulty
	int dice_roll = Math::rand() % 20 + 1; // 1-20
	int skill_modifier = get_variable(p_skill + "_modifier", 0);
	
	return (dice_roll + skill_modifier) >= p_threshold;
}

bool DialogueNode::can_select_response(int p_response_index) const {
	if (p_response_index < 0 || p_response_index >= current_dialogue.responses.size()) {
		return false;
	}
	
	DialogueResponse response = current_dialogue.responses[p_response_index];
	
	// Check locked responses
	if (response.type == RESPONSE_LOCKED) {
		return false;
	}
	
	// Check condition variables
	if (!response.condition_variable.is_empty()) {
		return get_variable(response.condition_variable, false);
	}
	
	return true;
}

void DialogueNode::apply_consciousness_effect(float p_intensity) {
	reality_distortion_accumulator += p_intensity;
	
	// Clamp to reasonable range
	reality_distortion_accumulator = CLAMP(reality_distortion_accumulator, -5.0f, 10.0f);
	
	emit_signal("consciousness_effect_applied", p_intensity);
	GDVIRTUAL_CALL(_on_consciousness_effect_applied, p_intensity);
	
	// Apply visual effects based on distortion level
	if (Math::abs(reality_distortion_accumulator) > 2.0f) {
		// Trigger reality distortion effects in the environment
		// This would interface with the consciousness system
	}
}

float DialogueNode::get_reality_distortion() const {
	return reality_distortion_accumulator;
}

void DialogueNode::reset_reality_distortion() {
	reality_distortion_accumulator = 0.0f;
}

void DialogueNode::add_to_history(const DialogueData &p_data) {
	dialogue_history.push_back(p_data);
	
	// Limit history size to prevent memory bloat
	const int MAX_HISTORY_SIZE = 100;
	if (dialogue_history.size() > MAX_HISTORY_SIZE) {
		dialogue_history.remove_at(0);
	}
}

Vector<DialogueNode::DialogueData> DialogueNode::get_dialogue_history() const {
	return dialogue_history;
}

void DialogueNode::clear_history() {
	dialogue_history.clear();
}