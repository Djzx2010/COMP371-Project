#ifndef OPENISS_PYTHON_ENV_H
#define OPENISS_PYTHON_ENV_H

#include <Python.h>

#include <string>
#include <unordered_map>
#include <vector>

#include "Config.h"

namespace openiss {
class OIPythonEnv{
public:
    OIPythonEnv();
    ~OIPythonEnv();

    void initPyWorkingPath(std::vector<std::string> paths);
    void showWokingPath();
    void importPyModule(char *name);
    PyObject* getPyModule(char *name);
    PyObject* createPyInstance(char *moduleName, char *className, const char *format);
    PyObject* loadPyMethod(PyObject *callerName, char *funcName);
    PyObject* invokePyMethod(PyObject *callerName, PyObject *args);

private:
    // a map maintain the imported module
    std::unordered_map<char*, PyObject*> modules;

    bool containsModule(char *name);
    void addModules(char* name, PyObject *module);
    void removeModules(char *name);
};
} // end of namespace

#endif // OPENISS_PYTHON_ENV_H
