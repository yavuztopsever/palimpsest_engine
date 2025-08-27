/**************************************************************************/
/*  palimpsest_editor_node.cpp                                            */
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

#include "palimpsest_editor_node.h"
#include "palimpsest_workspace_manager.h" 
#include "palimpsest_theme_manager.h"

#include "core/os/os.h"
#include "editor/docks/editor_dock_manager.h"
#include "editor/editor_main_screen.h"
#include "scene/gui/button.h"
#include "scene/gui/label.h"
#include "scene/gui/separator.h"

void PalimpsestEditorNode::_bind_methods() {
	ClassDB::bind_method(D_METHOD("switch_workspace", "workspace"), &PalimpsestEditorNode::switch_workspace);
	ClassDB::bind_method(D_METHOD("get_current_workspace"), &PalimpsestEditorNode::get_current_workspace);
	ClassDB::bind_method(D_METHOD("update_consciousness_count", "count"), &PalimpsestEditorNode::update_consciousness_count);
	ClassDB::bind_method(D_METHOD("update_narrative_branch", "branch"), &PalimpsestEditorNode::update_narrative_branch);
	ClassDB::bind_method(D_METHOD("update_bureaucracy_complexity", "score"), &PalimpsestEditorNode::update_bureaucracy_complexity);
}

PalimpsestEditorNode::PalimpsestEditorNode() {
	workspace_manager = memnew(PalimpsestWorkspaceManager(this));
	theme_manager = memnew(PalimpsestThemeManager);
}

PalimpsestEditorNode::~PalimpsestEditorNode() {
	if (workspace_manager) {
		memdelete(workspace_manager);
	}
	if (theme_manager) {
		memdelete(theme_manager);
	}
}

void PalimpsestEditorNode::_notification(int p_what) {
	EditorNode::_notification(p_what);
	
	switch (p_what) {
		case NOTIFICATION_READY: {
			if (!ui_reorganized) {
				call_deferred("_reorganize_ui");
				ui_reorganized = true;
			}
		} break;
		
		case NOTIFICATION_PROCESS: {
			_update_status_indicators();
		} break;
	}
}

void PalimpsestEditorNode::_reorganize_ui() {
	print_verbose("Palimpsest: Reorganizing UI for CRPG development...");
	
	// Apply Palimpsest theme first
	theme_manager->apply_palimpsest_theme();
	
	// Setup custom toolbar
	_setup_palimpsest_toolbar();
	
	// Setup custom menu
	_setup_palimpsest_menu();
	
	// Reorganize docks for CRPG workflow
	_reorganize_docks();
	
	// Setup status indicators
	_setup_status_indicators();
	
	// Filter node creation dialog
	_filter_node_creation_dialog();
	
	// Switch to default workspace
	switch_workspace(WORKSPACE_NARRATIVE);
	
	print_verbose("Palimpsest: UI reorganization complete.");
}

void PalimpsestEditorNode::_setup_palimpsest_toolbar() {
	// Get the main screen button container
	EditorMainScreen *main_screen = EditorNode::get_singleton()->get_editor_main_screen();
	if (!main_screen) {
		return;
	}
	
	// Remove existing buttons that aren't needed for CRPG development
	// (This would require modifying the EditorMainScreen to expose button management)
	
	// For now, we'll add our custom workspace buttons alongside existing ones
	// TODO: Implement button replacement once EditorMainScreen is modified
	
	print_verbose("Palimpsest: Custom toolbar setup (buttons integration pending)");
}

void PalimpsestEditorNode::_setup_palimpsest_menu() {
	// TODO: Fix menu bar access - get_menu_bar() method doesn't exist
	// MenuBar *menu_bar = EditorNode::get_singleton()->get_menu_bar();
	// if (!menu_bar) {
	//	return;
	// }
	
	// Add Palimpsest menu (disabled for now)
	palimpsest_menu = memnew(MenuButton);
	palimpsest_menu->set_text("Palimpsest");
	palimpsest_menu->set_tooltip_text("CRPG development tools and settings");
	
	PopupMenu *popup = palimpsest_menu->get_popup();
	popup->add_item("Consciousness Settings", MENU_CONSCIOUSNESS_SETTINGS);
	popup->add_item("Bureaucracy Templates", MENU_BUREAUCRACY_TEMPLATES);
	popup->add_item("Narrative Statistics", MENU_NARRATIVE_STATISTICS);
	popup->add_separator();
	popup->add_item("Export Dialogue to Yarn", MENU_EXPORT_DIALOGUE);
	popup->add_item("Import Ink/Twine Scripts", MENU_IMPORT_INK_SCRIPTS);
	popup->add_separator();
	popup->add_item("Initialize CRPG Template", MENU_INITIALIZE_CRPG_TEMPLATE);
	popup->add_item("Validate Palimpsest Setup", MENU_VALIDATE_SETUP);
	
	popup->connect("id_pressed", callable_mp(this, &PalimpsestEditorNode::_palimpsest_menu_option));
	
	// Add to menu bar (after Edit menu) - disabled due to menu_bar access issues
	// menu_bar->add_child(palimpsest_menu);
	
	print_verbose("Palimpsest: Custom menu added");
}

void PalimpsestEditorNode::_setup_status_indicators() {
	// Get status bar from EditorNode (this would require EditorNode modification to expose it)
	// For now, we'll create indicators that can be added later
	
	consciousness_indicator = memnew(Label);
	consciousness_indicator->set_text("Fields: 0");
	consciousness_indicator->set_tooltip_text("Active consciousness field count");
	
	narrative_indicator = memnew(Label); 
	narrative_indicator->set_text("Branch: Main");
	narrative_indicator->set_tooltip_text("Current narrative branch");
	
	bureaucracy_indicator = memnew(Label);
	bureaucracy_indicator->set_text("Complexity: 0");
	bureaucracy_indicator->set_tooltip_text("Bureaucratic complexity score");
	
	print_verbose("Palimpsest: Status indicators created (integration pending)");
}

void PalimpsestEditorNode::_reorganize_docks() {
	EditorDockManager *dock_manager = EditorDockManager::get_singleton();
	if (!dock_manager) {
		print_error("Palimpsest: Could not access dock manager");
		return;
	}
	
	// The current dock organization from plugin.gd:
	// LEFT_UR: DialogueGraphEditor -> Move to LEFT_BL for prominence
	// LEFT_BR: ConsciousnessFieldPainter -> Keep
	// RIGHT_UL: WFCPreviewDock -> Keep 
	// RIGHT_BL: BureaucracyDesigner -> Keep
	
	// Note: Actual dock reorganization would require accessing the plugin's docks
	// This is a placeholder for the reorganization logic
	
	print_verbose("Palimpsest: Dock reorganization setup (requires plugin dock access)");
}

void PalimpsestEditorNode::_filter_node_creation_dialog() {
	// Filter out nodes that aren't relevant for CRPG development
	// This would require modifying the CreateDialog class to support filtering
	
	print_verbose("Palimpsest: Node creation dialog filtering setup (requires CreateDialog modification)");
}

void PalimpsestEditorNode::_workspace_changed(int p_workspace) {
	if (p_workspace < 0 || p_workspace >= WORKSPACE_MAX) {
		return;
	}
	
	current_workspace = p_workspace;
	
	if (workspace_manager) {
		workspace_manager->switch_to_workspace((PalimpsestWorkspaceManager::WorkspaceType)p_workspace);
	}
}

void PalimpsestEditorNode::_palimpsest_menu_option(int p_option) {
	switch (p_option) {
		case MENU_CONSCIOUSNESS_SETTINGS: {
			// Open consciousness field configuration dialog
			print_line("Palimpsest: Opening consciousness settings...");
		} break;
		
		case MENU_BUREAUCRACY_TEMPLATES: {
			// Open bureaucracy template manager
			print_line("Palimpsest: Opening bureaucracy templates...");
		} break;
		
		case MENU_NARRATIVE_STATISTICS: {
			// Show narrative analytics
			print_line("Palimpsest: Showing narrative statistics...");
		} break;
		
		case MENU_EXPORT_DIALOGUE: {
			// Export dialogue to Yarn format
			print_line("Palimpsest: Exporting dialogue to Yarn...");
		} break;
		
		case MENU_IMPORT_INK_SCRIPTS: {
			// Import Ink/Twine scripts
			print_line("Palimpsest: Importing Ink/Twine scripts...");
		} break;
		
		case MENU_INITIALIZE_CRPG_TEMPLATE: {
			// Initialize new CRPG project template
			print_line("Palimpsest: Initializing CRPG template...");
		} break;
		
		case MENU_VALIDATE_SETUP: {
			// Run Palimpsest setup validation
			print_line("Palimpsest: Running setup validation...");
			OS::get_singleton()->execute("python3", {"validate_palimpsest.py"});
		} break;
	}
}

void PalimpsestEditorNode::_update_status_indicators() {
	// Update status indicators based on current project state
	// This would typically query the project for CRPG-specific data
}

void PalimpsestEditorNode::switch_workspace(PalimpsestWorkspace p_workspace) {
	if (p_workspace == (PalimpsestWorkspace)current_workspace) {
		return;
	}
	
	_workspace_changed((int)p_workspace);
	
	print_verbose(vformat("Palimpsest: Switched to workspace %d", p_workspace));
}

void PalimpsestEditorNode::update_consciousness_count(int p_count) {
	if (consciousness_indicator) {
		consciousness_indicator->set_text(vformat("Fields: %d", p_count));
	}
}

void PalimpsestEditorNode::update_narrative_branch(const String &p_branch) {
	if (narrative_indicator) {
		narrative_indicator->set_text(vformat("Branch: %s", p_branch));
	}
}

void PalimpsestEditorNode::update_bureaucracy_complexity(int p_score) {
	if (bureaucracy_indicator) {
		bureaucracy_indicator->set_text(vformat("Complexity: %d", p_score));
	}
}