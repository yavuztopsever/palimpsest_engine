/**************************************************************************/
/*  palimpsest_theme_manager.h                                            */
/**************************************************************************/
/*                         This file is part of:                          */
/*                       PALIMPSEST ENGINE                                 */
/*                        https://palimpsest.org                          */
/**************************************************************************/
/* Copyright (c) 2025-present Palimpsest Engine contributors.             */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#pragma once

#include "core/object/ref_counted.h"
#include "scene/resources/theme.h"
#include "scene/resources/style_box_flat.h"

class PalimpsestThemeManager : public RefCounted {
	GDCLASS(PalimpsestThemeManager, RefCounted);

public:
	enum PalimpsestColorType {
		COLOR_BACKGROUND_PRIMARY,   // Dark gray base
		COLOR_BACKGROUND_SECONDARY, // Lighter gray panels
		COLOR_BACKGROUND_TERTIARY,  // Input fields, buttons
		COLOR_TEXT_PRIMARY,         // High contrast text
		COLOR_TEXT_SECONDARY,       // Lower contrast text
		COLOR_TEXT_DISABLED,        // Disabled state text
		COLOR_ACCENT_RED,           // Blood red for critical elements
		COLOR_ACCENT_AMBER,         // Muted amber for warnings
		COLOR_ACCENT_GREEN,         // Desaturated green for success
		COLOR_BORDER_NORMAL,        // Default borders
		COLOR_BORDER_FOCUS,         // Focused element borders
		COLOR_SHADOW,               // Drop shadows and depth
		COLOR_OVERLAY,              // Modal overlays
		COLOR_MAX
	};

private:
	static PalimpsestThemeManager *singleton;
	
	Ref<Theme> palimpsest_theme;
	HashMap<PalimpsestColorType, Color> color_palette;
	HashMap<String, Ref<StyleBoxFlat>> style_cache;
	
	bool theme_applied = false;
	float ui_scale = 1.0f;
	bool enable_distortion_effects = false;
	
	void _initialize_color_palette();
	void _create_base_styles();
	void _apply_button_styles();
	void _apply_panel_styles();
	void _apply_input_styles();
	void _apply_dock_styles();
	void _apply_menu_styles();
	void _apply_tab_styles();
	
	Ref<StyleBoxFlat> _create_expressionist_style(PalimpsestColorType p_bg_color, 
												  PalimpsestColorType p_border_color,
												  bool p_angular = true, 
												  bool p_add_shadow = true);
	void _add_angular_corners(Ref<StyleBoxFlat> p_style);
	void _add_subtle_grain_texture(Ref<StyleBoxFlat> p_style);

protected:
	static void _bind_methods();

public:
	static PalimpsestThemeManager *get_singleton() { return singleton; }
	
	void apply_palimpsest_theme();
	void revert_to_default_theme();
	
	Ref<Theme> get_palimpsest_theme() const { return palimpsest_theme; }
	Color get_color(PalimpsestColorType p_color_type) const;
	void set_color(PalimpsestColorType p_color_type, const Color &p_color);
	
	void set_ui_scale(float p_scale);
	float get_ui_scale() const { return ui_scale; }
	
	void set_distortion_effects_enabled(bool p_enabled);
	bool are_distortion_effects_enabled() const { return enable_distortion_effects; }
	
	// Style creation helpers for specific UI elements
	Ref<StyleBoxFlat> create_consciousness_field_style();
	Ref<StyleBoxFlat> create_dialogue_node_style();
	Ref<StyleBoxFlat> create_bureaucracy_form_style();
	Ref<StyleBoxFlat> create_thought_cabinet_style();
	
	// Theme customization for workspaces
	void apply_workspace_accent_color(const Color &p_accent);
	void apply_consciousness_field_styling(float p_distortion_intensity);
	
	PalimpsestThemeManager();
	~PalimpsestThemeManager();
};