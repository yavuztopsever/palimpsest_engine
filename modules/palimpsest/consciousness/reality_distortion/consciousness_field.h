/**************************************************************************/
/*  consciousness_field.h                                                 */
/**************************************************************************/
/*                         This file is part of:                          */
/*                        PALIMPSEST ENGINE                               */
/**************************************************************************/

#pragma once

#include "scene/3d/physics/area_3d.h"
#include "core/math/vector3.h"
#include "core/templates/vector.h"

class ConsciousnessField : public Area3D {
	GDCLASS(ConsciousnessField, Area3D);

public:
	enum FieldType {
		FIELD_DEPRESSION,    // Causes buildings to droop, colors to desaturate
		FIELD_ANXIETY,       // Creates labyrinthine geometry, increases fog
		FIELD_ANGER,         // Sharp angles, red tints, aggressive architecture  
		FIELD_MEMORY,        // Overlays from the past, temporal distortions
		FIELD_BUREAUCRATIC,  // Rigid geometry, forms materialize, reality stabilizes
		FIELD_CREATIVE,      // Impossible geometry, vibrant colors, reality flexibility
		FIELD_FEAR,          // Shadows deepen, distances stretch, walls close in
		FIELD_HOPE           // Colors brighten, spaces open up, gentle curves
	};

	enum DistortionIntensity {
		INTENSITY_SUBTLE,    // Barely noticeable effects
		INTENSITY_MODERATE,  // Obvious but not overwhelming
		INTENSITY_STRONG,    // Clearly reality-warping
		INTENSITY_EXTREME,   // Physics-defying, reality breakdown
		INTENSITY_CASCADE    // Total local reality failure
	};

private:
	FieldType field_type = FIELD_DEPRESSION;
	DistortionIntensity intensity = INTENSITY_MODERATE;
	float field_strength = 1.0f;
	float consciousness_accumulation = 0.0f;
	Vector3 distortion_vector = Vector3();
	
	// Affected objects tracking
	Vector<Node3D*> affected_objects;
	HashMap<Node3D*, Transform3D> original_transforms;
	HashMap<Node3D*, Variant> original_properties;
	
	// Visual effects
	bool visual_effects_enabled = true;
	Color base_color_tint = Color(1, 1, 1, 1);
	float fog_intensity_multiplier = 1.0f;
	
	// Temporal effects
	float time_dilation_factor = 1.0f;
	bool causes_memory_bleeds = false;
	
	// Propagation settings
	float propagation_speed = 1.0f;
	float max_propagation_distance = 10.0f;
	bool can_cascade = true;
	
protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	ConsciousnessField();
	~ConsciousnessField();

	// Core field properties
	void set_field_type(FieldType p_type);
	FieldType get_field_type() const;
	
	void set_intensity(DistortionIntensity p_intensity);
	DistortionIntensity get_intensity() const;
	
	void set_field_strength(float p_strength);
	float get_field_strength() const;
	
	void set_distortion_vector(const Vector3 &p_vector);
	Vector3 get_distortion_vector() const;
	
	// Consciousness accumulation
	void add_consciousness(float p_amount);
	void drain_consciousness(float p_amount);
	float get_consciousness_accumulation() const;
	void reset_consciousness_accumulation();
	
	// Object interaction
	void apply_distortion_to_object(Node3D* p_object);
	void remove_distortion_from_object(Node3D* p_object);
	void update_all_affected_objects();
	
	// Visual effects
	void set_visual_effects_enabled(bool p_enabled);
	bool get_visual_effects_enabled() const;
	
	void set_base_color_tint(const Color &p_color);
	Color get_base_color_tint() const;
	
	void set_fog_intensity_multiplier(float p_multiplier);
	float get_fog_intensity_multiplier() const;
	
	// Temporal effects
	void set_time_dilation_factor(float p_factor);
	float get_time_dilation_factor() const;
	
	void set_causes_memory_bleeds(bool p_enabled);
	bool get_causes_memory_bleeds() const;
	
	// Propagation
	void set_propagation_speed(float p_speed);
	float get_propagation_speed() const;
	
	void set_max_propagation_distance(float p_distance);
	float get_max_propagation_distance() const;
	
	void set_can_cascade(bool p_enabled);
	bool get_can_cascade() const;
	
	// Field interactions
	void merge_with_field(ConsciousnessField* p_other_field);
	void conflict_with_field(ConsciousnessField* p_other_field);
	bool can_merge_with_type(FieldType p_other_type) const;
	
	// Reality distortion calculations
	Vector3 calculate_distortion_at_position(const Vector3 &p_position) const;
	float calculate_intensity_at_position(const Vector3 &p_position) const;
	Color calculate_color_distortion_at_position(const Vector3 &p_position) const;
	
	// Cascade detection and prevention
	bool is_cascade_imminent() const;
	void trigger_cascade_event();
	void apply_bureaucratic_stabilization();
	
	// Architecture effects
	void apply_building_distortion(Node3D* p_building);
	void apply_depression_droop(Node3D* p_building);
	void apply_anxiety_maze(Node3D* p_building);
	void apply_anger_spikes(Node3D* p_building);
	void apply_memory_overlay(Node3D* p_building);
	void apply_bureaucratic_rigid(Node3D* p_building);
	void apply_creative_impossible(Node3D* p_building);
	void apply_fear_shadow(Node3D* p_building);
	void apply_hope_opening(Node3D* p_building);

private:
	void _on_body_entered(Node3D* p_body);
	void _on_body_exited(Node3D* p_body);
	void _update_consciousness_accumulation(double p_delta);
	void _process_field_effects(double p_delta);
	Transform3D _calculate_distorted_transform(Node3D* p_object, const Transform3D &p_original);
};