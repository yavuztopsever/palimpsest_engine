@tool
extends EditorPlugin
# Palimpsest CRPG Tools - Editor Plugin
# Provides specialized tools for consciousness-driven narrative CRPGs

const DialogueGraphEditor = preload("res://addons/palimpsest_tools/dialogue_graph_editor.gd")
const ConsciousnessFieldPainter = preload("res://addons/palimpsest_tools/consciousness_field_painter.gd")
const WFCPreviewDock = preload("res://addons/palimpsest_tools/wfc_preview_dock.gd")
const BureaucracyDesigner = preload("res://addons/palimpsest_tools/bureaucracy_designer.gd")

var dialogue_graph_dock
var consciousness_field_dock  
var wfc_preview_dock
var bureaucracy_dock
var palimpsest_main_screen

func _enter_tree():
	print("Palimpsest CRPG Tools: Initializing...")
	
	# Add custom types for Palimpsest nodes
	add_custom_type(
		"DialogueNode",
		"Node", 
		preload("res://modules/palimpsest/narrative/dialogue_system/dialogue_node.gd"),
		preload("res://addons/palimpsest_tools/icons/dialogue_node.svg")
	)
	
	add_custom_type(
		"ConsciousnessField",
		"Area3D",
		preload("res://modules/palimpsest/consciousness/reality_distortion/consciousness_field.gd"), 
		preload("res://addons/palimpsest_tools/icons/consciousness_field.svg")
	)
	
	add_custom_type(
		"WFCGenerator", 
		"Node",
		preload("res://modules/palimpsest/procgen/wfc/wfc_generator.gd"),
		preload("res://addons/palimpsest_tools/icons/wfc_generator.svg")
	)
	
	add_custom_type(
		"QuestTracker",
		"Node",
		preload("res://modules/palimpsest/narrative/quest_system/quest_tracker.gd"),
		preload("res://addons/palimpsest_tools/icons/quest_tracker.svg")
	)
	
	add_custom_type(
		"PaletteEnforcer",
		"Node", 
		preload("res://modules/palimpsest/renderer/palette_enforcer/palette_enforcer.gd"),
		preload("res://addons/palimpsest_tools/icons/palette_enforcer.svg")
	)
	
	# Create and add docks
	dialogue_graph_dock = DialogueGraphEditor.new()
	add_control_to_dock(DOCK_SLOT_LEFT_UR, dialogue_graph_dock)
	
	consciousness_field_dock = ConsciousnessFieldPainter.new()
	add_control_to_dock(DOCK_SLOT_LEFT_BR, consciousness_field_dock)
	
	wfc_preview_dock = WFCPreviewDock.new()
	add_control_to_dock(DOCK_SLOT_RIGHT_UL, wfc_preview_dock)
	
	bureaucracy_dock = BureaucracyDesigner.new()
	add_control_to_dock(DOCK_SLOT_RIGHT_BL, bureaucracy_dock)
	
	# Add main screen plugin for full CRPG workflow
	palimpsest_main_screen = preload("res://addons/palimpsest_tools/palimpsest_main_screen.gd").new()
	add_control_to_container(CONTAINER_CANVAS_EDITOR_MENU, palimpsest_main_screen)
	
	# Add project settings for Palimpsest
	add_palimpsest_project_settings()
	
	# Connect to editor selection to update tools
	EditorInterface.get_selection().selection_changed.connect(_on_selection_changed)
	
	print("Palimpsest CRPG Tools: Initialized successfully!")

func _exit_tree():
	print("Palimpsest CRPG Tools: Shutting down...")
	
	# Remove custom types
	remove_custom_type("DialogueNode")
	remove_custom_type("ConsciousnessField") 
	remove_custom_type("WFCGenerator")
	remove_custom_type("QuestTracker")
	remove_custom_type("PaletteEnforcer")
	
	# Remove docks
	if dialogue_graph_dock:
		remove_control_from_docks(dialogue_graph_dock)
		dialogue_graph_dock.queue_free()
	
	if consciousness_field_dock:
		remove_control_from_docks(consciousness_field_dock)
		consciousness_field_dock.queue_free()
		
	if wfc_preview_dock:
		remove_control_from_docks(wfc_preview_dock)
		wfc_preview_dock.queue_free()
		
	if bureaucracy_dock:
		remove_control_from_docks(bureaucracy_dock)
		bureaucracy_dock.queue_free()
	
	if palimpsest_main_screen:
		remove_control_from_container(CONTAINER_CANVAS_EDITOR_MENU, palimpsest_main_screen)
		palimpsest_main_screen.queue_free()
	
	# Disconnect signals
	if EditorInterface.get_selection().selection_changed.is_connected(_on_selection_changed):
		EditorInterface.get_selection().selection_changed.disconnect(_on_selection_changed)
	
	print("Palimpsest CRPG Tools: Shutdown complete.")

func add_palimpsest_project_settings():
	"""Add Palimpsest-specific project settings"""
	
	# Consciousness System Settings
	if not ProjectSettings.has_setting("palimpsest/consciousness/global_influence"):
		ProjectSettings.set_setting("palimpsest/consciousness/global_influence", 1.0)
	
	if not ProjectSettings.has_setting("palimpsest/consciousness/max_distortion_radius"):
		ProjectSettings.set_setting("palimpsest/consciousness/max_distortion_radius", 50.0)
	
	if not ProjectSettings.has_setting("palimpsest/consciousness/cascade_threshold"):
		ProjectSettings.set_setting("palimpsest/consciousness/cascade_threshold", 8.0)
	
	# Dialogue System Settings
	if not ProjectSettings.has_setting("palimpsest/dialogue/auto_save_conversations"):
		ProjectSettings.set_setting("palimpsest/dialogue/auto_save_conversations", true)
	
	if not ProjectSettings.has_setting("palimpsest/dialogue/skill_check_difficulty"):
		ProjectSettings.set_setting("palimpsest/dialogue/skill_check_difficulty", 1.0)
	
	# Visual Style Settings  
	if not ProjectSettings.has_setting("palimpsest/style/enforce_palette"):
		ProjectSettings.set_setting("palimpsest/style/enforce_palette", true)
	
	if not ProjectSettings.has_setting("palimpsest/style/expressionist_intensity"):
		ProjectSettings.set_setting("palimpsest/style/expressionist_intensity", 0.7)
	
	if not ProjectSettings.has_setting("palimpsest/style/red_accent_trigger"):
		ProjectSettings.set_setting("palimpsest/style/red_accent_trigger", "anger")
	
	# Procedural Generation Settings
	if not ProjectSettings.has_setting("palimpsest/procgen/wfc_max_iterations"):
		ProjectSettings.set_setting("palimpsest/procgen/wfc_max_iterations", 10000)
	
	if not ProjectSettings.has_setting("palimpsest/procgen/consciousness_influence"):
		ProjectSettings.set_setting("palimpsest/procgen/consciousness_influence", true)
	
	# Bureaucracy Settings
	if not ProjectSettings.has_setting("palimpsest/bureaucracy/form_complexity"):
		ProjectSettings.set_setting("palimpsest/bureaucracy/form_complexity", 3)
	
	if not ProjectSettings.has_setting("palimpsest/bureaucracy/stamping_requirement"):
		ProjectSettings.set_setting("palimpsest/bureaucracy/stamping_requirement", true)
	
	ProjectSettings.save()

func _on_selection_changed():
	"""Update tools based on selected nodes"""
	var selected_nodes = EditorInterface.get_selection().get_selected_nodes()
	
	if selected_nodes.is_empty():
		return
	
	var selected_node = selected_nodes[0]
	
	# Update consciousness field painter if ConsciousnessField is selected
	if selected_node.get_script() and selected_node.get_script().get_path().find("consciousness_field") != -1:
		if consciousness_field_dock:
			consciousness_field_dock.set_target_field(selected_node)
	
	# Update WFC preview if WFCGenerator is selected
	if selected_node.get_script() and selected_node.get_script().get_path().find("wfc_generator") != -1:
		if wfc_preview_dock:
			wfc_preview_dock.set_target_generator(selected_node)
	
	# Update dialogue editor if DialogueNode is selected  
	if selected_node.get_script() and selected_node.get_script().get_path().find("dialogue_node") != -1:
		if dialogue_graph_dock:
			dialogue_graph_dock.set_target_dialogue(selected_node)

func get_plugin_name():
	return "Palimpsest CRPG Tools"

func has_main_screen():
	return true

func get_plugin_icon():
	return preload("res://addons/palimpsest_tools/icons/palimpsest_icon.svg")

# Custom import plugins for Palimpsest assets
class_name PalimpsestImportPlugin

func handles(source_file):
	return source_file.get_extension() in ["yarn", "form", "consciousness"]

func get_importer_name():
	return "palimpsest.asset"

func get_visible_name():
	return "Palimpsest Asset"

func get_recognized_extensions():
	return ["yarn", "form", "consciousness"]

func get_save_extension():
	return "res"

func get_resource_type():
	return "Resource"

func import(source_file, save_path, options, platform_variants, gen_files):
	var file = FileAccess.open(source_file, FileAccess.READ)
	if file == null:
		return FileAccess.get_open_error()
	
	var content = file.get_as_text()
	file.close()
	
	var resource
	var ext = source_file.get_extension()
	
	if ext == "yarn":
		# Import Yarn dialogue files
		resource = preload("res://addons/palimpsest_tools/resources/yarn_dialogue_resource.gd").new()
		resource.parse_yarn_content(content)
	elif ext == "form":
		# Import bureaucratic form definitions
		resource = preload("res://addons/palimpsest_tools/resources/form_resource.gd").new()
		resource.parse_form_content(content)
	elif ext == "consciousness":
		# Import consciousness field presets
		resource = preload("res://addons/palimpsest_tools/resources/consciousness_preset_resource.gd").new() 
		resource.parse_consciousness_content(content)
	else:
		return ERR_FILE_UNRECOGNIZED
	
	return ResourceSaver.save(resource, "%s.%s" % [save_path, get_save_extension()])