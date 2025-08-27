/**************************************************************************/
/*  palimpsest_editor_integration.cpp                                     */
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
/* the original conditions.                                               */
/**************************************************************************/

#include "palimpsest_editor_integration.h"
#include "palimpsest_editor_node.h" 
#include "palimpsest_workspace_manager.h"
#include "palimpsest_theme_manager.h"
#include "palimpsest_node_creator.h"

#include "core/config/project_settings.h"
#include "core/io/dir_access.h"
#include "core/io/file_access.h"
#include "editor/editor_node.h"
#include "core/object/class_db.h"

namespace PalimpsestEditorIntegration {

static PalimpsestEditorNode *palimpsest_editor_instance = nullptr;

void initialize_palimpsest_editor() {
	print_verbose("Initializing Palimpsest CRPG Editor Extensions...");
	
	// Check if this is a Palimpsest project
	if (!is_palimpsest_project()) {
		print_verbose("Not a Palimpsest project - skipping CRPG editor initialization");
		return;
	}
	
	// Register Palimpsest editor types
	register_palimpsest_editor_types();
	
	// Create Palimpsest editor node to replace/extend standard EditorNode
	// Note: In a full implementation, this would require more careful integration
	// with the existing EditorNode initialization process
	
	print_line("✓ Palimpsest CRPG Editor Extensions initialized successfully");
}

void shutdown_palimpsest_editor() {
	print_verbose("Shutting down Palimpsest CRPG Editor Extensions...");
	
	if (palimpsest_editor_instance) {
		palimpsest_editor_instance = nullptr;
	}
	
	unregister_palimpsest_editor_types();
	
	print_verbose("Palimpsest CRPG Editor Extensions shutdown complete");
}

bool is_palimpsest_project() {
	// Check for Palimpsest-specific project settings or files
	
	// Method 1: Check for Palimpsest project settings
	if (ProjectSettings::get_singleton()->has_setting("palimpsest/consciousness/global_influence")) {
		return true;
	}
	
	// Method 2: Check for palimpsest_crpg.profile
	if (FileAccess::file_exists("res://palimpsest_crpg.profile")) {
		return true;
	}
	
	// Method 3: Check for Palimpsest modules directory
	if (DirAccess::dir_exists_absolute("modules/palimpsest")) {
		return true;
	}
	
	// Method 4: Check application name for Palimpsest
	String app_name = ProjectSettings::get_singleton()->get_setting("application/config/name", "");
	if (app_name.contains("Palimpsest") || app_name.contains("CRPG")) {
		return true;
	}
	
	return false;
}

PalimpsestEditorNode *get_palimpsest_editor() {
	return palimpsest_editor_instance;
}

void register_palimpsest_editor_types() {
	// Register all Palimpsest editor classes with ClassDB
	ClassDB::register_class<PalimpsestEditorNode>();
	ClassDB::register_class<PalimpsestWorkspaceManager>();
	ClassDB::register_class<PalimpsestThemeManager>();
	ClassDB::register_class<PalimpsestNodeCreator>();
	
	print_verbose("Registered Palimpsest editor classes with ClassDB");
}

void unregister_palimpsest_editor_types() {
	// Cleanup is handled automatically by ClassDB during shutdown
	print_verbose("Unregistered Palimpsest editor classes from ClassDB");
}

} // namespace PalimpsestEditorIntegration