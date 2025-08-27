/**************************************************************************/
/*  palette_enforcer.h                                                    */
/**************************************************************************/
/*                         This file is part of:                          */
/*                        PALIMPSEST ENGINE                               */
/**************************************************************************/

#pragma once

#include "scene/main/node.h"
#include "core/math/color.h"
#include "core/templates/vector.h"
#include "scene/resources/material.h"

class PaletteEnforcer : public Node {
	GDCLASS(PaletteEnforcer, Node);

public:
	enum PaletteMode {
		MODE_STRICT_MONOCHROME,    // Pure black/white/gray only
		MODE_EXPRESSIONIST,        // Monochrome + red accents
		MODE_BUREAUCRATIC,         // Desaturated with blue-gray tints
		MODE_MEMORY,               // Sepia tones for flashbacks
		MODE_CUSTOM                // User-defined palette
	};

	enum RedAccentTrigger {
		TRIGGER_ANGER,            // Red appears during anger consciousness
		TRIGGER_VIOLENCE,         // Red for violent/aggressive elements
		TRIGGER_PASSION,          // Red for emotional intensity
		TRIGGER_BLOOD,            // Red for blood/injury
		TRIGGER_DANGER,           // Red for warnings/threats
		TRIGGER_ALWAYS            // Red always allowed
	};

private:
	PaletteMode current_mode = MODE_EXPRESSIONIST;
	RedAccentTrigger red_trigger = TRIGGER_ANGER;
	
	// Palette colors
	Color pure_black = Color(0.0f, 0.0f, 0.0f, 1.0f);
	Color pure_white = Color(1.0f, 1.0f, 1.0f, 1.0f);
	Color dark_gray = Color(0.2f, 0.2f, 0.2f, 1.0f);
	Color medium_gray = Color(0.5f, 0.5f, 0.5f, 1.0f);
	Color light_gray = Color(0.8f, 0.8f, 0.8f, 1.0f);
	Color expressionist_red = Color(0.7f, 0.1f, 0.1f, 1.0f);
	
	// Enforcement settings
	bool enforce_on_materials = true;
	bool enforce_on_textures = true;
	bool enforce_on_lights = true;
	bool enforce_on_ui = true;
	float enforcement_strength = 1.0f;
	
	// Red accent control
	float red_accent_threshold = 0.3f;
	bool red_accents_enabled = true;
	float current_consciousness_intensity = 0.0f;

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	PaletteEnforcer();
	~PaletteEnforcer();

	// Core palette enforcement
	Color enforce_color(const Color &p_input_color) const;
	void enforce_material(Material* p_material);
	void enforce_all_materials_in_scene(Node* p_root = nullptr);
	
	// Mode management
	void set_palette_mode(PaletteMode p_mode);
	PaletteMode get_palette_mode() const;
	
	void set_red_accent_trigger(RedAccentTrigger p_trigger);
	RedAccentTrigger get_red_accent_trigger() const;
	
	// Base palette colors
	void set_pure_black(const Color &p_color);
	Color get_pure_black() const;
	
	void set_pure_white(const Color &p_color);
	Color get_pure_white() const;
	
	void set_expressionist_red(const Color &p_color);
	Color get_expressionist_red() const;
	
	// Enforcement settings
	void set_enforce_on_materials(bool p_enabled);
	bool get_enforce_on_materials() const;
	
	void set_enforcement_strength(float p_strength);
	float get_enforcement_strength() const;
	
	// Red accent control
	void set_red_accents_enabled(bool p_enabled);
	bool get_red_accents_enabled() const;
	
	void set_consciousness_intensity(float p_intensity);
	float get_consciousness_intensity() const;
	
	// Utility functions
	bool is_color_compliant(const Color &p_color) const;
	Color get_nearest_palette_color(const Color &p_color) const;
	
	// Presets
	void apply_preset_expressionist();
	void apply_preset_bureaucratic();

private:
	// Color conversion helpers
	Color convert_to_monochrome(const Color &p_color, bool p_preserve_luminance = true) const;
	Color apply_expressionist_rules(const Color &p_color) const;
	Color apply_bureaucratic_rules(const Color &p_color) const;
	
	// Red accent logic
	bool should_preserve_red_accent(const Color &p_color) const;
	bool is_red_accent_allowed() const;
};
