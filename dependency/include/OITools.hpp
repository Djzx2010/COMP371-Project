#ifndef OPENISS_TOOLS_H
#define OPENISS_TOOLS_H

#include <iostream>

#ifdef OPENISS_OPENNI2_SUPPORT
#include <OpenNI.h>
#endif

namespace openiss
{

class OIUtilities
{
public:
#ifdef OPENISS_OPENNI2_SUPPORT
    static void checkStatus(openni::Status status, const char* msg)
    {
        if (status != openni::STATUS_OK)
        {
            std::cerr << msg << std::endl;
            std::cerr << openni::OpenNI::getExtendedError() << std::endl;
            exit(1);
        }
    };
#endif

#ifdef OPENISS_NITE2_SUPPORT
    static void checkStatus(nite::Status status, const char* msg)
    {
        if (status != nite::STATUS_OK)
        {
            std::cerr << msg << std::endl;
            exit(1);
        }
    };
#endif

    // TODO: from Jash's gesture trackers
    static void printPoint3f(float p_x, float p_y, float p_z)
    {
        printf("%f, %f, %f \n", p_x, p_y, p_z);
    }
};

} // namespace

#endif // OPENISS_TOOLS_H

