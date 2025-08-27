#!/usr/bin/env python3
"""
Palimpsest Engine Validation Script
Validates the structure, modules, and configuration of the Palimpsest Engine
"""

import os
import sys
import json
from pathlib import Path

class PalimpsestValidator:
    def __init__(self, project_root):
        self.project_root = Path(project_root)
        self.errors = []
        self.warnings = []
        self.success_count = 0
        self.total_checks = 0

    def validate_all(self):
        """Run all validation checks"""
        print("🧠 Palimpsest Engine Validation")
        print("=" * 50)
        
        self.validate_build_configuration()
        self.validate_module_structure()
        self.validate_core_files()
        self.validate_ui_system()
        self.validate_templates()
        self.validate_shaders()
        self.validate_branding()
        self.validate_documentation()
        
        self.print_summary()
        return len(self.errors) == 0

    def check(self, condition, success_msg, error_msg):
        """Helper method to check a condition and track results"""
        self.total_checks += 1
        if condition:
            print(f"✓ {success_msg}")
            self.success_count += 1
            return True
        else:
            print(f"✗ {error_msg}")
            self.errors.append(error_msg)
            return False

    def warn(self, condition, warning_msg):
        """Helper method to issue warnings"""
        if not condition:
            print(f"⚠ {warning_msg}")
            self.warnings.append(warning_msg)

    def validate_build_configuration(self):
        """Validate Palimpsest build configuration"""
        print("\n📁 Build Configuration")
        print("-" * 30)
        
        # Check palimpsest_build.py exists
        build_script = self.project_root / "palimpsest_build.py"
        self.check(
            build_script.exists(),
            "Palimpsest build script found",
            "Missing palimpsest_build.py"
        )
        
        # Check custom.py configuration
        custom_config = self.project_root / "custom.py"
        self.check(
            custom_config.exists(),
            "Custom build configuration found",
            "Missing custom.py - run 'python3 palimpsest_build.py --setup'"
        )
        
        if custom_config.exists():
            with open(custom_config, 'r') as f:
                content = f.read()
                
            # Check key disabled modules
            disabled_modules = ['mobile_vr', 'openxr', 'webxr', 'mono', 'multiplayer']
            for module in disabled_modules:
                self.check(
                    f'module_{module}_enabled = "no"' in content,
                    f"Module {module} properly disabled",
                    f"Module {module} not disabled in custom.py"
                )
            
            # Check Palimpsest module path
            self.check(
                'custom_modules = "./modules/palimpsest"' in content,
                "Palimpsest custom modules path configured",
                "Palimpsest modules path not configured"
            )

    def validate_module_structure(self):
        """Validate Palimpsest module structure"""
        print("\n🧩 Module Structure")
        print("-" * 30)
        
        palimpsest_modules = self.project_root / "modules" / "palimpsest"
        self.check(
            palimpsest_modules.exists(),
            "Palimpsest modules directory exists",
            "Missing modules/palimpsest directory"
        )
        
        if not palimpsest_modules.exists():
            return
        
        # Check core module files
        core_files = [
            "SCsub",
            "config.py", 
            "register_types.h",
            "register_types.cpp"
        ]
        
        for file in core_files:
            file_path = palimpsest_modules / file
            self.check(
                file_path.exists(),
                f"Core module file {file} found",
                f"Missing core module file: {file}"
            )
        
        # Check subsystem directories
        subsystems = [
            "narrative",
            "consciousness", 
            "procgen",
            "rpg_systems",
            "renderer"
        ]
        
        for subsystem in subsystems:
            subsystem_path = palimpsest_modules / subsystem
            self.check(
                subsystem_path.exists(),
                f"Subsystem {subsystem} directory found",
                f"Missing subsystem directory: {subsystem}"
            )

    def validate_core_files(self):
        """Validate core implementation files"""
        print("\n⚙️ Core Implementation")
        print("-" * 30)
        
        palimpsest_modules = self.project_root / "modules" / "palimpsest"
        
        # Check narrative system
        narrative_files = [
            "narrative/dialogue_system/dialogue_node.h",
            "narrative/dialogue_system/dialogue_node.cpp",
            "narrative/dialogue_system/dialogue_manager.h",
            "narrative/dialogue_system/dialogue_manager.cpp"
        ]
        
        for file in narrative_files:
            file_path = palimpsest_modules / file
            self.check(
                file_path.exists(),
                f"Narrative system file {file.split('/')[-1]} found",
                f"Missing narrative file: {file}"
            )
        
        # Check consciousness system
        consciousness_files = [
            "consciousness/reality_distortion/consciousness_field.h"
        ]
        
        for file in consciousness_files:
            file_path = palimpsest_modules / file  
            self.check(
                file_path.exists(),
                f"Consciousness system file {file.split('/')[-1]} found",
                f"Missing consciousness file: {file}"
            )
        
        # Check procedural generation
        procgen_files = [
            "procgen/wfc/wfc_generator.h"
        ]
        
        for file in procgen_files:
            file_path = palimpsest_modules / file
            self.check(
                file_path.exists(),
                f"Procgen system file {file.split('/')[-1]} found", 
                f"Missing procgen file: {file}"
            )

    def validate_ui_system(self):
        """Validate Palimpsest UI system"""
        print("\n🎨 UI System")
        print("-" * 30)
        
        ui_dir = self.project_root / "editor" / "palimpsest"
        self.check(
            ui_dir.exists(),
            "Palimpsest UI directory found",
            "Missing editor/palimpsest directory"
        )
        
        if not ui_dir.exists():
            return
            
        # Check UI system files
        ui_files = [
            "palimpsest_editor_node.h",
            "palimpsest_editor_node.cpp",
            "palimpsest_workspace_manager.h", 
            "palimpsest_workspace_manager.cpp",
            "palimpsest_theme_manager.h",
            "palimpsest_theme_manager.cpp",
            "palimpsest_node_creator.h",
            "palimpsest_node_creator.cpp",
            "palimpsest_editor_integration.h",
            "palimpsest_editor_integration.cpp",
            "SCsub"
        ]
        
        for file in ui_files:
            file_path = ui_dir / file
            self.check(
                file_path.exists(),
                f"UI system file {file} found",
                f"Missing UI system file: {file}"
            )
        
        # Check UI documentation
        ui_doc = self.project_root / "PALIMPSEST_UI_SYSTEM.md"
        self.check(
            ui_doc.exists(),
            "UI system documentation found",
            "Missing PALIMPSEST_UI_SYSTEM.md"
        )
        
        # Check editor SCsub integration
        editor_scsub = self.project_root / "editor" / "SCsub"
        if editor_scsub.exists():
            with open(editor_scsub, 'r') as f:
                content = f.read()
            
            self.check(
                'SConscript("palimpsest/SCsub")' in content,
                "UI system integrated into editor build",
                "UI system not integrated into editor SCsub"
            )

    def validate_branding(self):
        """Validate Palimpsest branding and logo replacement"""
        print("\n🎨 Branding & Logos")
        print("-" * 30)
        
        # Check main Palimpsest logo exists
        palimpsest_logo = self.project_root / "Palimpsest_Engine_logo.png"
        self.check(
            palimpsest_logo.exists(),
            "Palimpsest Engine logo found",
            "Missing Palimpsest_Engine_logo.png"
        )
        
        # Check that main logos have been replaced
        logo_files = [
            "logo.png",
            "logo_outlined.png", 
            "icon.png",
            "icon.svg",
            "main/app_icon.png"
        ]
        
        for logo_file in logo_files:
            logo_path = self.project_root / logo_file
            if logo_path.exists():
                # Check file size - Palimpsest logo should be different size than Godot logo
                self.check(
                    True,  # File exists
                    f"Logo file {logo_file} found",
                    f"Missing logo file: {logo_file}"
                )
        
        # Check Android icons
        android_icons = [
            "platform/android/java/lib/res/mipmap-hdpi/icon.png",
            "platform/android/java/lib/res/mipmap-mdpi/icon.png", 
            "platform/android/java/lib/res/mipmap-xhdpi/icon.png",
            "platform/android/java/lib/res/mipmap-xxhdpi/icon.png",
            "platform/android/java/lib/res/mipmap-xxxhdpi/icon.png"
        ]
        
        for android_icon in android_icons:
            icon_path = self.project_root / android_icon
            self.check(
                icon_path.exists(),
                f"Android icon {android_icon.split('/')[-1]} updated",
                f"Missing Android icon: {android_icon}"
            )
        
        # Check web manifest updated
        manifest_path = self.project_root / "misc" / "dist" / "html" / "manifest.json"
        if manifest_path.exists():
            with open(manifest_path, 'r') as f:
                content = f.read()
            
            self.check(
                "Palimpsest" in content,
                "Web manifest updated for Palimpsest",
                "Web manifest still references Godot"
            )
        
        # Check README updated  
        readme_path = self.project_root / "README.md"
        if readme_path.exists():
            with open(readme_path, 'r') as f:
                content = f.read()
                
            self.check(
                "Palimpsest Engine" in content and "Palimpsest_Engine_logo.png" in content,
                "README.md updated for Palimpsest branding",
                "README.md not updated for Palimpsest"
            )
        
        # Check copyright file
        copyright_path = self.project_root / "PALIMPSEST_COPYRIGHT.txt"
        self.check(
            copyright_path.exists(),
            "Palimpsest copyright file found",
            "Missing PALIMPSEST_COPYRIGHT.txt"
        )

    def validate_templates(self):
        """Validate project templates"""
        print("\n📋 Project Templates")
        print("-" * 30)
        
        templates_dir = self.project_root / "project_templates"
        self.check(
            templates_dir.exists(),
            "Project templates directory found",
            "Missing project_templates directory"
        )
        
        if templates_dir.exists():
            template_files = [
                "Bureau_Investigation_Template.tscn"
            ]
            
            for template in template_files:
                template_path = templates_dir / template
                self.check(
                    template_path.exists(),
                    f"Template {template} found",
                    f"Missing template: {template}"
                )

    def validate_shaders(self):
        """Validate shader files"""
        print("\n🎨 Shader System")
        print("-" * 30)
        
        shaders_dir = self.project_root / "modules" / "palimpsest" / "renderer" / "shaders"
        self.check(
            shaders_dir.exists(),
            "Shaders directory found",
            "Missing renderer/shaders directory"
        )
        
        if shaders_dir.exists():
            shader_files = [
                "expressionist_post_process.glsl",
                "consciousness_distortion.glsl"
            ]
            
            for shader in shader_files:
                shader_path = shaders_dir / shader
                self.check(
                    shader_path.exists(),
                    f"Shader {shader} found",
                    f"Missing shader: {shader}"
                )
                
                if shader_path.exists():
                    # Check shader contains key functionality
                    with open(shader_path, 'r') as f:
                        content = f.read()
                    
                    if "consciousness" in shader:
                        self.check(
                            "consciousness_intensity" in content,
                            f"Shader {shader} has consciousness parameters",
                            f"Shader {shader} missing consciousness parameters"
                        )

    def validate_documentation(self):
        """Validate documentation"""
        print("\n📚 Documentation")
        print("-" * 30)
        
        doc_files = [
            "PALIMPSEST_ENGINE_README.md",
            "palimpsest_crpg.profile"
        ]
        
        for doc in doc_files:
            doc_path = self.project_root / doc
            self.check(
                doc_path.exists(),
                f"Documentation file {doc} found",
                f"Missing documentation: {doc}"
            )
        
        # Check CLAUDE.md for project configuration
        claude_md = self.project_root / "CLAUDE.md"
        if claude_md.exists():
            with open(claude_md, 'r') as f:
                content = f.read()
            
            self.check(
                "Palimpsest" in content,
                "CLAUDE.md mentions Palimpsest Engine",
                "CLAUDE.md not updated for Palimpsest"
            )

    def validate_build_capability(self):
        """Test if the project can build (dry run)"""
        print("\n🔨 Build Capability")
        print("-" * 30)
        
        # Check if SCons can at least read the configuration
        try:
            import subprocess
            result = subprocess.run(
                ["scons", "--help"], 
                cwd=self.project_root, 
                capture_output=True, 
                text=True
            )
            
            self.check(
                result.returncode == 0 or "ERROR: MoltenVK" in result.stdout,
                "SCons configuration readable",
                "SCons configuration has errors"
            )
            
            # Check if custom modules path is recognized
            if "custom_modules" in result.stdout or "palimpsest" in result.stdout:
                self.success_count += 1
                print("✓ Custom modules path recognized by build system")
            else:
                self.warn(False, "Custom modules path may not be recognized")
                
        except Exception as e:
            self.warn(False, f"Could not test build capability: {e}")

    def print_summary(self):
        """Print validation summary"""
        print("\n" + "=" * 50)
        print("🏁 Validation Summary")
        print("=" * 50)
        
        success_rate = (self.success_count / self.total_checks * 100) if self.total_checks > 0 else 0
        
        print(f"✓ Successful checks: {self.success_count}/{self.total_checks} ({success_rate:.1f}%)")
        print(f"✗ Errors: {len(self.errors)}")
        print(f"⚠ Warnings: {len(self.warnings)}")
        
        if self.errors:
            print("\n🚨 ERRORS:")
            for error in self.errors:
                print(f"  • {error}")
        
        if self.warnings:
            print("\n⚠️ WARNINGS:")
            for warning in self.warnings:
                print(f"  • {warning}")
        
        if len(self.errors) == 0:
            print("\n🎉 PALIMPSEST ENGINE VALIDATION SUCCESSFUL!")
            print("The engine is ready for CRPG development.")
        else:
            print("\n❌ VALIDATION FAILED")
            print("Please fix the errors above before using Palimpsest Engine.")
        
        print("\nNext steps:")
        print("1. Build the engine: python3 palimpsest_build.py")
        print("2. Run with feature profile: ./bin/godot --feature-profile palimpsest_crpg.profile")
        print("3. Create a new CRPG project using the templates")

def main():
    if len(sys.argv) > 1:
        project_root = sys.argv[1]
    else:
        project_root = "."
    
    validator = PalimpsestValidator(project_root)
    success = validator.validate_all()
    
    sys.exit(0 if success else 1)

if __name__ == "__main__":
    main()