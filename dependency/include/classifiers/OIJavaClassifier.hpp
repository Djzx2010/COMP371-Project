#ifndef OPENISS_JAVA_CLASSIFER_H
#define OPENISS_JAVA_CLASSIFER_H

#include "OIClassifier.hpp"

namespace openiss
{

/**
 * Classifiers based on Java wrappers would inherit this.
 * E.g., MARF or Weka or similar.
 * @author Serguei Mokhov
 */
class OIJavaClassifier : public OIClassifier
{
public:
    OIJavaClassifier();
    virtual ~OIJavaClassifier() = default;

protected:
    // TODO

};

} // namespace

#endif // OPENISS_JAVA_CLASSIFER_H

// EOF
