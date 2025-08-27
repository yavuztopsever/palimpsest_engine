/**************************************************************************/
/*  register_types.cpp                                                    */
/**************************************************************************/
/*                         This file is part of:                          */
/*                        PALIMPSEST ENGINE                               */
/*                   Specialized CRPG Engine based on                     */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/

#include "register_types.h"

#include "core/object/class_db.h"
#include "core/config/engine.h"

// Narrative System
#include "narrative/dialogue_system/dialogue_node.h"
#include "narrative/dialogue_system/dialogue_manager.h"
#include "narrative/quest_system/quest_node.h"
#include "narrative/quest_system/quest_tracker.h"
#include "narrative/skill_check_system/skill_check_node.h"

// Consciousness System
#include "consciousness/reality_distortion/consciousness_field.h"
#include "consciousness/reality_distortion/reality_distortion.h"
#include "consciousness/reality_distortion/warp_zone.h"
#include "consciousness/memory_system/memory_fragment.h"
#include "consciousness/memory_system/palimpsest_effect.h"
#include "consciousness/bureaucracy_anchoring/form_processor.h"
#include "consciousness/bureaucracy_anchoring/reality_anchor.h"

// Procedural Generation
#include "procgen/wfc/wfc_generator.h"
#include "procgen/wfc/tile_constraint.h"
#include "procgen/building_generator/consciousness_architecture.h"
#include "procgen/building_generator/brutalist_generator.h"
#include "procgen/building_generator/distortion_modifier.h"
#include "procgen/texture_generation/expressionist_material.h"

// RPG Systems
#include "rpg_systems/inventory/inventory_container.h"
#include "rpg_systems/inventory/item_database.h"
#include "rpg_systems/character_stats/stat_manager.h"
#include "rpg_systems/character_stats/skill_tree_node.h"
#include "rpg_systems/thought_cabinet/thought_node.h"
#include "rpg_systems/thought_cabinet/internalization_system.h"

// Renderer
#include "renderer/palette_enforcer/palette_enforcer.h"
#include "renderer/palette_enforcer/monochrome_converter.h"

void initialize_palimpsest_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	// Register narrative system classes
	GDREGISTER_CLASS(DialogueNode);
	GDREGISTER_CLASS(DialogueManager);
	GDREGISTER_CLASS(QuestNode);
	GDREGISTER_CLASS(QuestTracker);
	GDREGISTER_CLASS(SkillCheckNode);

	// Register consciousness system classes
	GDREGISTER_CLASS(ConsciousnessField);
	GDREGISTER_CLASS(RealityDistortion);
	GDREGISTER_CLASS(WarpZone);
	GDREGISTER_CLASS(MemoryFragment);
	GDREGISTER_CLASS(PalimpsestEffect);
	GDREGISTER_CLASS(FormProcessor);
	GDREGISTER_CLASS(RealityAnchor);

	// Register procedural generation classes
	GDREGISTER_CLASS(WFCGenerator);
	GDREGISTER_CLASS(TileConstraint);
	GDREGISTER_CLASS(ConsciousnessArchitecture);
	GDREGISTER_CLASS(BrutalistGenerator);
	GDREGISTER_CLASS(DistortionModifier);
	GDREGISTER_CLASS(ExpressionistMaterial);

	// Register RPG system classes
	GDREGISTER_CLASS(InventoryContainer);
	GDREGISTER_CLASS(ItemDatabase);
	GDREGISTER_CLASS(StatManager);
	GDREGISTER_CLASS(SkillTreeNode);
	GDREGISTER_CLASS(ThoughtNode);
	GDREGISTER_CLASS(InternalizationSystem);

	// Register renderer classes
	GDREGISTER_CLASS(PaletteEnforcer);
	GDREGISTER_CLASS(MonochromeConverter);

	// Register Palimpsest engine info (note: add_singleton_info method not available)
	// Engine::get_singleton()->add_singleton_info("Palimpsest", "Engine specialized for Disco Elysium-style CRPGs");
}

void uninitialize_palimpsest_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	// Module cleanup if needed
}