#!/usr/bin/env python

import os
import sys
import subprocess
import platform
import shutil
from pathlib import Path

def check_python():
    """Check Python version and required packages."""
    print("Checking Python installation...")
    
    if sys.version_info < (3, 8):
        print("Error: Python 3.8 or higher is required")
        sys.exit(1)
    
    try:
        import pip
    except ImportError:
        print("Error: pip is not installed")
        sys.exit(1)

    required_packages = [
        'setuptools',
        'wheel',
        'SCons'
    ]

    for package in required_packages:
        try:
            __import__(package)
        except ImportError:
            print(f"Installing {package}...")
            subprocess.check_call([sys.executable, '-m', 'pip', 'install', package])

def check_mono():
    """Check Mono installation."""
    print("Checking Mono installation...")
    
    if platform.system() == "Windows":
        mono_path = shutil.which("mono")
        msbuild_path = shutil.which("msbuild")
        
        if not mono_path or not msbuild_path:
            print("Error: Mono and MSBuild are required. Please install the .NET SDK and Mono.")
            print("Download from: https://www.mono-project.com/download/stable/")
            sys.exit(1)
    else:
        try:
            subprocess.check_call(['mono', '--version'], stdout=subprocess.DEVNULL)
        except (subprocess.CalledProcessError, FileNotFoundError):
            print("Error: Mono is not installed")
            if platform.system() == "Linux":
                print("Install using: sudo apt-get install mono-complete")
            elif platform.system() == "Darwin":
                print("Install using: brew install mono")
            sys.exit(1)

def setup_godot():
    """Clone and set up Godot repository."""
    print("Setting up Godot repository...")
    
    if not os.path.exists("godot"):
        subprocess.check_call(['git', 'clone', 'https://github.com/godotengine/godot.git', 'godot'])
        
    os.chdir("godot")
    subprocess.check_call(['git', 'checkout', '3.x'])
    os.chdir("..")

def copy_modules():
    """Copy custom modules to Godot."""
    print("Installing custom modules...")
    
    godot_modules = Path("godot/modules")
    if not godot_modules.exists():
        godot_modules.mkdir(parents=True)

    # Copy our custom modules
    custom_modules = ["python", "uiverse"]
    for module in custom_modules:
        src = Path(f"modules/{module}")
        dst = godot_modules / module
        
        if dst.exists():
            shutil.rmtree(dst)
        shutil.copytree(src, dst)

def setup_build_environment():
    """Set up the build environment."""
    print("Setting up build environment...")
    
    # Copy custom build configuration
    shutil.copy2("custom.py", "godot/custom.py")
    
    # Create build directory
    if not os.path.exists("build"):
        os.makedirs("build")

def main():
    print("Setting up development environment for custom Godot build...")
    
    check_python()
    check_mono()
    setup_godot()
    copy_modules()
    setup_build_environment()
    
    print("""
Setup complete! You can now build the custom Godot engine:

Windows:
    scons platform=windows target=release_debug bits=64

Linux:
    scons platform=linux target=release_debug bits=64

macOS:
    scons platform=osx target=release_debug arch=x86_64

Add the following parameters to enable specific features:
    use_mono=yes
    use_python=yes
    use_uiverse=yes
""")

if __name__ == "__main__":
    main() 