# Custom Godot Features Documentation

This document describes the custom features added to this Godot build, including C# as the primary scripting language, Python integration, and Uiverse.io component support.

## C# Integration

C# is configured as the primary scripting language in this custom build. All standard Godot C# features are available, with additional improvements for stability and performance.

### Setting Up C# Projects

1. Create a new project in Godot
2. Enable C# support in Project Settings
3. Create a new C# script:
   ```csharp
   public class MyNode : Node
   {
       public override void _Ready()
       {
           GD.Print("Hello from C#!");
       }
   }
   ```

### Best Practices
- Use the latest .NET features supported by Mono
- Follow C# coding conventions
- Utilize Godot's built-in C# features and types

## Python Integration

Python support is provided through the `PythonBridge` class, allowing seamless integration between C# and Python code.

### Using Python in Your Project

1. Create a Python script (e.g., `my_script.py`):
   ```python
   def calculate_something(value):
       return value * 2
   ```

2. Call Python from C#:
   ```csharp
   public class MyNode : Node
   {
       private PythonBridge pythonBridge;

       public override void _Ready()
       {
           pythonBridge = new PythonBridge();
           
           // Import the Python module
           pythonBridge.ImportModule("my_script");
           
           // Call a Python function
           var result = pythonBridge.CallFunction(
               "my_script",
               "calculate_something",
               new Array { 42 }
           );
           
           GD.Print($"Result from Python: {result}");
       }
   }
   ```

### Python Bridge API

#### Core Functionality
- `ImportModule(string moduleName)`: Import a Python module
- `CallFunction(string moduleName, string functionName, Array args)`: Call a Python function
- `ExecuteString(string code)`: Execute Python code directly
- `GetVariable(string moduleName, string variableName)`: Get a Python variable's value
- `SetVariable(string moduleName, string variableName, Variant value)`: Set a Python variable's value

#### Error Handling
- `GetLastError()`: Get the last Python error message
- `ClearError()`: Clear the error state

#### Module Management
- `GetLoadedModules()`: Get a list of loaded Python modules
- `ReloadModule(string moduleName)`: Reload a Python module

## Uiverse.io Integration

The Uiverse.io integration allows you to use UI components from uiverse.io directly in your Godot projects.

### Using Uiverse Components

1. Add a UiverseComponent node to your scene
2. Set the component ID or provide HTML/CSS content directly
3. Customize the appearance using style overrides
4. Handle interaction events as needed

### Example Usage

```csharp
public class MyUI : Control
{
    private UiverseComponent uiverseButton;

    public override void _Ready()
    {
        uiverseButton = GetNode<UiverseComponent>("UiverseButton");
        
        // Load a component from uiverse.io
        uiverseButton.LoadFromUiverse("button-123");
        
        // Or set content directly
        uiverseButton.HtmlContent = "<button class='my-button'>Click Me</button>";
        uiverseButton.CssContent = ".my-button { background: blue; }";
        
        // Add style overrides
        var styles = new Godot.Collections.Dictionary();
        styles["background_color"] = Colors.Blue;
        styles["border_radius"] = 10;
        uiverseButton.StyleOverrides = styles;
        
        // Connect to interaction events
        uiverseButton.Connect("interaction_event", this, nameof(OnInteraction));
    }

    private void OnInteraction(string eventType)
    {
        GD.Print($"Uiverse component interaction: {eventType}");
    }
}
```

### UiverseComponent Properties

- `ComponentId`: The unique identifier for the uiverse.io component
- `HtmlContent`: Direct HTML content
- `CssContent`: Direct CSS content
- `StyleOverrides`: Dictionary of style overrides
- `Interactable`: Whether the component can receive input

### UiverseComponent Signals

- `component_loaded`: Emitted when a component is successfully loaded
- `component_error`: Emitted when an error occurs
- `interaction_event`: Emitted when the user interacts with the component

## Building from Source

To build this custom version of Godot:

1. Install prerequisites:
   - Python 3.8 or higher
   - Mono/.NET SDK
   - SCons build system
   - C++ compiler (Visual Studio 2022, GCC, or Clang)

2. Run the setup script:
   ```bash
   python tools/setup.py
   ```

3. Build the engine:
   ```bash
   # Windows
   scons platform=windows target=release_debug bits=64 use_mono=yes use_python=yes use_uiverse=yes

   # Linux
   scons platform=linux target=release_debug bits=64 use_mono=yes use_python=yes use_uiverse=yes

   # macOS
   scons platform=osx target=release_debug arch=x86_64 use_mono=yes use_python=yes use_uiverse=yes
   ```

## Troubleshooting

### Common Issues

1. Python Integration
   - Ensure Python 3.8 or higher is installed and in your PATH
   - Check that required Python modules are installed
   - Verify Python script paths are correct

2. C# Integration
   - Verify Mono is properly installed
   - Check .NET SDK version compatibility
   - Ensure NuGet packages are restored

3. Uiverse.io Integration
   - Check internet connectivity for component loading
   - Verify component IDs are correct
   - Check browser console for CSS/HTML errors

### Getting Help

- Check the GitHub repository issues
- Join our Discord community
- Review the API documentation
- Contact the maintainers

## Contributing

We welcome contributions! Please follow these steps:

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Write/update tests
5. Submit a pull request

Please follow our coding standards and include appropriate documentation. 