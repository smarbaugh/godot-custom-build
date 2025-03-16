#!/usr/bin/env python

# Configuration file for custom Godot build with C#, Python, and Uiverse.io integration

def can_build(env, platform):
    return True

def configure(env):
    # Enable required modules
    env['module_mono_enabled'] = 'yes'
    env['module_python_enabled'] = 'yes'
    env['module_uiverse_enabled'] = 'yes'

    # C# configuration
    env['mono_static'] = 'no'
    env['tools_enabled'] = 'yes'
    env['module_mono_config_file'] = 'modules/mono/config.py'

    # Python configuration
    env['python_version'] = '3.8'
    env['python_lib'] = 'python3'
    env['python_include'] = ''  # Will be set by the Python module's SCsub

    # Uiverse.io configuration
    env['uiverse_api_url'] = 'https://uiverse.io/api'
    env['uiverse_cache_dir'] = 'user://uiverse_cache'

def get_doc_classes():
    return [
        "PythonBridge",
        "UiverseComponent"
    ]

def get_doc_path():
    return "doc_classes" 