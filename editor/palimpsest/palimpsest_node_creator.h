/**************************************************************************/
/*  palimpsest_node_creator.h                                             */
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

#include "scene/gui/dialogs.h"
#include "scene/gui/tree.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/option_button.h"

class PalimpsestWorkspaceManager;

class PalimpsestNodeCreator : public AcceptDialog {
	GDCLASS(PalimpsestNodeCreator, AcceptDialog);

public:
	enum NodeCategory {
		CATEGORY_ALL,
		CATEGORY_NARRATIVE,
		CATEGORY_CONSCIOUSNESS,
		CATEGORY_BUREAUCRACY,
		CATEGORY_WORLD,
		CATEGORY_ESSENTIAL,
		CATEGORY_CUSTOM
	};

private:
	struct NodeInfo {
		String name;
		String class_name;
		String description;
		String icon_path;
		NodeCategory category;
		Vector<String> workspace_visibility; // Which workspaces show this node
		bool is_palimpsest_node = false;
	};
	
	Tree *node_tree = nullptr;
	LineEdit *search_edit = nullptr;
	OptionButton *category_filter = nullptr;
	Label *description_label = nullptr;
	Button *create_button = nullptr;
	
	PalimpsestWorkspaceManager *workspace_manager = nullptr;
	Vector<NodeInfo> node_database;
	HashMap<String, TreeItem*> category_items;
	TreeItem *root_item = nullptr;
	
	NodeCategory current_category = CATEGORY_ALL;
	String current_search_filter;
	String selected_node_class;
	
	void _initialize_node_database();
	void _add_palimpsest_nodes();
	void _add_essential_godot_nodes();
	void _filter_nodes_by_workspace();
	
	void _rebuild_tree();
	void _create_category_item(NodeCategory p_category, const String &p_name);
	void _add_node_to_tree(const NodeInfo &p_node_info);
	
	void _on_search_text_changed(const String &p_text);
	void _on_category_selected(int p_index);
	void _on_item_selected();
	void _on_item_activated();
	void _on_create_pressed();
	
	bool _node_matches_filter(const NodeInfo &p_node_info) const;
	bool _node_visible_in_current_workspace(const NodeInfo &p_node_info) const;

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void set_workspace_manager(PalimpsestWorkspaceManager *p_manager);
	void refresh_for_current_workspace();
	
	String get_selected_node_class() const { return selected_node_class; }
	void set_category_filter(NodeCategory p_category);
	
	signal_type selected_node_class_changed;

	PalimpsestNodeCreator();
};