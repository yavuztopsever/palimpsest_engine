/**************************************************************************/
/*  wfc_generator.h                                                       */
/**************************************************************************/
/*                         This file is part of:                          */
/*                        PALIMPSEST ENGINE                               */
/**************************************************************************/

#pragma once

#include "scene/main/node.h"
#include "core/math/vector2i.h"
#include "core/math/vector3i.h"
#include "core/templates/vector.h"
#include "core/templates/hash_map.h"
#include "tile_constraint.h"

class WFCGenerator : public Node {
	GDCLASS(WFCGenerator, Node);

public:
	enum GenerationMode {
		MODE_2D,
		MODE_3D
	};

	enum BacktrackingMode {
		BACKTRACK_NONE,     // No backtracking - may fail
		BACKTRACK_SIMPLE,   // Basic backtracking
		BACKTRACK_ADVANCED  // Advanced with heuristics
	};

	struct WFCTile {
		String tile_id;
		PackedStringArray compatible_north;
		PackedStringArray compatible_south; 
		PackedStringArray compatible_east;
		PackedStringArray compatible_west;
		PackedStringArray compatible_up;    // For 3D
		PackedStringArray compatible_down;  // For 3D
		float weight = 1.0f;
		bool is_edge_tile = false;
		Dictionary metadata;
		
		// Consciousness effects
		float consciousness_influence = 0.0f;
		String required_consciousness_type = "";
	};

	struct WFCCell {
		Vector<String> possible_tiles;
		String collapsed_tile = "";
		bool is_collapsed = false;
		float entropy = 0.0f;
		Vector3i position;
		
		// Consciousness tracking
		float consciousness_accumulation = 0.0f;
		String dominant_consciousness_type = "";
	};

private:
	GenerationMode mode = MODE_2D;
	BacktrackingMode backtracking_mode = BACKTRACK_SIMPLE;
	
	// Grid dimensions
	Vector3i grid_size = Vector3i(10, 10, 1);
	Vector<WFCCell> grid;
	
	// Tile definitions
	HashMap<String, WFCTile> tile_definitions;
	Vector<String> available_tiles;
	
	// Generation settings
	int max_iterations = 10000;
	bool use_multithreading = true;
	bool enable_consciousness_influence = true;
	
	// Generation state
	bool is_generating = false;
	bool has_failed = false;
	Vector<Vector3i> generation_stack; // For backtracking
	HashMap<Vector3i, Vector<String>> backup_states;
	
	// Consciousness integration
	HashMap<String, float> consciousness_field_values;
	float consciousness_bias_strength = 1.0f;

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	WFCGenerator();
	~WFCGenerator();

	// Core generation
	bool generate();
	bool generate_step(); // Single iteration step
	void reset_generation();
	void clear_grid();
	
	// Setup
	void set_mode(GenerationMode p_mode);
	GenerationMode get_mode() const;
	
	void set_grid_size(const Vector3i &p_size);
	Vector3i get_grid_size() const;
	
	void set_backtracking_mode(BacktrackingMode p_mode);
	BacktrackingMode get_backtracking_mode() const;
	
	// Tile management
	void add_tile_definition(const String &p_id, const WFCTile &p_tile);
	void remove_tile_definition(const String &p_id);
	WFCTile get_tile_definition(const String &p_id) const;
	bool has_tile_definition(const String &p_id) const;
	PackedStringArray get_tile_ids() const;
	void clear_tile_definitions();
	
	// Grid access
	WFCCell get_cell(const Vector3i &p_position) const;
	void set_cell(const Vector3i &p_position, const WFCCell &p_cell);
	bool is_valid_position(const Vector3i &p_position) const;
	
	String get_collapsed_tile(const Vector3i &p_position) const;
	bool is_cell_collapsed(const Vector3i &p_position) const;
	float get_cell_entropy(const Vector3i &p_position) const;
	
	// Generation control
	void set_max_iterations(int p_iterations);
	int get_max_iterations() const;
	
	bool is_generation_active() const;
	bool has_generation_failed() const;
	float get_generation_progress() const;
	
	// Consciousness integration
	void set_consciousness_influence_enabled(bool p_enabled);
	bool is_consciousness_influence_enabled() const;
	
	void set_consciousness_field_value(const String &p_type, float p_value);
	float get_consciousness_field_value(const String &p_type) const;
	
	// Utility functions
	void load_tiles_from_json(const String &p_file_path);
	Dictionary export_generation_data() const;

private:
	// Core WFC algorithm
	Vector3i find_lowest_entropy_cell() const;
	void collapse_cell(const Vector3i &p_position);
	void propagate_constraints(const Vector3i &p_position);
	void update_cell_entropy(const Vector3i &p_position);
	
	// Grid helpers
	int get_grid_index(const Vector3i &p_position) const;
	Vector3i get_grid_position(int p_index) const;
	void initialize_grid();
	
	// Consciousness influence
	void apply_consciousness_influence_to_cell(WFCCell &p_cell) const;
	float calculate_consciousness_weight_for_tile(const String &p_tile_id, const Vector3i &p_position) const;
};
