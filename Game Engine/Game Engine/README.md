# Custom Godot Build with C#, Python, and Uiverse.io Integration

This is a custom build of the Godot game engine that integrates C# as the primary scripting language, Python as a secondary language, and direct Uiverse.io component integration.

## Features

- **Enhanced C# Support**: Primary scripting language with full .NET feature support
- **Python Integration**: Secondary scripting language with seamless C#/Python interoperability
- **Uiverse.io Components**: Direct integration of Uiverse.io UI elements in the Godot editor
- **Custom Node Types**: Specialized nodes for Uiverse.io components
- **Cross-Platform Support**: Windows, macOS, and Linux compatibility

## Prerequisites

- CMake 3.20 or higher
- Python 3.8 or higher
- .NET SDK 7.0 or higher
- Visual Studio 2022 (Windows) or equivalent C++ compiler
- Mono development tools
- Git

## Build Instructions

### Windows

1. Clone the repository:
   ```powershell
   git clone https://github.com/yourusername/godot-custom-build
   cd godot-custom-build
   ```

2. Initialize submodules:
   ```powershell
   git submodule update --init --recursive
   ```

3. Set up build environment:
   ```powershell
   python tools/setup.py
   ```

4. Build the engine:
   ```powershell
   scons platform=windows target=release_debug bits=64
   ```

### macOS

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/godot-custom-build
   cd godot-custom-build
   ```

2. Initialize submodules:
   ```bash
   git submodule update --init --recursive
   ```

3. Set up build environment:
   ```bash
   python3 tools/setup.py
   ```

4. Build the engine:
   ```bash
   scons platform=osx target=release_debug arch=x86_64
   ```

### Linux

1. Install dependencies:
   ```bash
   sudo apt-get install build-essential scons pkg-config libx11-dev libxcursor-dev libxinerama-dev \
       libgl1-mesa-dev libglu-dev libasound2-dev libpulse-dev libudev-dev libxi-dev libxrandr-dev
   ```

2. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/godot-custom-build
   cd godot-custom-build
   ```

3. Initialize submodules:
   ```bash
   git submodule update --init --recursive
   ```

4. Set up build environment:
   ```bash
   python3 tools/setup.py
   ```

5. Build the engine:
   ```bash
   scons platform=linux target=release_debug bits=64
   ```

## Usage Guide

### C# Scripting

The C# scripting environment is set up automatically during the build process. Create new C# scripts from the Godot editor:

1. Right-click in the FileSystem dock
2. Select "New Script"
3. Choose "C# Script"

### Python Scripting

Python scripts can be created and used alongside C# scripts:

1. Create a new Python script using the custom Python script template
2. Access Python functionality from C# using the `PythonBridge` class
3. Import Python modules using standard Python import syntax

Example C#/Python interop:

```csharp
// C# script
public class MyNode : Node
{
    public override void _Ready()
    {
        // Call Python function from C#
        PythonBridge.CallFunction("my_python_script", "my_function", args);
    }
}
```

```python
# Python script
def my_function(*args):
    # Function implementation
    pass
```

### Uiverse.io Integration

Access Uiverse.io components through the built-in browser:

1. Open the Uiverse.io panel in the editor (View -> Uiverse.io Browser)
2. Browse available components
3. Click "Import" on desired components
4. Drag the imported components from the Scene tree into your game

## Custom Nodes

### UiverseComponent

The `UiverseComponent` node type represents Uiverse.io UI elements:

- Properties:
  - ComponentID: Unique identifier for the Uiverse.io component
  - StyleOverrides: Custom CSS overrides
  - Interactable: Enable/disable user interaction

## Contributing

1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Support

For issues and support:
- Create an issue in the GitHub repository
- Join our Discord community
- Check the FAQ in the wiki

## Acknowledgments

- Godot Engine contributors
- Uiverse.io community
- All contributors to this custom build 