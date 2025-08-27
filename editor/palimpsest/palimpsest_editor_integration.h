/**************************************************************************/
/*  palimpsest_editor_integration.h                                       */
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

#pragma once

/**
 * Palimpsest Editor Integration
 * 
 * This file provides integration hooks for the Palimpsest CRPG editor
 * extensions into the main Godot editor. It handles initialization,
 * registration, and lifecycle management of Palimpsest-specific UI
 * components.
 */

class EditorNode;
class PalimpsestEditorNode;

namespace PalimpsestEditorIntegration {
	
	/**
	 * Initialize Palimpsest editor extensions
	 * Called during editor startup to setup CRPG-specific UI
	 */
	void initialize_palimpsest_editor();
	
	/**
	 * Shutdown Palimpsest editor extensions
	 * Called during editor shutdown to cleanup resources
	 */
	void shutdown_palimpsest_editor();
	
	/**
	 * Check if Palimpsest editor extensions are enabled
	 * Returns true if the current project is using Palimpsest features
	 */
	bool is_palimpsest_project();
	
	/**
	 * Get the Palimpsest editor node instance
	 * Returns nullptr if Palimpsest extensions are not active
	 */
	PalimpsestEditorNode *get_palimpsest_editor();
	
	/**
	 * Register Palimpsest editor classes with the ClassDB
	 * This ensures all custom editor components are available
	 */
	void register_palimpsest_editor_types();
	
	/**
	 * Unregister Palimpsest editor classes from ClassDB
	 * Cleanup during shutdown
	 */
	void unregister_palimpsest_editor_types();

} // namespace PalimpsestEditorIntegration