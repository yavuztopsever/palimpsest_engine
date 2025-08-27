# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is **Palimpsest Engine**, a specialized CRPG game engine based on Godot, designed for creating Disco Elysium-style narrative games with consciousness mechanics, German Expressionist aesthetics, and bureaucratic themes. 

Palimpsest Engine is a heavily customized fork of the Godot Engine that removes unnecessary features while adding specialized systems for:
- **Consciousness-responsive environments** that warp based on character mental states
- **Advanced dialogue systems** with skill checks and narrative branching  
- **Procedural generation** using Wave Function Collapse with consciousness constraints
- **German Expressionist rendering** with monochrome palette and red accents
- **Bureaucratic gameplay mechanics** featuring forms, stamps, and paperwork

The engine maintains Godot's MIT license while adding Palimpsest-specific modules for CRPG development.

## Build System

Godot uses **SCons** (Software Construction tool) as its build system, not traditional Makefiles or CMake.

### Essential Build Commands

```bash
# Build editor (default target)
scons platform=<platform>

# Build export templates
scons platform=<platform> target=template_release
scons platform=<platform> target=template_debug

# Platform examples (auto-detected if not specified):
scons platform=linuxbsd    # Linux/BSD
scons platform=windows     # Windows
scons platform=macos       # macOS

# Development builds with debugging
scons platform=<platform> dev_build=yes

# Fast incremental builds
scons platform=<platform> fast_unsafe=yes

# Clean build
scons --clean

# Build with tests
scons platform=<platform> tests=yes

# Show all build options
scons --help
```

### Key Build Options
- `target`: `editor` (default), `template_release`, `template_debug`
- `arch`: `auto` (default), `x86_64`, `arm64`, etc.
- `dev_build=yes`: Enable developer build with debugging
- `tests=yes`: Build unit tests
- `verbose=yes`: Show detailed build output
- `optimize`: `auto`, `none`, `debug`, `speed`, `size`
- `debug_symbols=yes`: Include debug symbols

## Testing

### Running Tests
```bash
# Build and run tests
scons platform=<platform> tests=yes
./bin/godot.<platform>.editor.dev.<arch> --test

# Run specific test suites
./bin/godot.<platform>.editor.dev.<arch> --test --test-case="*[Math]*"

# With doctest options
./bin/godot.<platform>.editor.dev.<arch> --test --help
```

Tests are located in the `tests/` directory and use the doctest framework. Test files follow the pattern `test_*.h`.

## Code Architecture

### Core Structure
- `core/`: Engine core systems (math, I/O, memory, variant system, object system)
  - `math/`: Vector math, transforms, geometry utilities
  - `io/`: File access, networking, serialization
  - `variant/`: Dynamic type system and containers
  - `object/`: Base object system, signals, method binding
  - `string/`: String handling, localization
  - `templates/`: Container classes and data structures

- `scene/`: Scene system, nodes, and high-level game objects
  - `2d/`: 2D-specific nodes (sprites, physics bodies, etc.)
  - `3d/`: 3D-specific nodes (meshes, cameras, lighting)
  - `gui/`: User interface controls
  - `animation/`: Animation system
  - `resources/`: Resource management system

- `servers/`: Low-level rendering, physics, and audio backends
  - `rendering/`: Graphics rendering backend
  - `physics_2d/` & `physics_3d/`: Physics simulation
  - `audio/`: Audio processing and output

- `editor/`: Godot editor implementation
  - `plugins/`: Editor plugin system
  - `import/`: Asset import pipeline

- `drivers/`: Platform-specific drivers (graphics, audio, input)
- `platform/`: Platform-specific implementations
- `modules/`: Optional/pluggable engine modules
- `thirdparty/`: External libraries

### Module System
Godot uses a modular architecture where features can be enabled/disabled:
```bash
# Disable specific modules
scons platform=<platform> module_<name>_enabled=no

# Custom modules
scons platform=<platform> custom_modules=path/to/modules
```

### Memory Management
- Uses custom memory allocators and object pooling
- Reference counting via `RefCounted` class
- Manual memory management with `memdelete()` and `memnew()`

## Development Workflow

### Code Style
- Follow the project's [Code Style Guidelines](https://docs.godotengine.org/en/latest/contributing/development/code_style_guidelines.html)
- Use tabs for indentation, spaces for alignment
- Class names use PascalCase, methods use snake_case
- File names use snake_case with appropriate extensions

### Important Files
- `SConstruct`: Main SCons build configuration
- `version.py`: Version information
- `methods.py`: Build helper methods
- Individual `SCsub` files: Module-specific build configurations

### Platform Support
The engine supports multiple platforms via the `platform/` directory:
- `linuxbsd/`: Linux and BSD variants
- `windows/`: Windows (MinGW and MSVC)
- `macos/`: macOS
- `android/`: Android
- `ios/`: iOS
- `web/`: Web (Emscripten)

### Performance Considerations
- Enable `scu_build=yes` for faster compilation via single compilation units
- Use `fast_unsafe=yes` for faster incremental builds during development
- Build with `optimize=speed` for release builds

## Common Development Tasks

### Adding New Features
1. Check if feature requires a proposal in [godot-proposals](https://github.com/godotengine/godot-proposals)
2. Implement in appropriate core/scene/server directory
3. Add unit tests in `tests/`
4. Update class documentation if exposing new APIs
5. Follow the project's contribution guidelines

### Debugging
- Build with `dev_build=yes debug_symbols=yes`
- Use GDB/LLDB for native debugging
- Enable verbose output with `verbose=yes`
- Check `misc/scripts/` for debugging utilities

### Documentation
- Class reference is generated from XML files in `doc/classes/`
- Update documentation when adding/modifying public APIs
- Use `--doctool` to generate class reference

This project requires significant C++ expertise and understanding of game engine architecture. Always refer to the extensive documentation at https://docs.godotengine.org for detailed information about engine internals and development practices.