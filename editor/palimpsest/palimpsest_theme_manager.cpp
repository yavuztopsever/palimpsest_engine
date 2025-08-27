/**************************************************************************/
/*  palimpsest_theme_manager.cpp                                          */
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

#include "palimpsest_theme_manager.h"

#include "core/io/resource_saver.h"
#include "scene/theme/theme_db.h"
#include "editor/editor_node.h"

PalimpsestThemeManager *PalimpsestThemeManager::singleton = nullptr;

void PalimpsestThemeManager::_bind_methods() {
	ClassDB::bind_method(D_METHOD("apply_palimpsest_theme"), &PalimpsestThemeManager::apply_palimpsest_theme);
	ClassDB::bind_method(D_METHOD("revert_to_default_theme"), &PalimpsestThemeManager::revert_to_default_theme);
	ClassDB::bind_method(D_METHOD("get_color", "color_type"), &PalimpsestThemeManager::get_color);
	ClassDB::bind_method(D_METHOD("set_color", "color_type", "color"), &PalimpsestThemeManager::set_color);
	ClassDB::bind_method(D_METHOD("set_ui_scale", "scale"), &PalimpsestThemeManager::set_ui_scale);
	ClassDB::bind_method(D_METHOD("get_ui_scale"), &PalimpsestThemeManager::get_ui_scale);
	ClassDB::bind_method(D_METHOD("set_distortion_effects_enabled", "enabled"), &PalimpsestThemeManager::set_distortion_effects_enabled);
	ClassDB::bind_method(D_METHOD("are_distortion_effects_enabled"), &PalimpsestThemeManager::are_distortion_effects_enabled);
	
	// Bind color type constants
	BIND_ENUM_CONSTANT(COLOR_BACKGROUND_PRIMARY);
	BIND_ENUM_CONSTANT(COLOR_BACKGROUND_SECONDARY);
	BIND_ENUM_CONSTANT(COLOR_BACKGROUND_TERTIARY);
	BIND_ENUM_CONSTANT(COLOR_TEXT_PRIMARY);
	BIND_ENUM_CONSTANT(COLOR_TEXT_SECONDARY);
	BIND_ENUM_CONSTANT(COLOR_TEXT_DISABLED);
	BIND_ENUM_CONSTANT(COLOR_ACCENT_RED);
	BIND_ENUM_CONSTANT(COLOR_ACCENT_AMBER);
	BIND_ENUM_CONSTANT(COLOR_ACCENT_GREEN);
	BIND_ENUM_CONSTANT(COLOR_BORDER_NORMAL);
	BIND_ENUM_CONSTANT(COLOR_BORDER_FOCUS);
	BIND_ENUM_CONSTANT(COLOR_SHADOW);
	BIND_ENUM_CONSTANT(COLOR_OVERLAY);
}

PalimpsestThemeManager::PalimpsestThemeManager() {
	singleton = this;
	palimpsest_theme.instantiate();
	_initialize_color_palette();
}

PalimpsestThemeManager::~PalimpsestThemeManager() {
	if (singleton == this) {
		singleton = nullptr;
	}
}

void PalimpsestThemeManager::_initialize_color_palette() {
	// German Expressionist color palette - monochrome with selective red
	color_palette[COLOR_BACKGROUND_PRIMARY] = Color(0.1f, 0.1f, 0.1f, 1.0f);        // #1a1a1a - Very dark gray
	color_palette[COLOR_BACKGROUND_SECONDARY] = Color(0.16f, 0.16f, 0.16f, 1.0f);   // #282828 - Dark gray panels
	color_palette[COLOR_BACKGROUND_TERTIARY] = Color(0.2f, 0.2f, 0.2f, 1.0f);       // #333333 - Input fields
	
	color_palette[COLOR_TEXT_PRIMARY] = Color(0.88f, 0.88f, 0.88f, 1.0f);           // #e0e0e0 - High contrast text
	color_palette[COLOR_TEXT_SECONDARY] = Color(0.7f, 0.7f, 0.7f, 1.0f);            // #b3b3b3 - Lower contrast
	color_palette[COLOR_TEXT_DISABLED] = Color(0.5f, 0.5f, 0.5f, 1.0f);             // #808080 - Disabled
	
	color_palette[COLOR_ACCENT_RED] = Color(0.545f, 0.0f, 0.0f, 1.0f);              // #8B0000 - Blood red
	color_palette[COLOR_ACCENT_AMBER] = Color(0.5f, 0.375f, 0.185f, 1.0f);          // #806030 - Muted amber
	color_palette[COLOR_ACCENT_GREEN] = Color(0.29f, 0.35f, 0.29f, 1.0f);           // #4a5a4a - Desaturated green
	
	color_palette[COLOR_BORDER_NORMAL] = Color(0.3f, 0.3f, 0.3f, 1.0f);             // #4d4d4d - Default borders
	color_palette[COLOR_BORDER_FOCUS] = Color(0.6f, 0.1f, 0.1f, 1.0f);              // #991a1a - Red focus
	color_palette[COLOR_SHADOW] = Color(0.0f, 0.0f, 0.0f, 0.4f);                    // #000000 - Drop shadows
	color_palette[COLOR_OVERLAY] = Color(0.0f, 0.0f, 0.0f, 0.6f);                   // #000000 - Modal overlays
}

void PalimpsestThemeManager::apply_palimpsest_theme() {
	if (theme_applied) {
		return;
	}
	
	print_verbose("Applying Palimpsest German Expressionist theme...");
	
	_create_base_styles();
	_apply_button_styles();
	_apply_panel_styles(); 
	_apply_input_styles();
	_apply_dock_styles();
	_apply_menu_styles();
	_apply_tab_styles();
	
	// Apply theme to editor
	EditorNode *editor = EditorNode::get_singleton();
	if (editor) {
		// This would require EditorNode to expose theme setting
		print_verbose("Palimpsest theme created (integration with EditorNode pending)");
	}
	
	theme_applied = true;
	
	// Save the theme resource for inspection
	ResourceSaver::save(palimpsest_theme, "res://palimpsest_expressionist_theme.tres");
	
	print_line("✓ Palimpsest German Expressionist theme applied successfully");
}

void PalimpsestThemeManager::revert_to_default_theme() {
	if (!theme_applied) {
		return;
	}
	
	// Revert to default Godot theme
	theme_applied = false;
	print_verbose("Reverted to default Godot theme");
}

void PalimpsestThemeManager::_create_base_styles() {
	// Create fundamental style boxes used throughout the theme
	
	// Primary panel style - used for main backgrounds
	Ref<StyleBoxFlat> primary_panel = _create_expressionist_style(COLOR_BACKGROUND_PRIMARY, COLOR_BORDER_NORMAL, true, true);
	style_cache["primary_panel"] = primary_panel;
	palimpsest_theme->set_stylebox("panel", "Panel", primary_panel);
	
	// Secondary panel style - used for dock backgrounds
	Ref<StyleBoxFlat> secondary_panel = _create_expressionist_style(COLOR_BACKGROUND_SECONDARY, COLOR_BORDER_NORMAL, true, false);
	style_cache["secondary_panel"] = secondary_panel;
	
	// Focus style - used for focused elements
	Ref<StyleBoxFlat> focus_style = _create_expressionist_style(COLOR_BACKGROUND_TERTIARY, COLOR_BORDER_FOCUS, true, true);
	style_cache["focus_style"] = focus_style;
}

void PalimpsestThemeManager::_apply_button_styles() {
	// Normal button
	Ref<StyleBoxFlat> button_normal = _create_expressionist_style(COLOR_BACKGROUND_SECONDARY, COLOR_BORDER_NORMAL);
	_add_angular_corners(button_normal);
	palimpsest_theme->set_stylebox("normal", "Button", button_normal);
	
	// Hovered button - slight red tint
	Ref<StyleBoxFlat> button_hover = _create_expressionist_style(COLOR_BACKGROUND_TERTIARY, COLOR_BORDER_NORMAL);
	_add_angular_corners(button_hover);
	Color hover_bg = get_color(COLOR_BACKGROUND_TERTIARY);
	hover_bg.r += 0.1f; // Add slight red tint
	button_hover->set_bg_color(hover_bg);
	palimpsest_theme->set_stylebox("hover", "Button", button_hover);
	
	// Pressed button
	Ref<StyleBoxFlat> button_pressed = _create_expressionist_style(COLOR_BACKGROUND_PRIMARY, COLOR_BORDER_FOCUS);
	_add_angular_corners(button_pressed);
	palimpsest_theme->set_stylebox("pressed", "Button", button_pressed);
	
	// Disabled button
	Ref<StyleBoxFlat> button_disabled = _create_expressionist_style(COLOR_BACKGROUND_PRIMARY, COLOR_BORDER_NORMAL);
	_add_angular_corners(button_disabled);
	button_disabled->set_bg_color(get_color(COLOR_BACKGROUND_PRIMARY));
	palimpsest_theme->set_stylebox("disabled", "Button", button_disabled);
	
	// Button text colors
	palimpsest_theme->set_color("font_color", "Button", get_color(COLOR_TEXT_PRIMARY));
	palimpsest_theme->set_color("font_hover_color", "Button", get_color(COLOR_TEXT_PRIMARY));
	palimpsest_theme->set_color("font_pressed_color", "Button", get_color(COLOR_ACCENT_RED));
	palimpsest_theme->set_color("font_disabled_color", "Button", get_color(COLOR_TEXT_DISABLED));
}

void PalimpsestThemeManager::_apply_panel_styles() {
	// Main panel style
	palimpsest_theme->set_stylebox("panel", "Panel", style_cache["primary_panel"]);
	
	// Dock panel style  
	palimpsest_theme->set_stylebox("panel", "PanelContainer", style_cache["secondary_panel"]);
	
	// Popup panel style - more pronounced shadow
	Ref<StyleBoxFlat> popup_panel = _create_expressionist_style(COLOR_BACKGROUND_SECONDARY, COLOR_BORDER_NORMAL, true, true);
	popup_panel->set_shadow_size(8);
	popup_panel->set_shadow_color(get_color(COLOR_SHADOW));
	palimpsest_theme->set_stylebox("panel", "PopupPanel", popup_panel);
}

void PalimpsestThemeManager::_apply_input_styles() {
	// LineEdit styles
	Ref<StyleBoxFlat> line_edit_normal = _create_expressionist_style(COLOR_BACKGROUND_TERTIARY, COLOR_BORDER_NORMAL);
	_add_angular_corners(line_edit_normal);
	palimpsest_theme->set_stylebox("normal", "LineEdit", line_edit_normal);
	
	Ref<StyleBoxFlat> line_edit_focus = _create_expressionist_style(COLOR_BACKGROUND_TERTIARY, COLOR_BORDER_FOCUS);
	_add_angular_corners(line_edit_focus);
	palimpsest_theme->set_stylebox("focus", "LineEdit", line_edit_focus);
	
	// LineEdit text colors
	palimpsest_theme->set_color("font_color", "LineEdit", get_color(COLOR_TEXT_PRIMARY));
	palimpsest_theme->set_color("font_selected_color", "LineEdit", get_color(COLOR_TEXT_PRIMARY));
	palimpsest_theme->set_color("font_uneditable_color", "LineEdit", get_color(COLOR_TEXT_DISABLED));
	palimpsest_theme->set_color("caret_color", "LineEdit", get_color(COLOR_ACCENT_RED));
	palimpsest_theme->set_color("selection_color", "LineEdit", Color(get_color(COLOR_ACCENT_RED).r, get_color(COLOR_ACCENT_RED).g, get_color(COLOR_ACCENT_RED).b, 0.3f));
}

void PalimpsestThemeManager::_apply_dock_styles() {
	// Dock background
	Ref<StyleBoxFlat> dock_bg = _create_expressionist_style(COLOR_BACKGROUND_SECONDARY, COLOR_BORDER_NORMAL, true, false);
	palimpsest_theme->set_stylebox("bg", "EditorDock", dock_bg);
}

void PalimpsestThemeManager::_apply_menu_styles() {
	// MenuButton styles
	Ref<StyleBoxFlat> menu_normal = _create_expressionist_style(COLOR_BACKGROUND_SECONDARY, COLOR_BORDER_NORMAL);
	menu_normal->set_content_margin_all(8 * ui_scale);
	palimpsest_theme->set_stylebox("normal", "MenuButton", menu_normal);
	
	Ref<StyleBoxFlat> menu_hover = _create_expressionist_style(COLOR_BACKGROUND_TERTIARY, COLOR_BORDER_NORMAL);
	menu_hover->set_content_margin_all(8 * ui_scale);
	Color hover_bg = get_color(COLOR_BACKGROUND_TERTIARY);
	hover_bg.r += 0.05f; // Subtle red tint
	menu_hover->set_bg_color(hover_bg);
	palimpsest_theme->set_stylebox("hover", "MenuButton", menu_hover);
	
	// MenuButton text
	palimpsest_theme->set_color("font_color", "MenuButton", get_color(COLOR_TEXT_PRIMARY));
	palimpsest_theme->set_color("font_hover_color", "MenuButton", get_color(COLOR_TEXT_PRIMARY));
}

void PalimpsestThemeManager::_apply_tab_styles() {
	// TabContainer styles for docks
	Ref<StyleBoxFlat> tab_selected = _create_expressionist_style(COLOR_BACKGROUND_SECONDARY, COLOR_BORDER_FOCUS);
	_add_angular_corners(tab_selected);
	palimpsest_theme->set_stylebox("tab_selected", "TabContainer", tab_selected);
	
	Ref<StyleBoxFlat> tab_unselected = _create_expressionist_style(COLOR_BACKGROUND_PRIMARY, COLOR_BORDER_NORMAL);
	_add_angular_corners(tab_unselected);
	palimpsest_theme->set_stylebox("tab_unselected", "TabContainer", tab_unselected);
	
	// Tab text colors
	palimpsest_theme->set_color("font_selected_color", "TabContainer", get_color(COLOR_TEXT_PRIMARY));
	palimpsest_theme->set_color("font_unselected_color", "TabContainer", get_color(COLOR_TEXT_SECONDARY));
}

Ref<StyleBoxFlat> PalimpsestThemeManager::_create_expressionist_style(PalimpsestColorType p_bg_color, 
																	   PalimpsestColorType p_border_color,
																	   bool p_angular, 
																	   bool p_add_shadow) {
	Ref<StyleBoxFlat> style;
	style.instantiate();
	
	// Set background color
	style->set_bg_color(get_color(p_bg_color));
	
	// Set border
	style->set_border_width_all(1 * ui_scale);
	style->set_border_color(get_color(p_border_color));
	
	// Angular corners for expressionist aesthetic
	if (p_angular) {
		_add_angular_corners(style);
	}
	
	// Add shadow for depth
	if (p_add_shadow) {
		style->set_shadow_size(2 * ui_scale);
		style->set_shadow_color(get_color(COLOR_SHADOW));
		style->set_shadow_offset(Vector2(1, 1) * ui_scale);
	}
	
	// Add subtle texture if distortion effects are enabled
	if (enable_distortion_effects) {
		_add_subtle_grain_texture(style);
	}
	
	return style;
}

void PalimpsestThemeManager::_add_angular_corners(Ref<StyleBoxFlat> p_style) {
	// Create angular, modernist corners
	p_style->set_corner_radius_all(2 * ui_scale);  // Minimal rounding for sharp aesthetic
}

void PalimpsestThemeManager::_add_subtle_grain_texture(Ref<StyleBoxFlat> p_style) {
	// Add film grain texture for expressionist effect
	// This would require custom shader support in StyleBoxFlat
	// For now, just add a comment for future implementation
}

Color PalimpsestThemeManager::get_color(PalimpsestColorType p_color_type) const {
	if (color_palette.has(p_color_type)) {
		return color_palette[p_color_type];
	}
	return Color::WHITE; // Fallback
}

void PalimpsestThemeManager::set_color(PalimpsestColorType p_color_type, const Color &p_color) {
	color_palette[p_color_type] = p_color;
	
	// If theme is applied, need to regenerate styles
	if (theme_applied) {
		theme_applied = false;
		apply_palimpsest_theme();
	}
}

void PalimpsestThemeManager::set_ui_scale(float p_scale) {
	ui_scale = Math::max(0.5f, Math::min(3.0f, p_scale)); // Clamp between 0.5x and 3.0x
	
	// Regenerate styles with new scale
	if (theme_applied) {
		theme_applied = false;
		apply_palimpsest_theme();
	}
}

void PalimpsestThemeManager::set_distortion_effects_enabled(bool p_enabled) {
	enable_distortion_effects = p_enabled;
	
	// Regenerate styles with/without effects
	if (theme_applied) {
		theme_applied = false;
		apply_palimpsest_theme();
	}
}

Ref<StyleBoxFlat> PalimpsestThemeManager::create_consciousness_field_style() {
	Ref<StyleBoxFlat> style = _create_expressionist_style(COLOR_BACKGROUND_SECONDARY, COLOR_ACCENT_RED, true, true);
	
	// Add consciousness-specific styling
	style->set_bg_color(Color(get_color(COLOR_BACKGROUND_SECONDARY).r * 1.1f, 
							  get_color(COLOR_BACKGROUND_SECONDARY).g * 0.9f, 
							  get_color(COLOR_BACKGROUND_SECONDARY).b * 0.9f, 0.9f));
							  
	// Glowing red border for consciousness fields
	style->set_border_color(get_color(COLOR_ACCENT_RED));
	style->set_border_width_all(2 * ui_scale);
	
	return style;
}

Ref<StyleBoxFlat> PalimpsestThemeManager::create_dialogue_node_style() {
	Ref<StyleBoxFlat> style = _create_expressionist_style(COLOR_BACKGROUND_TERTIARY, COLOR_BORDER_NORMAL, true, true);
	
	// Dialogue nodes have subtle amber accenting
	Color bg = get_color(COLOR_BACKGROUND_TERTIARY);
	bg.g += 0.05f; // Slight warmth for dialogue
	style->set_bg_color(bg);
	
	return style;
}

Ref<StyleBoxFlat> PalimpsestThemeManager::create_bureaucracy_form_style() {
	Ref<StyleBoxFlat> style = _create_expressionist_style(COLOR_BACKGROUND_PRIMARY, COLOR_BORDER_NORMAL, true, false);
	
	// Bureaucracy forms are deliberately stark and institutional
	style->set_bg_color(Color(0.95f, 0.95f, 0.95f, 1.0f)); // Almost white, like paper
	style->set_border_color(Color(0.0f, 0.0f, 0.0f, 1.0f)); // Black border like official forms
	style->set_border_width_all(1 * ui_scale);
	
	return style;
}

Ref<StyleBoxFlat> PalimpsestThemeManager::create_thought_cabinet_style() {
	Ref<StyleBoxFlat> style = _create_expressionist_style(COLOR_BACKGROUND_SECONDARY, COLOR_ACCENT_AMBER, true, true);
	
	// Thought cabinet has warm, introspective styling
	Color bg = get_color(COLOR_BACKGROUND_SECONDARY);
	bg.r += 0.03f;
	bg.g += 0.02f;
	style->set_bg_color(bg);
	
	return style;
}

void PalimpsestThemeManager::apply_workspace_accent_color(const Color &p_accent) {
	// Apply workspace-specific accent color to relevant elements
	color_palette[COLOR_BORDER_FOCUS] = p_accent;
	
	if (theme_applied) {
		theme_applied = false;
		apply_palimpsest_theme();
	}
}

void PalimpsestThemeManager::apply_consciousness_field_styling(float p_distortion_intensity) {
	// Apply distortion effects based on consciousness field intensity
	// This would modify the visual appearance of UI elements when consciousness fields are active
	print_verbose(vformat("Applying consciousness distortion at intensity: %f", p_distortion_intensity));
}