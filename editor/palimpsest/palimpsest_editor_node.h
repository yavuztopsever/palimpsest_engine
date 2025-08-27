/**************************************************************************/
/*  palimpsest_editor_node.h                                              */
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

#include "editor/editor_node.h"
#include "scene/gui/menu_bar.h"
#include "scene/gui/menu_button.h"

class PalimpsestWorkspaceManager;
class PalimpsestThemeManager;

class PalimpsestEditorNode : public EditorNode {
	GDCLASS(PalimpsestEditorNode, EditorNode);

public:
	enum PalimpsestWorkspace {
		WORKSPACE_NARRATIVE = 0,
		WORKSPACE_CONSCIOUSNESS,
		WORKSPACE_BUREAUCRACY, 
		WORKSPACE_WORLD_BUILDING,
		WORKSPACE_TESTING,
		WORKSPACE_MAX
	};

private:
	PalimpsestWorkspaceManager *workspace_manager = nullptr;
	PalimpsestThemeManager *theme_manager = nullptr;
	
	MenuButton *palimpsest_menu = nullptr;
	PopupMenu *workspace_menu = nullptr;
	
	Button *narrative_button = nullptr;
	Button *consciousness_button = nullptr;
	Button *bureaucracy_button = nullptr;
	Button *world_button = nullptr;
	Button *testing_button = nullptr;
	
	Label *consciousness_indicator = nullptr;
	Label *narrative_indicator = nullptr;
	Label *bureaucracy_indicator = nullptr;
	
	int current_workspace = WORKSPACE_NARRATIVE;
	bool ui_reorganized = false;

	void _reorganize_ui();
	void _setup_palimpsest_toolbar();
	void _setup_palimpsest_menu();
	void _setup_status_indicators();
	void _reorganize_docks();
	void _filter_node_creation_dialog();
	
	void _workspace_changed(int p_workspace);
	void _palimpsest_menu_option(int p_option);
	void _update_status_indicators();

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	enum PalimpsestMenuOptions {
		MENU_CONSCIOUSNESS_SETTINGS,
		MENU_BUREAUCRACY_TEMPLATES,
		MENU_NARRATIVE_STATISTICS,
		MENU_EXPORT_DIALOGUE,
		MENU_IMPORT_INK_SCRIPTS,
		MENU_INITIALIZE_CRPG_TEMPLATE,
		MENU_VALIDATE_SETUP
	};

	void switch_workspace(PalimpsestWorkspace p_workspace);
	PalimpsestWorkspace get_current_workspace() const { return (PalimpsestWorkspace)current_workspace; }
	
	void update_consciousness_count(int p_count);
	void update_narrative_branch(const String &p_branch);
	void update_bureaucracy_complexity(int p_score);
	
	PalimpsestWorkspaceManager *get_workspace_manager() const { return workspace_manager; }
	PalimpsestThemeManager *get_theme_manager() const { return theme_manager; }

	PalimpsestEditorNode();
	~PalimpsestEditorNode();
};

VARIANT_ENUM_CAST(PalimpsestEditorNode::PalimpsestWorkspace);