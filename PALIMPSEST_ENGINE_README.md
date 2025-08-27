# Palimpsest Engine: CRPG-Specialized Godot Fork

**A specialized game engine for creating Disco Elysium-style narrative CRPGs with consciousness mechanics, German Expressionist aesthetics, and procedural generation.**

## 🎯 Project Overview

Palimpsest Engine is a heavily customized fork of Godot Engine, designed specifically for creating narrative-rich CRPGs inspired by Disco Elysium. The engine removes unnecessary features while adding specialized systems for consciousness-responsive environments, bureaucratic mechanics, and procedural generation.

### Key Features

- **Consciousness Field System**: Reality distorts based on character emotions and thoughts
- **Advanced Dialogue System**: Disco Elysium-style branching conversations with skill checks
- **Bureaucracy Mechanics**: Forms, stamps, and paperwork as gameplay elements
- **Wave Function Collapse**: Procedural generation with consciousness-driven constraints
- **German Expressionist Renderer**: Monochrome palette with red accents, distorted architecture
- **Specialized Editor**: CRPG-focused interface hiding irrelevant tools

## 🏗️ Architecture Overview

### Core Modules

#### 1. Narrative Systems (`modules/palimpsest/narrative/`)
- **DialogueNode**: Handles branching conversations with consciousness effects
- **DialogueManager**: Coordinates dialogue flow and integrates with Yarn Spinner
- **QuestSystem**: Manages objectives, tracking, and quest state
- **SkillCheckSystem**: Disco Elysium-style attribute-based checks

#### 2. Consciousness Mechanics (`modules/palimpsest/consciousness/`)
- **ConsciousnessField**: Areas that warp reality based on emotional states
- **RealityDistortion**: Physics-defying effects from mental states
- **MemorySystem**: Past events bleeding into present reality
- **BureaucracyAnchoring**: Forms and paperwork stabilizing reality

#### 3. Procedural Generation (`modules/palimpsest/procgen/`)
- **WFCGenerator**: Wave Function Collapse with consciousness integration
- **BuildingGenerator**: Creates warped, expressionist architecture
- **TextureGeneration**: Procedural materials matching art style

#### 4. RPG Systems (`modules/palimpsest/rpg_systems/`)
- **InventoryContainer**: Item management with bureaucratic themes
- **StatManager**: Character attributes affecting reality
- **ThoughtCabinet**: Disco Elysium-style internal thought system

#### 5. Renderer (`modules/palimpsest/renderer/`)
- **PaletteEnforcer**: Restricts colors to monochrome + red
- **DistortionRenderer**: Real-time architectural warping effects

## 🚀 Getting Started

### Prerequisites

- SCons build system
- C++ compiler (GCC, Clang, or MSVC)
- Python 3.8+
- Blender (optional, for asset pipeline)

### Building Palimpsest Engine

1. **Clone the repository:**
   ```bash
   git clone https://github.com/your-org/palimpsest-engine.git
   cd palimpsest-engine
   ```

2. **Set up build configuration:**
   ```bash
   python3 palimpsest_build.py --setup
   ```

3. **Build the engine:**
   ```bash
   # Debug build with Palimpsest features
   python3 palimpsest_build.py
   
   # Release build
   python3 palimpsest_build.py --release
   ```

4. **Run the specialized editor:**
   ```bash
   ./bin/godot.macos.editor.dev.arm64 --feature-profile palimpsest_crpg.profile
   ```

### Disabled Modules (vs Standard Godot)

The following modules are disabled in Palimpsest to reduce complexity and focus on CRPG needs:

- **VR/XR**: `mobile_vr`, `openxr`, `webxr`
- **Multiplayer**: `enet`, `webrtc`, `websocket`, `multiplayer`
- **Advanced 3D**: `lightmapper_rd`, `raycast`, `csg`, `vhacd`
- **Language Bindings**: `mono` (C# scripting)
- **Video/Streaming**: `theora`
- **Network Discovery**: `upnp`
- **Advanced Textures**: `astcenc`, `betsy`, `cvtt`, `etcpak`

### Enabled CRPG-Specific Features

- **Narrative Tools**: Full dialogue and quest management
- **Consciousness Systems**: Reality-warping mechanics
- **Procedural Generation**: WFC with consciousness constraints
- **Expressionist Rendering**: Stylized visual pipeline
- **Bureaucracy Simulation**: Form-based gameplay mechanics

## 📚 Usage Guide

### Creating a Dialogue

```gdscript
# Create a DialogueNode
var dialogue = DialogueNode.new()
dialogue.set_dialogue_id("intro_conversation")

# Set dialogue data
var data = DialogueNode.DialogueData.new()
data.speaker_name = "Detective"
data.dialogue_text = "The fog thickens around your thoughts..."
data.consciousness_effect = 1.5  # Warps reality

# Add response with skill check
var response = DialogueNode.DialogueResponse.new()
response.text = "[Encyclopedia - Medium] You recall the city's history"
response.type = DialogueNode.RESPONSE_SKILL_CHECK
response.skill_requirement = "Encyclopedia"
response.skill_threshold = 12

dialogue.add_response(response)
dialogue.start_dialogue()
```

### Setting up Consciousness Fields

```gdscript
# Create consciousness field that affects architecture
var field = ConsciousnessField.new()
field.set_field_type(ConsciousnessField.FIELD_DEPRESSION)
field.set_intensity(ConsciousnessField.INTENSITY_STRONG)
field.set_field_strength(2.0)

# This will cause nearby buildings to droop and sag
add_child(field)
```

### Procedural Level Generation

```gdscript
# Set up WFC generator
var wfc = WFCGenerator.new()
wfc.set_grid_size(Vector3i(20, 20, 1))
wfc.set_consciousness_influence_enabled(true)

# Define building tiles with consciousness requirements
var tile = WFCGenerator.WFCTile.new()
tile.tile_id = "brutalist_office"
tile.consciousness_influence = -0.5  # Stabilizes reality
tile.required_consciousness_type = "bureaucratic"

wfc.add_tile_definition("office", tile)
wfc.generate()  # Creates a district layout
```

### Custom CRPG Editor Features

The Palimpsest editor includes specialized tools:

- **Dialogue Graph Editor**: Visual conversation flow design
- **Consciousness Field Painter**: Paint emotional effects on environments  
- **WFC Preview Dock**: Real-time procedural generation preview
- **Bureaucracy Designer**: Create forms and stamps as gameplay elements
- **District Builder**: Layout urban areas with consciousness zones

## 🎨 Art Pipeline

### German Expressionist Style Guide

1. **Color Palette**: Strict monochrome with red accents only
2. **Architecture**: No right angles - all lines must be skewed or curved
3. **Lighting**: High contrast chiaroscuro effects
4. **Textures**: Brush-stroke simulation, 4K hand-painted backgrounds

### Consciousness-Responsive Buildings

Buildings in Palimpsest physically respond to character mental states:

- **Depression**: Structures droop and sag downward
- **Anxiety**: Maze-like corridors and confusing layouts
- **Anger**: Sharp, aggressive architectural spikes
- **Memory**: Ghostly overlays of past building states
- **Bureaucracy**: Rigid, perfectly perpendicular geometry (stabilizing)

### Asset Creation Workflow

1. **3D Models**: Create in Blender, import directly as .blend files
2. **Textures**: Use Material Maker for procedural, style-compliant materials
3. **Backgrounds**: Paint in 4K, import as textured planes in 3D space
4. **Architecture**: Design with consciousness response in mind

## 🧠 Consciousness System Deep Dive

The consciousness system is the heart of Palimpsest Engine. It simulates how mental states affect physical reality:

### Field Types

| Field Type | Effect | Visual | Gameplay Impact |
|------------|--------|--------|-----------------|
| Depression | Buildings droop, colors desaturate | Gray fog, sagging walls | Slower movement, blocked passages |
| Anxiety | Labyrinthine geometry | Twisted corridors | Navigation confusion |
| Anger | Sharp edges, red tints | Spikes, hostile architecture | Damage zones, aggressive NPCs |
| Memory | Temporal overlays | Ghostly building states | Past events replay |
| Bureaucratic | Rigid geometry, forms appear | Perfect angles, paperwork | Reality stabilizes, skill bonuses |
| Creative | Impossible geometry | Vibrant colors, M.C. Escher effects | New paths open, rule-breaking |

### Implementation Example

```gdscript
# Character enters depression field
func _on_character_entered_depression_field(field):
    # Apply visual effects
    field.set_base_color_tint(Color.GRAY)
    field.set_fog_intensity_multiplier(2.0)
    
    # Affect nearby buildings
    for building in field.get_affected_objects():
        field.apply_depression_droop(building)
    
    # Gameplay effects
    character.movement_speed *= 0.7
    character.add_thought("The weight of existence...")
```

## 📋 Project Templates

Palimpsest includes project templates for common CRPG scenarios:

### Bureau Investigation Template
- Pre-built office environments with consciousness fields
- Form-processing mini-games
- Bureaucratic NPC dialogue trees
- Evidence collection system

### Memory Palace Template  
- Non-linear, consciousness-driven navigation
- Past/present reality layering
- Temporal distortion mechanics
- Narrative archaeology gameplay

### Urban Decay Template
- Procedurally generated districts
- Economic consciousness effects
- Social stratification mechanics
- Class-based reality perception

## 🔧 Advanced Configuration

### Custom Consciousness Types

```gdscript
# Define new consciousness field type
ConsciousnessField.register_custom_type("nostalgia", {
    "visual_effect": "sepia_tint",
    "architecture_effect": "vintage_overlay", 
    "gameplay_effect": "memory_trigger",
    "propagation_rules": ["amplifies_in_old_buildings"]
})
```

### Consciousness Field Interactions

Fields can merge, conflict, or create cascade events:

```gdscript
# Anger + Depression = Destructive Cascade
if anger_field.overlaps(depression_field):
    create_cascade_event("destructive_spiral")
    # Buildings begin physically crumbling
    
# Bureaucracy + Creativity = Stabilized Innovation  
if bureau_field.can_merge_with_type("creative"):
    bureau_field.merge_with_field(creative_field)
    # Creates "regulated creativity" zones
```

## 🐛 Debugging & Development

### Consciousness Field Visualization

```gdscript
# Enable debug overlay
ConsciousnessField.set_debug_visualization(true)
# Shows field boundaries, intensity gradients, type colors

# Print field statistics
print("Active fields: ", ConsciousnessField.get_active_field_count())
print("Reality stability: ", ConsciousnessField.get_global_stability())
```

### WFC Generation Debug

```gdscript
# Monitor WFC generation
wfc.connect("generation_step", self, "_on_wfc_step")
func _on_wfc_step(step, entropy_map):
    print("Step ", step, " - Avg entropy: ", entropy_map.get_average())
```

### Performance Profiling

Palimpsest includes built-in profiling for consciousness-heavy scenes:

```gdscript
ConsciousnessProfiler.start()
# Run consciousness-intensive scene
ConsciousnessProfiler.stop()
ConsciousnessProfiler.print_report()
```

## 📖 API Reference

### DialogueNode Class

| Method | Description |
|--------|-------------|
| `start_dialogue()` | Begins conversation |
| `select_response(index)` | Chooses dialogue option |
| `add_response(response)` | Adds new dialogue choice |
| `set_variable(name, value)` | Sets dialogue state variable |
| `evaluate_skill_check(skill, threshold)` | Performs attribute check |

### ConsciousnessField Class

| Method | Description |
|--------|-------------|
| `set_field_type(type)` | Sets emotional field type |
| `apply_distortion_to_object(object)` | Warps object based on field |
| `add_consciousness(amount)` | Increases field intensity |
| `is_cascade_imminent()` | Checks for reality breakdown |

### WFCGenerator Class

| Method | Description |
|--------|-------------|
| `generate()` | Runs full WFC generation |
| `generate_step()` | Single WFC iteration |
| `add_tile_definition(id, tile)` | Registers new tile type |
| `set_consciousness_field_value(type, value)` | Influences generation |

## 🤝 Contributing

### Development Guidelines

1. **Maintain CRPG Focus**: All features should serve narrative RPG goals
2. **Consciousness First**: Consider mental state effects in all systems
3. **Bureaucratic Theming**: Emphasize paperwork, forms, administrative processes
4. **Performance Awareness**: Consciousness effects can be expensive
5. **Style Compliance**: Follow German Expressionist aesthetic strictly

### Code Standards

- Use consciousness-aware design patterns
- Include bureaucracy metaphors in naming
- Document reality distortion side effects
- Test with multiple field interactions

### Testing

```bash
# Run Palimpsest-specific tests
python3 palimpsest_build.py --test

# Test consciousness field performance
./run_consciousness_benchmarks.sh

# Validate WFC constraint satisfaction
./test_wfc_correctness.py
```

## 📄 License

Palimpsest Engine inherits Godot's MIT license. Additional Palimpsest-specific modules are also MIT licensed.

## 🙏 Acknowledgments

- **Godot Engine**: Foundation engine and architecture
- **ZA/UM Studio**: Inspiration from Disco Elysium's consciousness mechanics
- **German Expressionist Movement**: Visual and thematic inspiration
- **Wave Function Collapse**: Procedural generation technique
- **Yarn Spinner**: Dialogue system integration

## 📞 Support

- **Documentation**: [Palimpsest Engine Docs](https://docs.palimpsest-engine.org)
- **Community**: [Discord](https://discord.gg/palimpsest-engine) 
- **Issues**: [GitHub Issues](https://github.com/your-org/palimpsest-engine/issues)
- **Discussions**: [GitHub Discussions](https://github.com/your-org/palimpsest-engine/discussions)

---

*"In Palimpsest, every conversation reshapes reality, every form filed anchors existence, and every building remembers the consciousness of those who inhabit it."*