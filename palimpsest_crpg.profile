[gd_resource type="EditorFeatureProfile" format=3]

[resource]

; Palimpsest CRPG Feature Profile
; Hides non-CRPG features and emphasizes narrative/consciousness tools

disabled_classes = PackedStringArray([
	; VR/XR classes - disabled for CRPG focus
	"XRInterface", "XRServer", "XRCamera3D", "XRController3D", "XROrigin3D",
	"OpenXRInterface", "MobileVRInterface", "WebXRInterface",
	
	; Advanced 3D features not needed for isometric CRPG
	"LightmapGI", "VoxelGI", "ReflectionProbe", "LightmapProbe", 
	"GPUParticles3D", "GPUParticlesCollision3D", "GPUParticlesAttractor3D",
	
	; Multiplayer - single player focused
	"MultiplayerPeer", "MultiplayerSpawner", "MultiplayerSynchronizer",
	"ENetConnection", "ENetMultiplayerPeer", "WebRTCPeerConnection",
	"WebSocketPeer", "WebSocketMultiplayerPeer",
	
	; Advanced physics not needed
	"SoftBody3D", "PhysicsServer3DExtension", "PhysicsDirectSpaceState3DExtension",
	
	; Mobile/web specific
	"MobileVRInterface", "JavaScriptObject", "JavaScriptBridge",
	
	; C# scripting
	"CSharpScript",
	
	; Advanced rendering
	"RenderingDevice", "RenderingServer", "RenderSceneData",
	"CompositorEffect", "RenderSceneBuffer",
	
	; Video playback
	"VideoStreamPlayer", "VideoStream", "VideoStreamTheora"
])

disabled_editors = PackedStringArray([
	; Hide VR/XR project settings
	"xr",
	
	; Hide multiplayer settings  
	"network",
	"multiplayer", 
	
	; Hide mobile settings
	"mobile",
	
	; Hide web export settings
	"web",
	
	; Advanced 3D tools
	"3d_gizmo",
	"lightmap"
])

disabled_properties = PackedStringArray([
	; Hide VR properties
	"xr_enabled",
	"vr_enabled", 
	
	; Hide multiplayer properties
	"multiplayer_poll",
	"network_peer",
	
	; Hide advanced rendering properties
	"rendering/global_illumination",
	"rendering/lightmapping",
	"rendering/vrs",
	
	; Mobile-specific properties
	"application/run/disable_stdout",
	"application/run/disable_stderr"
])

; Enable CRPG-specific features
enabled_classes = PackedStringArray([
	; Palimpsest CRPG classes
	"DialogueNode", "DialogueManager", "QuestNode", "QuestTracker",
	"ConsciousnessField", "RealityDistortion", "MemoryFragment",
	"WFCGenerator", "BrutalistGenerator", "PaletteEnforcer",
	"InventoryContainer", "StatManager", "ThoughtNode",
	
	; Essential 2D/3D for isometric view
	"Node2D", "Node3D", "CharacterBody2D", "CharacterBody3D",
	"Area2D", "Area3D", "CollisionShape2D", "CollisionShape3D",
	"Camera2D", "Camera3D", "CanvasLayer",
	
	; UI for CRPG interfaces
	"Control", "Panel", "VBoxContainer", "HBoxContainer",
	"Button", "Label", "RichTextLabel", "ItemList",
	"TabContainer", "ScrollContainer", "LineEdit", "TextEdit",
	
	; Audio for narrative
	"AudioStreamPlayer", "AudioStreamPlayer2D", "AudioStreamPlayer3D",
	"AudioStreamOggVorbis", "AudioStreamMP3",
	
	; Resources
	"Resource", "PackedScene", "Texture2D", "ImageTexture",
	"Material", "ShaderMaterial", "StandardMaterial3D"
])

; Dock configuration for CRPG workflow
dock_1_enabled = true  ; Scene dock
dock_2_enabled = true  ; Import dock  
dock_3_enabled = true  ; Node dock
dock_4_enabled = true  ; Groups/Connections dock
dock_5_enabled = false ; Remote inspector (not needed)
dock_6_enabled = true  ; FileSystem dock

; Feature flags
feature_3d = false      ; Disable advanced 3D (keep basic 3D for isometric)
feature_script = true   ; Keep scripting
feature_asset_lib = true ; Asset library for CRPG assets
feature_scene_tree = true 
feature_node_dock = true
feature_filesystem_dock = true
feature_import_dock = true
feature_history_dock = false ; Simplify interface

; Custom feature description
description = "Palimpsest CRPG Profile - Optimized for Disco Elysium style narrative RPGs with consciousness mechanics, bureaucratic themes, and procedural generation."