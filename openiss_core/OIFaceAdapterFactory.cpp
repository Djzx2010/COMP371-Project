//
// Created by Yiran Shen on 2019-03-10
//

#include "OIFaceAdapter.hpp"
#include "OINullFaceTracker.hpp"
//#include "OINullDFaceTracker.hpp"

#ifdef OPENISS_OPENFACE_SUPPORT
#include "OIOpenFaceAdapter.hpp"
#endif

#include <cstring>
#include <iostream>

openiss::OIFaceAdapterFactory::~OIFaceAdapterFactory()
{

}

std::shared_ptr<openiss::OIFaceAdapter>
openiss::OIFaceAdapterFactory::createAdapter(const char* adapterName, OIDevice* pDev)
{
#ifdef OPENISS_OPENFACE_SUPPORT
    if(strcmp(adapterName, "openface") == 0)
    {
        if(!mIsOpenFaceInit)
        {
            mIsOpenFaceInit = true;
        }

        std::shared_ptr<OIFaceAdapter> adapter(new OIOpenFaceAdapter(*pDev));
        return adapter;
    }
#endif

    if(adapterName == nullptr || strcmp(adapterName, "null") == 0)
    {
        std::shared_ptr<OIFaceAdapter> adapter(new OINullFaceTracker(*pDev));
        return adapter;
    }

    /*
    //Adding a check for dynamic face tracker
    if (adapterName == nullptr || strcmp(adapterName, "nullD") == 0)
    {
        std::shared_ptr<OIFaceAdapter> adapter(new OINullDFaceTracker());
        return adapter;
    }
    */

    std::cerr
        << "cannot create any Face tracker/detector adapter ("
        << adapterName
        << ")"
        << std::endl;

    //exit(1);
    return nullptr;
}
