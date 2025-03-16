#include "python_bridge.h"
#include "core/os/os.h"
#include "core/string_builder.h"

void PythonBridge::_bind_methods() {
    ClassDB::bind_method(D_METHOD("call_function", "module_name", "function_name", "args"), &PythonBridge::call_function);
    ClassDB::bind_method(D_METHOD("import_module", "module_name"), &PythonBridge::import_module);
    ClassDB::bind_method(D_METHOD("execute_string", "code"), &PythonBridge::execute_string);
    ClassDB::bind_method(D_METHOD("get_last_error"), &PythonBridge::get_last_error);
    ClassDB::bind_method(D_METHOD("clear_error"), &PythonBridge::clear_error);
    ClassDB::bind_method(D_METHOD("get_loaded_modules"), &PythonBridge::get_loaded_modules);
    ClassDB::bind_method(D_METHOD("reload_module", "module_name"), &PythonBridge::reload_module);
    ClassDB::bind_method(D_METHOD("get_variable", "module_name", "variable_name"), &PythonBridge::get_variable);
    ClassDB::bind_method(D_METHOD("set_variable", "module_name", "variable_name", "value"), &PythonBridge::set_variable);
}

PythonBridge::PythonBridge() : initialized(false), main_module(nullptr), main_dict(nullptr) {
    initialize_python();
}

PythonBridge::~PythonBridge() {
    finalize_python();
}

void PythonBridge::initialize_python() {
    if (!initialized) {
        Py_Initialize();
        main_module = PyImport_AddModule("__main__");
        main_dict = PyModule_GetDict(main_module);
        initialized = true;
    }
}

void PythonBridge::finalize_python() {
    if (initialized) {
        Py_Finalize();
        initialized = false;
        main_module = nullptr;
        main_dict = nullptr;
    }
}

Variant PythonBridge::call_function(const String& module_name, const String& function_name, const Array& args) {
    if (!initialized) return Variant();

    PyObject* module = PyImport_ImportModule(module_name.utf8().get_data());
    if (!module) {
        return Variant();
    }

    PyObject* func = PyObject_GetAttrString(module, function_name.utf8().get_data());
    if (!func || !PyCallable_Check(func)) {
        Py_XDECREF(func);
        Py_DECREF(module);
        return Variant();
    }

    PyObject* args_tuple = PyTuple_New(args.size());
    for (int i = 0; i < args.size(); i++) {
        PyObject* arg = variant_to_python(args[i]);
        PyTuple_SetItem(args_tuple, i, arg);
    }

    PyObject* result = PyObject_CallObject(func, args_tuple);
    Py_DECREF(args_tuple);
    Py_DECREF(func);
    Py_DECREF(module);

    if (result) {
        Variant ret = python_to_variant(result);
        Py_DECREF(result);
        return ret;
    }

    return Variant();
}

bool PythonBridge::import_module(const String& module_name) {
    if (!initialized) return false;

    PyObject* module = PyImport_ImportModule(module_name.utf8().get_data());
    if (!module) {
        return false;
    }

    Py_DECREF(module);
    return true;
}

bool PythonBridge::execute_string(const String& code) {
    if (!initialized) return false;

    PyObject* result = PyRun_String(code.utf8().get_data(), Py_file_input, main_dict, main_dict);
    if (!result) {
        return false;
    }

    Py_DECREF(result);
    return true;
}

String PythonBridge::get_last_error() {
    if (!initialized) return String();

    PyObject* type, *value, *traceback;
    PyErr_Fetch(&type, &value, &traceback);
    
    if (!value) return String();

    PyObject* str_value = PyObject_Str(value);
    String error_msg = String(PyUnicode_AsUTF8(str_value));

    Py_XDECREF(str_value);
    Py_XDECREF(type);
    Py_XDECREF(value);
    Py_XDECREF(traceback);

    return error_msg;
}

void PythonBridge::clear_error() {
    if (initialized) {
        PyErr_Clear();
    }
}

Array PythonBridge::get_loaded_modules() {
    Array modules;
    if (!initialized) return modules;

    PyObject* sys_modules = PyImport_GetModuleDict();
    PyObject* keys = PyDict_Keys(sys_modules);
    
    Py_ssize_t size = PyList_Size(keys);
    for (Py_ssize_t i = 0; i < size; i++) {
        PyObject* key = PyList_GetItem(keys, i);
        modules.push_back(String(PyUnicode_AsUTF8(key)));
    }

    Py_DECREF(keys);
    return modules;
}

bool PythonBridge::reload_module(const String& module_name) {
    if (!initialized) return false;

    PyObject* module = PyImport_ImportModule(module_name.utf8().get_data());
    if (!module) return false;

    PyObject* reloaded_module = PyImport_ReloadModule(module);
    Py_DECREF(module);

    if (!reloaded_module) return false;

    Py_DECREF(reloaded_module);
    return true;
}

Variant PythonBridge::get_variable(const String& module_name, const String& variable_name) {
    if (!initialized) return Variant();

    PyObject* module = PyImport_ImportModule(module_name.utf8().get_data());
    if (!module) return Variant();

    PyObject* var = PyObject_GetAttrString(module, variable_name.utf8().get_data());
    Py_DECREF(module);

    if (!var) return Variant();

    Variant result = python_to_variant(var);
    Py_DECREF(var);
    return result;
}

bool PythonBridge::set_variable(const String& module_name, const String& variable_name, const Variant& value) {
    if (!initialized) return false;

    PyObject* module = PyImport_ImportModule(module_name.utf8().get_data());
    if (!module) return false;

    PyObject* py_value = variant_to_python(value);
    int result = PyObject_SetAttrString(module, variable_name.utf8().get_data(), py_value);
    
    Py_DECREF(py_value);
    Py_DECREF(module);

    return result == 0;
}

Variant PythonBridge::python_to_variant(PyObject* obj) {
    if (!obj) return Variant();

    if (PyLong_Check(obj)) {
        return Variant(PyLong_AsLong(obj));
    }
    else if (PyFloat_Check(obj)) {
        return Variant(PyFloat_AsDouble(obj));
    }
    else if (PyUnicode_Check(obj)) {
        return Variant(String(PyUnicode_AsUTF8(obj)));
    }
    else if (PyList_Check(obj)) {
        Array arr;
        Py_ssize_t size = PyList_Size(obj);
        for (Py_ssize_t i = 0; i < size; i++) {
            arr.push_back(python_to_variant(PyList_GetItem(obj, i)));
        }
        return arr;
    }
    else if (PyDict_Check(obj)) {
        Dictionary dict;
        PyObject* keys = PyDict_Keys(obj);
        Py_ssize_t size = PyList_Size(keys);
        for (Py_ssize_t i = 0; i < size; i++) {
            PyObject* key = PyList_GetItem(keys, i);
            PyObject* value = PyDict_GetItem(obj, key);
            dict[python_to_variant(key)] = python_to_variant(value);
        }
        Py_DECREF(keys);
        return dict;
    }
    else if (PyBool_Check(obj)) {
        return Variant(obj == Py_True);
    }
    else if (obj == Py_None) {
        return Variant();
    }

    return Variant();
}

PyObject* PythonBridge::variant_to_python(const Variant& var) {
    switch (var.get_type()) {
        case Variant::NIL:
            Py_RETURN_NONE;
        case Variant::BOOL:
            return PyBool_FromLong(var.operator bool());
        case Variant::INT:
            return PyLong_FromLong(var.operator int64_t());
        case Variant::REAL:
            return PyFloat_FromDouble(var.operator double());
        case Variant::STRING: {
            String str = var.operator String();
            return PyUnicode_FromString(str.utf8().get_data());
        }
        case Variant::ARRAY: {
            Array arr = var;
            PyObject* list = PyList_New(arr.size());
            for (int i = 0; i < arr.size(); i++) {
                PyList_SetItem(list, i, variant_to_python(arr[i]));
            }
            return list;
        }
        case Variant::DICTIONARY: {
            Dictionary dict = var;
            PyObject* py_dict = PyDict_New();
            Array keys = dict.keys();
            for (int i = 0; i < keys.size(); i++) {
                PyDict_SetItem(py_dict,
                    variant_to_python(keys[i]),
                    variant_to_python(dict[keys[i]]));
            }
            return py_dict;
        }
        default:
            Py_RETURN_NONE;
    }
} 