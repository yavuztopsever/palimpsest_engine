/**************************************************************************/
/*  palimpsest_workspace_manager.cpp                                      */
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

#include "palimpsest_workspace_manager.h"
#include "palimpsest_editor_node.h"

#include "core/io/config_file.h"
#include "editor/docks/editor_dock_manager.h"

void PalimpsestWorkspaceManager::_bind_methods() {
	ClassDB::bind_method(D_METHOD("switch_to_workspace", "workspace"), &PalimpsestWorkspaceManager::switch_to_workspace);
	ClassDB::bind_method(D_METHOD("get_current_workspace"), &PalimpsestWorkspaceManager::get_current_workspace);
	ClassDB::bind_method(D_METHOD("set_dock_visibility", "dock_name", "visible"), &PalimpsestWorkspaceManager::set_dock_visibility);
	ClassDB::bind_method(D_METHOD("get_dock_visibility", "dock_name"), &PalimpsestWorkspaceManager::get_dock_visibility);
	
	BIND_ENUM_CONSTANT(WORKSPACE_NARRATIVE);
	BIND_ENUM_CONSTANT(WORKSPACE_CONSCIOUSNESS);
	BIND_ENUM_CONSTANT(WORKSPACE_BUREAUCRACY);
	BIND_ENUM_CONSTANT(WORKSPACE_WORLD_BUILDING);
	BIND_ENUM_CONSTANT(WORKSPACE_TESTING);
}

PalimpsestWorkspaceManager::PalimpsestWorkspaceManager(PalimpsestEditorNode *p_editor_node) {
	editor_node = p_editor_node;
	_initialize_default_layouts();
}

PalimpsestWorkspaceManager::~PalimpsestWorkspaceManager() {
	// Cleanup if needed
}

void PalimpsestWorkspaceManager::_initialize_default_layouts() {
	_setup_narrative_workspace();
	_setup_consciousness_workspace();
	_setup_bureaucracy_workspace();
	_setup_world_building_workspace();
	_setup_testing_workspace();
}

void PalimpsestWorkspaceManager::_setup_narrative_workspace() {
	WorkspaceLayout layout;
	layout.name = "Narrative Design";
	layout.description = "Optimized for dialogue writing and quest design";
	
	// Dock visibility - emphasize dialogue and quest tools
	layout.dock_visibility["DialogueGraphEditor"] = true;
	layout.dock_visibility["QuestTracker"] = true;
	layout.dock_visibility["ThoughtCabinet"] = true;
	layout.dock_visibility["SceneTree"] = true;
	layout.dock_visibility["Inspector"] = true;
	layout.dock_visibility["FileSystem"] = true;
	layout.dock_visibility["ConsciousnessFieldPainter"] = false;
	layout.dock_visibility["WFCPreviewDock"] = false;
	layout.dock_visibility["BureaucracyDesigner"] = false;
	
	// Hide tools not needed for narrative work
	layout.hide_3d_tools = true;
	layout.hide_animation_tools = true;
	
	// Visible node types for this workspace
	layout.visible_node_types.push_back("DialogueNode");
	layout.visible_node_types.push_back("QuestNode");
	layout.visible_node_types.push_back("ThoughtNode");
	layout.visible_node_types.push_back("ChoiceNode");
	layout.visible_node_types.push_back("SkillCheckNode");
	layout.visible_node_types.push_back("Node");
	layout.visible_node_types.push_back("Control");
	layout.visible_node_types.push_back("Label");
	layout.visible_node_types.push_back("RichTextLabel");
	
	workspace_layouts[WORKSPACE_NARRATIVE] = layout;
}

void PalimpsestWorkspaceManager::_setup_consciousness_workspace() {
	WorkspaceLayout layout;
	layout.name = "Consciousness Tuning";
	layout.description = "Tools for reality distortion and consciousness field design";
	
	// Dock visibility - emphasize consciousness tools
	layout.dock_visibility["ConsciousnessFieldPainter"] = true;
	layout.dock_visibility["RealityDistortionPreview"] = true;
	layout.dock_visibility["EmotionalStateDebugger"] = true;
	layout.dock_visibility["FieldOverlapVisualizer"] = true;
	layout.dock_visibility["SceneTree"] = true;
	layout.dock_visibility["Inspector"] = true;
	layout.dock_visibility["FileSystem"] = true;
	layout.dock_visibility["DialogueGraphEditor"] = false;
	layout.dock_visibility["BureaucracyDesigner"] = false;
	
	layout.hide_3d_tools = false; // Need 3D tools for consciousness fields
	layout.hide_animation_tools = true;
	
	// Visible node types for consciousness work
	layout.visible_node_types.push_back("ConsciousnessField");
	layout.visible_node_types.push_back("RealityDistortion");
	layout.visible_node_types.push_back("MemoryFragment");
	layout.visible_node_types.push_back("EmotionalTrigger");
	layout.visible_node_types.push_back("Area3D");
	layout.visible_node_types.push_back("Node3D");
	layout.visible_node_types.push_back("CollisionShape3D");
	
	workspace_layouts[WORKSPACE_CONSCIOUSNESS] = layout;
}

void PalimpsestWorkspaceManager::_setup_bureaucracy_workspace() {
	WorkspaceLayout layout;
	layout.name = "Bureaucracy Design";
	layout.description = "Form design and paperwork system creation";
	
	// Dock visibility - emphasize bureaucratic tools
	layout.dock_visibility["BureaucracyDesigner"] = true;
	layout.dock_visibility["FormPreview"] = true;
	layout.dock_visibility["PaperworkFlowVisualizer"] = true;
	layout.dock_visibility["RedTapeComplexityMeter"] = true;
	layout.dock_visibility["SceneTree"] = true;
	layout.dock_visibility["Inspector"] = true;
	layout.dock_visibility["FileSystem"] = true;
	layout.dock_visibility["DialogueGraphEditor"] = false;
	layout.dock_visibility["ConsciousnessFieldPainter"] = false;
	
	layout.hide_3d_tools = true;
	layout.hide_animation_tools = true;
	
	// Visible node types for bureaucracy work
	layout.visible_node_types.push_back("FormNode");
	layout.visible_node_types.push_back("StampValidator");
	layout.visible_node_types.push_back("PaperworkContainer");
	layout.visible_node_types.push_back("BureaucratNPC");
	layout.visible_node_types.push_back("Control");
	layout.visible_node_types.push_back("Panel");
	layout.visible_node_types.push_back("LineEdit");
	layout.visible_node_types.push_back("CheckBox");
	layout.visible_node_types.push_back("Button");
	
	workspace_layouts[WORKSPACE_BUREAUCRACY] = layout;
}

void PalimpsestWorkspaceManager::_setup_world_building_workspace() {
	WorkspaceLayout layout;
	layout.name = "World Building";
	layout.description = "Isometric level design with consciousness integration";
	
	// Dock visibility - emphasize world building tools
	layout.dock_visibility["WFCPreviewDock"] = true;
	layout.dock_visibility["BrutalistArchitecturePresets"] = true;
	layout.dock_visibility["ConsciousnessInfluenceOverlay"] = true;
	layout.dock_visibility["IsometricCameraControls"] = true;
	layout.dock_visibility["SceneTree"] = true;
	layout.dock_visibility["Inspector"] = true;
	layout.dock_visibility["FileSystem"] = true;
	layout.dock_visibility["DialogueGraphEditor"] = false;
	layout.dock_visibility["BureaucracyDesigner"] = false;
	
	layout.hide_3d_tools = false; // Need 3D for isometric view
	layout.hide_animation_tools = true;
	
	// Visible node types for world building
	layout.visible_node_types.push_back("WFCGenerator");
	layout.visible_node_types.push_back("BrutalistTile");
	layout.visible_node_types.push_back("IsometricCamera");
	layout.visible_node_types.push_back("EnvironmentTrigger");
	layout.visible_node_types.push_back("Node3D");
	layout.visible_node_types.push_back("StaticBody3D");
	layout.visible_node_types.push_back("MeshInstance3D");
	layout.visible_node_types.push_back("CollisionShape3D");
	
	workspace_layouts[WORKSPACE_WORLD_BUILDING] = layout;
}

void PalimpsestWorkspaceManager::_setup_testing_workspace() {
	WorkspaceLayout layout;
	layout.name = "Testing & Debug";
	layout.description = "Thought cabinet testing and skill check simulation";
	
	// Dock visibility - emphasize testing tools
	layout.dock_visibility["ThoughtCabinetTester"] = true;
	layout.dock_visibility["SkillCheckSimulator"] = true;
	layout.dock_visibility["NarrativeDebugger"] = true;
	layout.dock_visibility["ConsciousnessMonitor"] = true;
	layout.dock_visibility["SceneTree"] = true;
	layout.dock_visibility["Inspector"] = true;
	layout.dock_visibility["FileSystem"] = true;
	layout.dock_visibility["DialogueGraphEditor"] = true; // For testing dialogue
	
	layout.hide_3d_tools = false;
	layout.hide_animation_tools = false; // May need for testing
	
	// All node types visible for testing
	layout.visible_node_types.push_back("*"); // Wildcard for all types
	
	workspace_layouts[WORKSPACE_TESTING] = layout;
}

void PalimpsestWorkspaceManager::switch_to_workspace(WorkspaceType p_workspace) {
	if (p_workspace < 0 || p_workspace >= WORKSPACE_MAX) {
		print_error("Invalid workspace type");
		return;
	}
	
	if (current_workspace != p_workspace) {
		_save_current_layout();
	}
	
	current_workspace = p_workspace;
	
	if (workspace_layouts.has(p_workspace)) {
		_apply_workspace_layout(workspace_layouts[p_workspace]);
		customize_node_creation_for_workspace(p_workspace);
		
		print_verbose(vformat("Switched to workspace: %s", workspace_layouts[p_workspace].name));
	}
}

void PalimpsestWorkspaceManager::_apply_workspace_layout(const WorkspaceLayout &p_layout) {
	EditorDockManager *dock_manager = EditorDockManager::get_singleton();
	if (!dock_manager) {
		print_error("Could not access dock manager");
		return;
	}
	
	// Apply dock visibility settings
	for (const KeyValue<String, Variant> &kv : p_layout.dock_visibility) {
		// This would need to interface with the actual docks
		// For now, just log the intended changes
		print_verbose(vformat("Setting dock %s visibility: %s", kv.key, kv.value.operator bool() ? "visible" : "hidden"));
	}
	
	// Apply tool visibility settings
	if (p_layout.hide_3d_tools) {
		print_verbose("Hiding 3D tools for this workspace");
	}
	
	if (p_layout.hide_animation_tools) {
		print_verbose("Hiding animation tools for this workspace");
	}
}

void PalimpsestWorkspaceManager::_save_current_layout() {
	// Save current dock positions and sizes for the current workspace
	print_verbose("Saving current workspace layout");
}

void PalimpsestWorkspaceManager::customize_node_creation_for_workspace(WorkspaceType p_workspace) {
	if (!workspace_layouts.has(p_workspace)) {
		return;
	}
	
	const WorkspaceLayout &layout = workspace_layouts[p_workspace];
	
	// Filter visible node types based on workspace
	print_verbose(vformat("Customizing node creation for workspace with %d visible types", layout.visible_node_types.size()));
	
	// This would interface with the CreateDialog to filter available nodes
	// Implementation would depend on modifying CreateDialog to support filtering
}

void PalimpsestWorkspaceManager::save_workspace_layout(WorkspaceType p_workspace, const String &p_name) {
	// Save custom workspace layout
	print_verbose(vformat("Saving workspace layout %s for workspace %d", p_name, p_workspace));
}

void PalimpsestWorkspaceManager::load_workspace_layout(WorkspaceType p_workspace, const String &p_name) {
	// Load custom workspace layout
	print_verbose(vformat("Loading workspace layout %s for workspace %d", p_name, p_workspace));
}

void PalimpsestWorkspaceManager::save_layouts_to_config(Ref<ConfigFile> p_config) {
	if (p_config.is_null()) {
		return;
	}
	
	for (const KeyValue<WorkspaceType, WorkspaceLayout> &kv : workspace_layouts) {
		String section = vformat("workspace_%d", kv.key);
		p_config->set_value(section, "name", kv.value.name);
		p_config->set_value(section, "description", kv.value.description);
		p_config->set_value(section, "hide_3d_tools", kv.value.hide_3d_tools);
		p_config->set_value(section, "hide_animation_tools", kv.value.hide_animation_tools);
	}
}

void PalimpsestWorkspaceManager::load_layouts_from_config(Ref<ConfigFile> p_config) {
	if (p_config.is_null()) {
		return;
	}
	
	// Load workspace configurations from config file
	for (int i = 0; i < WORKSPACE_MAX; i++) {
		String section = vformat("workspace_%d", i);
		if (p_config->has_section(section)) {
			// Load workspace settings
		}
	}
}

Vector<String> PalimpsestWorkspaceManager::get_workspace_names() const {
	Vector<String> names;
	for (const KeyValue<WorkspaceType, WorkspaceLayout> &kv : workspace_layouts) {
		names.push_back(kv.value.name);
	}
	return names;
}

String PalimpsestWorkspaceManager::get_workspace_description(WorkspaceType p_workspace) const {
	if (workspace_layouts.has(p_workspace)) {
		return workspace_layouts[p_workspace].description;
	}
	return "";
}

void PalimpsestWorkspaceManager::set_dock_visibility(const String &p_dock_name, bool p_visible) {
	if (workspace_layouts.has(current_workspace)) {
		workspace_layouts[current_workspace].dock_visibility[p_dock_name] = p_visible;
	}
}

bool PalimpsestWorkspaceManager::get_dock_visibility(const String &p_dock_name) const {
	if (workspace_layouts.has(current_workspace)) {
		const WorkspaceLayout &layout = workspace_layouts[current_workspace];
		if (layout.dock_visibility.has(p_dock_name)) {
			return layout.dock_visibility[p_dock_name];
		}
	}
	return true; // Default to visible
}