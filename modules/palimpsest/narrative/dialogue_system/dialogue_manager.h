/**************************************************************************/
/*  dialogue_manager.h                                                    */
/**************************************************************************/
/*                         This file is part of:                          */
/*                        PALIMPSEST ENGINE                               */
/**************************************************************************/

#pragma once

#include "scene/main/node.h"
#include "core/templates/hash_map.h"
#include "core/io/json.h"
#include "dialogue_node.h"

class DialogueManager : public Node {
	GDCLASS(DialogueManager, Node);

public:
	struct YarnDialogueData {
		String yarn_file_path;
		String start_node_id;
		HashMap<String, Variant> variables;
	};

private:
	HashMap<String, DialogueNode*> registered_dialogues;
	DialogueNode* current_dialogue = nullptr;
	String current_speaker = "";
	
	// Yarn Spinner integration
	HashMap<String, YarnDialogueData> yarn_dialogues;
	JSON json_parser;
	
	// Global dialogue state
	Dictionary global_variables;
	Vector<String> conversation_history;
	
	// Consciousness tracking
	float global_consciousness_effect = 0.0f;

protected:
	static void _bind_methods();

public:
	DialogueManager();
	~DialogueManager();

	// Dialogue registration and management
	void register_dialogue(const String &p_id, DialogueNode* p_dialogue);
	void unregister_dialogue(const String &p_id);
	DialogueNode* get_dialogue(const String &p_id) const;
	bool has_dialogue(const String &p_id) const;
	
	// Dialogue flow control
	bool start_dialogue(const String &p_dialogue_id);
	bool continue_dialogue(const String &p_next_id);
	void end_current_dialogue();
	DialogueNode* get_current_dialogue() const;
	bool is_in_dialogue() const;
	
	// Yarn Spinner integration
	bool load_yarn_file(const String &p_file_path);
	bool start_yarn_dialogue(const String &p_yarn_id, const String &p_start_node = "");
	void set_yarn_variable(const String &p_yarn_id, const String &p_variable, const Variant &p_value);
	Variant get_yarn_variable(const String &p_yarn_id, const String &p_variable) const;
	
	// Global state management
	void set_global_variable(const String &p_name, const Variant &p_value);
	Variant get_global_variable(const String &p_name, const Variant &p_default = Variant()) const;
	bool has_global_variable(const String &p_name) const;
	
	// Speaker management
	void set_current_speaker(const String &p_speaker);
	String get_current_speaker() const;
	
	// Conversation history
	void add_to_conversation_history(const String &p_entry);
	Vector<String> get_conversation_history() const;
	void clear_conversation_history();
	
	// Consciousness effects
	void apply_global_consciousness_effect(float p_intensity);
	float get_global_consciousness_effect() const;
	void reset_global_consciousness_effect();
	
	// Utility functions
	PackedStringArray get_registered_dialogue_ids() const;
	void clear_all_dialogues();
	
	// Save/Load system
	Dictionary save_state() const;
	void load_state(const Dictionary &p_state);

private:
	// Yarn parsing helpers
	bool parse_yarn_json(const String &p_json_content, const String &p_yarn_id);
	DialogueNode::DialogueData convert_yarn_node_to_dialogue(const Dictionary &p_yarn_node);
	DialogueNode::DialogueResponse convert_yarn_option_to_response(const Dictionary &p_yarn_option);
	
	// Internal dialogue management
	void _on_dialogue_ended();
	void _on_response_selected(int p_response_index);
	void _on_consciousness_effect_applied(float p_intensity);
};