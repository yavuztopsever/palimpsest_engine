/**************************************************************************/
/*  dialogue_node.h                                                       */
/**************************************************************************/
/*                         This file is part of:                          */
/*                        PALIMPSEST ENGINE                               */
/**************************************************************************/

#pragma once

#include "scene/main/node.h"
#include "core/templates/vector.h"
#include "core/string/ustring.h"
#include "core/object/gdvirtual.gen.inc"

class DialogueNode : public Node {
	GDCLASS(DialogueNode, Node);

public:
	enum ResponseType {
		RESPONSE_NORMAL,     // Standard dialogue choice
		RESPONSE_SKILL_CHECK, // Requires skill check (Disco Elysium style)
		RESPONSE_THOUGHT,    // Internal thought/reflection
		RESPONSE_BUREAUCRATIC, // Official/formal response
		RESPONSE_MEMORY,     // Triggered by memory fragment
		RESPONSE_LOCKED      // Unavailable (shows why)
	};

	struct DialogueResponse {
		String text;
		ResponseType type = RESPONSE_NORMAL;
		String skill_requirement; // e.g., "Encyclopedia [Medium]"
		int skill_threshold = 0;
		String condition_variable; // Variable that must be true
		String next_dialogue_id;
		bool is_exit = false;
		Dictionary metadata; // For custom data
	};

	struct DialogueData {
		String speaker_name;
		String speaker_portrait; // Path to portrait texture
		String dialogue_text;
		Vector<DialogueResponse> responses;
		String background_music;
		String ambient_sound;
		String location_description; // For setting context
		float consciousness_effect = 0.0f; // How this dialogue affects reality
	};

private:
	String dialogue_id;
	DialogueData current_dialogue;
	Vector<DialogueData> dialogue_history;
	Dictionary variables; // For tracking dialogue state
	bool is_active = false;
	
	// Consciousness effects
	float reality_distortion_accumulator = 0.0f;

protected:
	static void _bind_methods();

public:
	DialogueNode();
	~DialogueNode();

	// Core dialogue functions
	void set_dialogue_id(const String &p_id);
	String get_dialogue_id() const;
	
	void set_dialogue_data(const DialogueData &p_data);
	DialogueData get_dialogue_data() const;
	
	// Dictionary versions for property binding
	void _set_dialogue_data_dict(const Dictionary &p_dict);
	Dictionary _get_dialogue_data_dict() const;
	
	void add_response(const DialogueResponse &p_response);
	void remove_response(int p_index);
	DialogueResponse get_response(int p_index) const;
	int get_response_count() const;
	
	// Variable management (for tracking game state)
	void set_variable(const String &p_name, const Variant &p_value);
	Variant get_variable(const String &p_name, const Variant &p_default = Variant()) const;
	bool has_variable(const String &p_name) const;
	
	// Dialogue flow control
	void start_dialogue();
	void select_response(int p_response_index);
	void end_dialogue();
	bool is_dialogue_active() const;
	
	// Skill check system
	bool evaluate_skill_check(const String &p_skill, int p_threshold) const;
	bool can_select_response(int p_response_index) const;
	
	// Consciousness effects
	void apply_consciousness_effect(float p_intensity);
	float get_reality_distortion() const;
	void reset_reality_distortion();
	
	// History management
	void add_to_history(const DialogueData &p_data);
	Vector<DialogueData> get_dialogue_history() const;
	void clear_history();

	// Signals
	GDVIRTUAL2(_on_dialogue_started, String, Dictionary);
	GDVIRTUAL1(_on_response_selected, int);
	GDVIRTUAL0(_on_dialogue_ended);
	GDVIRTUAL2(_on_skill_check_attempted, String, bool);
	GDVIRTUAL1(_on_consciousness_effect_applied, float);
};

VARIANT_ENUM_CAST(DialogueNode::ResponseType);