#ifndef PYTHON_BRIDGE_H
#define PYTHON_BRIDGE_H

#include "core/object.h"
#include "core/reference.h"
#include <Python.h>

class PythonBridge : public Reference {
    GDCLASS(PythonBridge, Reference);

protected:
    static void _bind_methods();

private:
    PyObject* main_module;
    PyObject* main_dict;
    bool initialized;

    void initialize_python();
    void finalize_python();

public:
    PythonBridge();
    ~PythonBridge();

    // Core functionality
    Variant call_function(const String& module_name, const String& function_name, const Array& args);
    bool import_module(const String& module_name);
    bool execute_string(const String& code);
    
    // Error handling
    String get_last_error();
    void clear_error();

    // Module management
    Array get_loaded_modules();
    bool reload_module(const String& module_name);

    // Variable access
    Variant get_variable(const String& module_name, const String& variable_name);
    bool set_variable(const String& module_name, const String& variable_name, const Variant& value);

    // Type conversion utilities
    static Variant python_to_variant(PyObject* obj);
    static PyObject* variant_to_python(const Variant& var);
};

#endif // PYTHON_BRIDGE_H 