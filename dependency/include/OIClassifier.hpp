#ifndef OPENISS_CLASSIFIER_H
#define OPENISS_CLASSIFIER_H

namespace openiss
{

/**
 * The root of the OpenISS classifier framework.
 * Classifiers can be detectors, predictors, identifiers, etc.
 * 
 * @see classifiers directory
 * @author Serguei Mokhov
 */
class OIClassifier
{
public:
    virtual ~OIClassifier() = default;

    // TODO make use of OIStatus type instead
    virtual int init() = 0;

    // TODO, argument OIFrame or similar
    // predict()
    // detect()
    // classify()
};

} // namespace

#endif // OPENISS_CLASSIFIER_H

// EOF
