/**************************************************************************/
/*  palimpsest_workspace_manager.h                                        */
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
#include "core/templates/hash_map.h"

class PalimpsestEditorNode;
class Control;
class ConfigFile;

class PalimpsestWorkspaceManager : public RefCounted {
	GDCLASS(PalimpsestWorkspaceManager, RefCounted);

public:
	enum WorkspaceType {
		WORKSPACE_NARRATIVE = 0,
		WORKSPACE_CONSCIOUSNESS,
		WORKSPACE_BUREAUCRACY,
		WORKSPACE_WORLD_BUILDING,
		WORKSPACE_TESTING,
		WORKSPACE_MAX
	};

private:
	struct WorkspaceLayout {
		String name;
		String description;
		HashMap<String, Variant> dock_visibility;
		HashMap<String, Vector2> dock_sizes;
		HashMap<String, int> dock_positions;
		bool hide_3d_tools = false;
		bool hide_animation_tools = false;
		Vector<String> visible_node_types;
	};

	PalimpsestEditorNode *editor_node = nullptr;
	WorkspaceType current_workspace = WORKSPACE_NARRATIVE;
	HashMap<WorkspaceType, WorkspaceLayout> workspace_layouts;
	
	void _initialize_default_layouts();
	void _setup_narrative_workspace();
	void _setup_consciousness_workspace();
	void _setup_bureaucracy_workspace();
	void _setup_world_building_workspace();
	void _setup_testing_workspace();
	
	void _apply_workspace_layout(const WorkspaceLayout &p_layout);
	void _save_current_layout();

protected:
	static void _bind_methods();

public:
	void switch_to_workspace(WorkspaceType p_workspace);
	WorkspaceType get_current_workspace() const { return current_workspace; }
	
	void save_workspace_layout(WorkspaceType p_workspace, const String &p_name);
	void load_workspace_layout(WorkspaceType p_workspace, const String &p_name);
	
	void save_layouts_to_config(Ref<ConfigFile> p_config);
	void load_layouts_from_config(Ref<ConfigFile> p_config);
	
	Vector<String> get_workspace_names() const;
	String get_workspace_description(WorkspaceType p_workspace) const;
	
	void set_dock_visibility(const String &p_dock_name, bool p_visible);
	bool get_dock_visibility(const String &p_dock_name) const;
	
	void customize_node_creation_for_workspace(WorkspaceType p_workspace);

	PalimpsestWorkspaceManager(PalimpsestEditorNode *p_editor_node);
	~PalimpsestWorkspaceManager();
};