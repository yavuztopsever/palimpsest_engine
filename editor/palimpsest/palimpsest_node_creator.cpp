/**************************************************************************/
/*  palimpsest_node_creator.cpp                                           */
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

#include "palimpsest_node_creator.h"
#include "palimpsest_workspace_manager.h"

#include "scene/gui/v_box_container.h"
#include "scene/gui/h_box_container.h"
#include "scene/gui/separator.h"

void PalimpsestNodeCreator::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_workspace_manager", "manager"), &PalimpsestNodeCreator::set_workspace_manager);
	ClassDB::bind_method(D_METHOD("refresh_for_current_workspace"), &PalimpsestNodeCreator::refresh_for_current_workspace);
	ClassDB::bind_method(D_METHOD("get_selected_node_class"), &PalimpsestNodeCreator::get_selected_node_class);
	ClassDB::bind_method(D_METHOD("set_category_filter", "category"), &PalimpsestNodeCreator::set_category_filter);
	
	// Private method bindings for signal connections
	ClassDB::bind_method(D_METHOD("_on_search_text_changed", "text"), &PalimpsestNodeCreator::_on_search_text_changed);
	ClassDB::bind_method(D_METHOD("_on_category_selected", "index"), &PalimpsestNodeCreator::_on_category_selected);
	ClassDB::bind_method(D_METHOD("_on_item_selected"), &PalimpsestNodeCreator::_on_item_selected);
	ClassDB::bind_method(D_METHOD("_on_item_activated"), &PalimpsestNodeCreator::_on_item_activated);
	ClassDB::bind_method(D_METHOD("_on_create_pressed"), &PalimpsestNodeCreator::_on_create_pressed);
	
	ADD_SIGNAL(MethodInfo("selected_node_class_changed", PropertyInfo(Variant::STRING, "class_name")));
	
	// Bind enum constants
	BIND_ENUM_CONSTANT(CATEGORY_ALL);
	BIND_ENUM_CONSTANT(CATEGORY_NARRATIVE);
	BIND_ENUM_CONSTANT(CATEGORY_CONSCIOUSNESS);
	BIND_ENUM_CONSTANT(CATEGORY_BUREAUCRACY);
	BIND_ENUM_CONSTANT(CATEGORY_WORLD);
	BIND_ENUM_CONSTANT(CATEGORY_ESSENTIAL);
	BIND_ENUM_CONSTANT(CATEGORY_CUSTOM);
}

PalimpsestNodeCreator::PalimpsestNodeCreator() {
	set_title("Create Palimpsest Node");
	set_min_size(Size2(600, 500));
	
	// Main container
	VBoxContainer *main_vbox = memnew(VBoxContainer);
	add_child(main_vbox);
	
	// Search and filter row
	HBoxContainer *filter_hbox = memnew(HBoxContainer);
	main_vbox->add_child(filter_hbox);
	
	// Search box
	search_edit = memnew(LineEdit);
	search_edit->set_placeholder("Search nodes...");
	search_edit->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	search_edit->connect("text_changed", callable_mp(this, &PalimpsestNodeCreator::_on_search_text_changed));
	filter_hbox->add_child(search_edit);
	
	// Category filter
	category_filter = memnew(OptionButton);
	category_filter->add_item("All Nodes", CATEGORY_ALL);
	category_filter->add_item("Narrative", CATEGORY_NARRATIVE);
	category_filter->add_item("Consciousness", CATEGORY_CONSCIOUSNESS);
	category_filter->add_item("Bureaucracy", CATEGORY_BUREAUCRACY);
	category_filter->add_item("World Building", CATEGORY_WORLD);
	category_filter->add_item("Essential", CATEGORY_ESSENTIAL);
	category_filter->add_item("Custom", CATEGORY_CUSTOM);
	category_filter->connect("item_selected", callable_mp(this, &PalimpsestNodeCreator::_on_category_selected));
	filter_hbox->add_child(category_filter);
	
	// Separator
	main_vbox->add_child(memnew(HSeparator));
	
	// Node tree
	node_tree = memnew(Tree);
	node_tree->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	node_tree->set_hide_root(true);
	node_tree->connect("item_selected", callable_mp(this, &PalimpsestNodeCreator::_on_item_selected));
	node_tree->connect("item_activated", callable_mp(this, &PalimpsestNodeCreator::_on_item_activated));
	main_vbox->add_child(node_tree);
	
	// Description area
	description_label = memnew(Label);
	description_label->set_text("Select a node type to see its description");
	description_label->set_autowrap_mode(TextServer::AUTOWRAP_WORD_SMART);
	description_label->set_custom_minimum_size(Size2(0, 60));
	description_label->set_vertical_alignment(VERTICAL_ALIGNMENT_TOP);
	main_vbox->add_child(description_label);
	
	// Separator
	main_vbox->add_child(memnew(HSeparator));
	
	// Create button
	create_button = memnew(Button);
	create_button->set_text("Create Node");
	create_button->set_disabled(true);
	create_button->connect("pressed", callable_mp(this, &PalimpsestNodeCreator::_on_create_pressed));
	main_vbox->add_child(create_button);
	
	// Initialize node database
	_initialize_node_database();
	
	// Build initial tree
	_rebuild_tree();
}

void PalimpsestNodeCreator::_notification(int p_what) {
	AcceptDialog::_notification(p_what);
	
	switch (p_what) {
		case NOTIFICATION_READY: {
			// Apply Palimpsest theme if available
		} break;
		
		case NOTIFICATION_VISIBILITY_CHANGED: {
			if (is_visible()) {
				refresh_for_current_workspace();
				search_edit->grab_focus();
			}
		} break;
	}
}

void PalimpsestNodeCreator::_initialize_node_database() {
	node_database.clear();
	
	_add_palimpsest_nodes();
	_add_essential_godot_nodes();
	
	print_verbose(vformat("Initialized Palimpsest node database with %d nodes", node_database.size()));
}

void PalimpsestNodeCreator::_add_palimpsest_nodes() {
	// Narrative nodes
	NodeInfo dialogue_node;
	dialogue_node.name = "Dialogue Node";
	dialogue_node.class_name = "DialogueNode";
	dialogue_node.description = "A node for handling interactive dialogue with consciousness effects and skill checks";
	dialogue_node.category = CATEGORY_NARRATIVE;
	dialogue_node.workspace_visibility.append("narrative");
	dialogue_node.workspace_visibility.append("testing");
	dialogue_node.is_palimpsest_node = true;
	node_database.push_back(dialogue_node);
	
	NodeInfo quest_node;
	quest_node.name = "Quest Node";
	quest_node.class_name = "QuestNode";
	quest_node.description = "Manages quest states and progression with bureaucratic complexity tracking";
	quest_node.category = CATEGORY_NARRATIVE;
	quest_node.workspace_visibility.append("narrative");
	quest_node.workspace_visibility.append("testing");
	quest_node.is_palimpsest_node = true;
	node_database.push_back(quest_node);
	
	NodeInfo thought_node;
	thought_node.name = "Thought Node";
	thought_node.class_name = "ThoughtNode";
	thought_node.description = "Represents internalized thoughts and skills in the thought cabinet system";
	thought_node.category = CATEGORY_NARRATIVE;
	thought_node.workspace_visibility.append("narrative");
	thought_node.workspace_visibility.append("consciousness");
	thought_node.workspace_visibility.append("testing");
	thought_node.is_palimpsest_node = true;
	node_database.push_back(thought_node);
	
	// Consciousness nodes
	NodeInfo consciousness_field;
	consciousness_field.name = "Consciousness Field";
	consciousness_field.class_name = "ConsciousnessField";
	consciousness_field.description = "An area that distorts reality based on emotional and mental states";
	consciousness_field.category = CATEGORY_CONSCIOUSNESS;
	consciousness_field.workspace_visibility.append("consciousness");
	consciousness_field.workspace_visibility.append("world");
	consciousness_field.workspace_visibility.append("testing");
	consciousness_field.is_palimpsest_node = true;
	node_database.push_back(consciousness_field);
	
	NodeInfo reality_distortion;
	reality_distortion.name = "Reality Distortion";
	reality_distortion.class_name = "RealityDistortion";
	reality_distortion.description = "Applies visual and architectural distortions based on consciousness fields";
	reality_distortion.category = CATEGORY_CONSCIOUSNESS;
	reality_distortion.workspace_visibility.append("consciousness");
	reality_distortion.workspace_visibility.append("testing");
	reality_distortion.is_palimpsest_node = true;
	node_database.push_back(reality_distortion);
	
	NodeInfo memory_fragment;
	memory_fragment.name = "Memory Fragment";
	memory_fragment.class_name = "MemoryFragment";
	memory_fragment.description = "Stores fragmented memories that can be triggered by consciousness fields";
	memory_fragment.category = CATEGORY_CONSCIOUSNESS;
	memory_fragment.workspace_visibility.append("consciousness");
	memory_fragment.workspace_visibility.append("narrative");
	memory_fragment.workspace_visibility.append("testing");
	memory_fragment.is_palimpsest_node = true;
	node_database.push_back(memory_fragment);
	
	// Bureaucracy nodes
	NodeInfo form_node;
	form_node.name = "Form Node";
	form_node.class_name = "FormNode";
	form_node.description = "Interactive bureaucratic form with validation and stamping mechanics";
	form_node.category = CATEGORY_BUREAUCRACY;
	form_node.workspace_visibility.append("bureaucracy");
	form_node.workspace_visibility.append("testing");
	form_node.is_palimpsest_node = true;
	node_database.push_back(form_node);
	
	NodeInfo stamp_validator;
	stamp_validator.name = "Stamp Validator";
	stamp_validator.class_name = "StampValidator";
	stamp_validator.description = "Validates official stamps and signatures on bureaucratic documents";
	stamp_validator.category = CATEGORY_BUREAUCRACY;
	stamp_validator.workspace_visibility.append("bureaucracy");
	stamp_validator.workspace_visibility.append("testing");
	stamp_validator.is_palimpsest_node = true;
	node_database.push_back(stamp_validator);
	
	NodeInfo paperwork_container;
	paperwork_container.name = "Paperwork Container";
	paperwork_container.class_name = "PaperworkContainer";
	paperwork_container.description = "Organizes and manages collections of bureaucratic documents";
	paperwork_container.category = CATEGORY_BUREAUCRACY;
	paperwork_container.workspace_visibility.append("bureaucracy");
	paperwork_container.workspace_visibility.append("testing");
	paperwork_container.is_palimpsest_node = true;
	node_database.push_back(paperwork_container);
	
	// World building nodes
	NodeInfo wfc_generator;
	wfc_generator.name = "WFC Generator";
	wfc_generator.class_name = "WFCGenerator";
	wfc_generator.description = "Wave Function Collapse generator for procedural brutalist architecture";
	wfc_generator.category = CATEGORY_WORLD;
	wfc_generator.workspace_visibility.append("world");
	wfc_generator.workspace_visibility.append("testing");
	wfc_generator.is_palimpsest_node = true;
	node_database.push_back(wfc_generator);
	
	NodeInfo brutalist_tile;
	brutalist_tile.name = "Brutalist Tile";
	brutalist_tile.class_name = "BrutalistTile";
	brutalist_tile.description = "Architectural tile for creating brutalist concrete structures";
	brutalist_tile.category = CATEGORY_WORLD;
	brutalist_tile.workspace_visibility.append("world");
	brutalist_tile.workspace_visibility.append("testing");
	brutalist_tile.is_palimpsest_node = true;
	node_database.push_back(brutalist_tile);
	
	NodeInfo palette_enforcer;
	palette_enforcer.name = "Palette Enforcer";
	palette_enforcer.class_name = "PaletteEnforcer";
	palette_enforcer.description = "Enforces German Expressionist color palette with selective red accents";
	palette_enforcer.category = CATEGORY_WORLD;
	palette_enforcer.workspace_visibility.append("world");
	palette_enforcer.workspace_visibility.append("testing");
	palette_enforcer.is_palimpsest_node = true;
	node_database.push_back(palette_enforcer);
}

void PalimpsestNodeCreator::_add_essential_godot_nodes() {
	// Essential base nodes that are commonly used in CRPG development
	
	// Base nodes
	NodeInfo node;
	node.name = "Node";
	node.class_name = "Node";
	node.description = "The base class for all scene tree nodes";
	node.category = CATEGORY_ESSENTIAL;
	node.workspace_visibility.append("all");
	node_database.push_back(node);
	
	// 2D nodes for UI
	NodeInfo node2d;
	node2d.name = "Node2D";
	node2d.class_name = "Node2D";
	node2d.description = "Base class for 2D positioning and transformations";
	node2d.category = CATEGORY_ESSENTIAL;
	node2d.workspace_visibility.append("narrative");
	node2d.workspace_visibility.append("bureaucracy");
	node_database.push_back(node2d);
	
	// 3D nodes for isometric view
	NodeInfo node3d;
	node3d.name = "Node3D";
	node3d.class_name = "Node3D";
	node3d.description = "Base class for 3D positioning and transformations";
	node3d.category = CATEGORY_ESSENTIAL;
	node3d.workspace_visibility.append("consciousness");
	node3d.workspace_visibility.append("world");
	node_database.push_back(node3d);
	
	// UI controls
	NodeInfo control;
	control.name = "Control";
	control.class_name = "Control";
	control.description = "Base class for all user interface elements";
	control.category = CATEGORY_ESSENTIAL;
	control.workspace_visibility.append("narrative");
	control.workspace_visibility.append("bureaucracy");
	node_database.push_back(control);
	
	NodeInfo label;
	label.name = "Label";
	label.class_name = "Label";
	label.description = "Displays text on the screen";
	label.category = CATEGORY_ESSENTIAL;
	label.workspace_visibility.append("narrative");
	label.workspace_visibility.append("bureaucracy");
	node_database.push_back(label);
	
	NodeInfo button;
	button.name = "Button";
	button.class_name = "Button";
	button.description = "Interactive button control";
	button.category = CATEGORY_ESSENTIAL;
	button.workspace_visibility.append("narrative");
	button.workspace_visibility.append("bureaucracy");
	node_database.push_back(button);
	
	// Physics bodies for interaction
	NodeInfo static_body3d;
	static_body3d.name = "StaticBody3D";
	static_body3d.class_name = "StaticBody3D";
	static_body3d.description = "Physics body that cannot be moved";
	static_body3d.category = CATEGORY_ESSENTIAL;
	static_body3d.workspace_visibility.append("world");
	static_body3d.workspace_visibility.append("consciousness");
	node_database.push_back(static_body3d);
	
	NodeInfo character_body3d;
	character_body3d.name = "CharacterBody3D";
	character_body3d.class_name = "CharacterBody3D";
	character_body3d.description = "Physics body for character movement";
	character_body3d.category = CATEGORY_ESSENTIAL;
	character_body3d.workspace_visibility.append("world");
	node_database.push_back(character_body3d);
}

void PalimpsestNodeCreator::_rebuild_tree() {
	node_tree->clear();
	category_items.clear();
	
	root_item = node_tree->create_item();
	root_item->set_text(0, "Nodes");
	
	// Create category items
	_create_category_item(CATEGORY_NARRATIVE, "📖 Narrative");
	_create_category_item(CATEGORY_CONSCIOUSNESS, "🧠 Consciousness");
	_create_category_item(CATEGORY_BUREAUCRACY, "📋 Bureaucracy");
	_create_category_item(CATEGORY_WORLD, "🌍 World Building");
	_create_category_item(CATEGORY_ESSENTIAL, "⚙️ Essential");
	
	// Add nodes to appropriate categories
	for (const NodeInfo &node_info : node_database) {
		if (_node_matches_filter(node_info) && _node_visible_in_current_workspace(node_info)) {
			_add_node_to_tree(node_info);
		}
	}
	
	// Expand all categories by default
	for (const KeyValue<String, TreeItem*> &kv : category_items) {
		kv.value->set_collapsed(false);
	}
}

void PalimpsestNodeCreator::_create_category_item(NodeCategory p_category, const String &p_name) {
	if (current_category != CATEGORY_ALL && current_category != p_category) {
		return; // Skip this category if filtering
	}
	
	TreeItem *category_item = node_tree->create_item(root_item);
	category_item->set_text(0, p_name);
	category_item->set_selectable(0, false);
	
	// Style category items differently
	category_item->set_custom_bg_color(0, Color(0.2f, 0.2f, 0.2f, 1.0f));
	
	String category_key = String::num_int64(p_category);
	category_items[category_key] = category_item;
}

void PalimpsestNodeCreator::_add_node_to_tree(const NodeInfo &p_node_info) {
	String category_key = String::num_int64(p_node_info.category);
	
	if (!category_items.has(category_key)) {
		return; // Category not visible
	}
	
	TreeItem *category_item = category_items[category_key];
	TreeItem *node_item = node_tree->create_item(category_item);
	
	// Set node display name and class
	String display_name = p_node_info.name;
	if (p_node_info.is_palimpsest_node) {
		display_name = "✨ " + display_name; // Mark Palimpsest nodes with sparkle
	}
	
	node_item->set_text(0, display_name);
	node_item->set_metadata(0, p_node_info.class_name);
	node_item->set_tooltip_text(0, p_node_info.description);
	
	// Style Palimpsest nodes differently
	if (p_node_info.is_palimpsest_node) {
		node_item->set_custom_color(0, Color(1.0f, 0.9f, 0.7f, 1.0f)); // Warm tint
	}
}

void PalimpsestNodeCreator::_on_search_text_changed(const String &p_text) {
	current_search_filter = p_text;
	_rebuild_tree();
}

void PalimpsestNodeCreator::_on_category_selected(int p_index) {
	current_category = (NodeCategory)p_index;
	_rebuild_tree();
}

void PalimpsestNodeCreator::_on_item_selected() {
	TreeItem *selected = node_tree->get_selected();
	if (!selected || selected->get_metadata(0).is_null()) {
		selected_node_class = "";
		description_label->set_text("Select a node type to see its description");
		create_button->set_disabled(true);
		return;
	}
	
	selected_node_class = selected->get_metadata(0);
	create_button->set_disabled(false);
	
	// Find node info for description
	for (const NodeInfo &node_info : node_database) {
		if (node_info.class_name == selected_node_class) {
			description_label->set_text(node_info.description);
			break;
		}
	}
	
	emit_signal("selected_node_class_changed", selected_node_class);
}

void PalimpsestNodeCreator::_on_item_activated() {
	if (!selected_node_class.is_empty()) {
		_on_create_pressed();
	}
}

void PalimpsestNodeCreator::_on_create_pressed() {
	if (selected_node_class.is_empty()) {
		return;
	}
	
	// Signal that a node should be created
	// This would be handled by the editor to actually instantiate the node
	print_line(vformat("Creating node of type: %s", selected_node_class));
	
	hide();
}

bool PalimpsestNodeCreator::_node_matches_filter(const NodeInfo &p_node_info) const {
	if (current_search_filter.is_empty()) {
		return true;
	}
	
	String filter_lower = current_search_filter.to_lower();
	return p_node_info.name.to_lower().contains(filter_lower) ||
		   p_node_info.class_name.to_lower().contains(filter_lower) ||
		   p_node_info.description.to_lower().contains(filter_lower);
}

bool PalimpsestNodeCreator::_node_visible_in_current_workspace(const NodeInfo &p_node_info) const {
	if (!workspace_manager) {
		return true; // Show all if no workspace manager
	}
	
	// Check if node is visible in current workspace
	for (const String &workspace_name : p_node_info.workspace_visibility) {
		if (workspace_name == "all") {
			return true;
		}
		
		// Map workspace manager workspace to visibility string
		auto current_workspace = workspace_manager->get_current_workspace();
		String current_workspace_name;
		
		switch (current_workspace) {
			case PalimpsestWorkspaceManager::WORKSPACE_NARRATIVE:
				current_workspace_name = "narrative";
				break;
			case PalimpsestWorkspaceManager::WORKSPACE_CONSCIOUSNESS:
				current_workspace_name = "consciousness";
				break;
			case PalimpsestWorkspaceManager::WORKSPACE_BUREAUCRACY:
				current_workspace_name = "bureaucracy";
				break;
			case PalimpsestWorkspaceManager::WORKSPACE_WORLD_BUILDING:
				current_workspace_name = "world";
				break;
			case PalimpsestWorkspaceManager::WORKSPACE_TESTING:
				current_workspace_name = "testing";
				break;
			default:
				return true;
		}
		
		if (workspace_name == current_workspace_name) {
			return true;
		}
	}
	
	return false;
}

void PalimpsestNodeCreator::set_workspace_manager(PalimpsestWorkspaceManager *p_manager) {
	workspace_manager = p_manager;
}

void PalimpsestNodeCreator::refresh_for_current_workspace() {
	_filter_nodes_by_workspace();
	_rebuild_tree();
}

void PalimpsestNodeCreator::_filter_nodes_by_workspace() {
	// Additional filtering based on current workspace
	// This is handled in _node_visible_in_current_workspace()
}

void PalimpsestNodeCreator::set_category_filter(NodeCategory p_category) {
	current_category = p_category;
	category_filter->select(p_category);
	_rebuild_tree();
}