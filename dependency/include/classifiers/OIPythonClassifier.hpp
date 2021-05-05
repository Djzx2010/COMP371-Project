#ifndef OPENISS_PYTHON_CLASSIFER_H
#define OPENISS_PYTHON_CLASSIFER_H

#include "OIClassifier.hpp"
#include "OIPythonEnv.hpp"

namespace openiss
{

/**
 * Classifiers based on Java wrappers would inherit this.
 * E.g., TensorFlow, PyTourch, Caffee or similar.
 * @author Serguei Mokhov
 */
class OIPythonClassifier : public OIClassifier
{
public:
    virtual OIPythonClassifier();
    virtual ~OIPythonClassifier() = default;

protected:
    PyObject* pInstance;
    PyObject* pResult;
    //PyObject* pImageArg1;
    PyObject* pFuncPredict;
    OIPythonEnv env;

};

} // namespace

#endif // OPENISS_PYTHON_CLASSIFER_H

// EOF
