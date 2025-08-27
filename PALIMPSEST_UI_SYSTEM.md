# Palimpsest Engine UI System

## Overview

The Palimpsest Engine features a specialized user interface designed for CRPG development, particularly for Disco Elysium-style narrative games. The UI system transforms Godot's general-purpose editor into a focused CRPG development environment.

## Key Components

### 1. PalimpsestEditorNode

The core editor node that extends Godot's EditorNode with CRPG-specific functionality:

- **Custom Toolbar**: Replaces generic editor buttons with CRPG workflow buttons
- **Palimpsest Menu**: Adds specialized menu for consciousness settings, bureaucracy templates, and narrative tools
- **Status Indicators**: Shows consciousness field count, narrative branch, and bureaucracy complexity
- **Workspace Management**: Coordinates with workspace manager for optimal layout

**Location**: `editor/palimpsest/palimpsest_editor_node.h/.cpp`

### 2. PalimpsestWorkspaceManager

Manages 5 specialized workspaces optimized for different CRPG development tasks:

#### Workspaces:

1. **Narrative Design**
   - Maximized dialogue graph editor
   - Quest tracker and thought cabinet
   - Hidden 3D/animation tools
   - Focus on DialogueNode, QuestNode, ThoughtNode

2. **Consciousness Tuning**
   - Consciousness field painter
   - Reality distortion preview
   - Emotional state debugger
   - Focus on ConsciousnessField, RealityDistortion nodes

3. **Bureaucracy Design**
   - Form designer and preview
   - Paperwork flow visualizer
   - Red tape complexity meter
   - Focus on FormNode, StampValidator, PaperworkContainer

4. **World Building**
   - Isometric level design
   - WFC generator controls
   - Brutalist architecture presets
   - Focus on WFCGenerator, BrutalistTile nodes

5. **Testing & Debug**
   - Thought cabinet testing
   - Skill check simulation
   - Narrative debugger
   - All node types visible

**Location**: `editor/palimpsest/palimpsest_workspace_manager.h/.cpp`

### 3. PalimpsestThemeManager

Implements German Expressionist visual theme with:

- **Monochrome Color Palette**: Dark grays with high contrast text
- **Selective Red Accents**: Blood red (#8B0000) for critical elements
- **Angular Styling**: Sharp, modernist UI elements
- **Atmospheric Effects**: Optional film grain and distortion

**Color Scheme**:
- Primary Background: #1a1a1a (very dark gray)
- Secondary Background: #282828 (panels)
- Tertiary Background: #333333 (input fields)
- Primary Text: #e0e0e0 (high contrast)
- Accent Red: #8B0000 (blood red)
- Success Green: #4a5a4a (desaturated)
- Warning Amber: #806030 (muted)

**Location**: `editor/palimpsest/palimpsest_theme_manager.h/.cpp`

### 4. PalimpsestNodeCreator

Custom node creation dialog that filters available nodes based on current workspace:

#### Node Categories:
- **Narrative**: DialogueNode, QuestNode, ThoughtNode, ChoiceNode
- **Consciousness**: ConsciousnessField, RealityDistortion, MemoryFragment
- **Bureaucracy**: FormNode, StampValidator, PaperworkContainer
- **World Building**: WFCGenerator, BrutalistTile, PaletteEnforcer
- **Essential**: Core Godot nodes (Node, Control, Label, Button, etc.)

Features:
- Search filtering
- Category filtering
- Workspace-specific node visibility
- Rich descriptions for each node type

**Location**: `editor/palimpsest/palimpsest_node_creator.h/.cpp`

## Integration with Godot Editor

### Automatic Detection

The system automatically detects Palimpsest projects by checking for:
1. `palimpsest/consciousness/global_influence` project setting
2. `palimpsest_crpg.profile` file
3. `modules/palimpsest` directory
4. "Palimpsest" or "CRPG" in project name

### Build Integration

The UI components are built as a separate static library:
- **SCsub**: `editor/palimpsest/SCsub`
- **Integration**: `palimpsest_editor_integration.h/.cpp`
- **Registration**: Automatic ClassDB registration

## Usage

### Activation

The Palimpsest UI automatically activates when opening a detected Palimpsest project:

1. Editor starts with German Expressionist theme
2. Specialized toolbars and menus appear
3. Default workspace is "Narrative Design"
4. Node creation dialog filters to CRPG-relevant nodes

### Workspace Switching

Switch between workspaces using:
- Toolbar buttons (Narrative, Consciousness, Bureaucracy, World, Testing)
- Keyboard shortcuts (Ctrl+1 through Ctrl+5)
- Palimpsest menu → Workspace submenu

### Customization

#### Theme Customization
```gdscript
var theme_manager = PalimpsestThemeManager.get_singleton()
theme_manager.set_color(PalimpsestThemeManager.COLOR_ACCENT_RED, Color.CRIMSON)
theme_manager.set_ui_scale(1.2)
theme_manager.set_distortion_effects_enabled(true)
```

#### Workspace Customization
```gdscript
var workspace_manager = palimpsest_editor.get_workspace_manager()
workspace_manager.set_dock_visibility("ConsciousnessFieldPainter", false)
workspace_manager.save_workspace_layout(WORKSPACE_NARRATIVE, "my_custom_layout")
```

## Benefits

### For CRPG Developers

1. **Focused Workflow**: Removes distractions, emphasizes narrative tools
2. **Context Awareness**: UI adapts to current development task
3. **Thematic Consistency**: Editor matches game's aesthetic
4. **Efficiency**: Quick access to CRPG-specific features
5. **Professional Quality**: Built on proven Godot UI framework

### For Narrative Designers

- Dialogue graph editor prominently displayed
- Quick access to thought cabinet and quest tracker
- Skill check probability calculator
- Yarn/Ink import/export tools

### For Technical Artists

- German Expressionist styling tools
- Consciousness field visualization
- Reality distortion preview
- Palette enforcement controls

## Technical Architecture

### Class Hierarchy
```
EditorNode (Godot)
└── PalimpsestEditorNode (Palimpsest)
    ├── PalimpsestWorkspaceManager
    ├── PalimpsestThemeManager
    └── PalimpsestNodeCreator
```

### Event Flow
1. Editor startup → Palimpsest detection
2. Theme application → UI reorganization
3. Workspace switching → Dock rearrangement
4. Node creation → Filtered dialog
5. Status updates → Real-time indicators

### Memory Management
- RefCounted base classes for managers
- Automatic cleanup on editor shutdown
- Efficient theme resource caching
- Minimal memory footprint

## Future Enhancements

### Planned Features
- Custom shader effects for consciousness distortion
- Animated transitions between workspaces
- Voice-over integration for dialogue testing
- Advanced bureaucratic form validation
- Procedural soundtrack integration

### Extension Points
- Custom workspace creation API
- Plugin system for additional CRPG tools
- Theme modification scripting
- Integration with external narrative tools

## Compatibility

- **Godot Version**: 4.3+ (built on current Godot master)
- **Platform**: All platforms supported by Godot
- **Dependencies**: None beyond standard Godot editor
- **Performance**: Minimal overhead, efficient resource usage

## Troubleshooting

### Common Issues

**UI not appearing**: Ensure project has Palimpsest detection markers
**Theme not applying**: Check PalimpsestThemeManager singleton initialization
**Workspace not switching**: Verify dock visibility settings
**Nodes not filtered**: Check workspace manager integration

### Debug Mode
Enable verbose logging for detailed information:
```
./bin/godot.editor --verbose
```

This will show Palimpsest UI initialization steps and any issues encountered.

## Conclusion

The Palimpsest UI system transforms Godot into a specialized CRPG development environment while maintaining professional quality and usability. It demonstrates how game engines can be customized for specific genres while preserving their underlying power and flexibility.