def can_build(env, platform):
    # Palimpsest can be built on all supported platforms
    return True

def configure(env):
    # Configure environment for Palimpsest module
    pass

def get_doc_classes():
    return [
        # Narrative System
        "DialogueNode",
        "DialogueManager", 
        "QuestNode",
        "QuestTracker",
        "SkillCheckNode",
        
        # Consciousness System
        "ConsciousnessField",
        "RealityDistortion",
        "WarpZone",
        "MemoryFragment",
        "PalimpsestEffect",
        "FormProcessor",
        "RealityAnchor",
        
        # Procedural Generation
        "WFCGenerator",
        "TileConstraint",
        "ConsciousnessArchitecture",
        "BrutalistGenerator",
        "DistortionModifier",
        "ExpressionistMaterial",
        
        # RPG Systems
        "InventoryContainer",
        "ItemDatabase",
        "StatManager",
        "SkillTreeNode",
        "ThoughtNode",
        "InternalizationSystem",
        
        # Renderer
        "PaletteEnforcer",
        "MonochromeConverter",
    ]

def get_doc_path():
    return "doc_classes"