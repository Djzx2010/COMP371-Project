//
// Created by jash on 15/04/19.
//

#ifndef OPENISS_STATUS_H
#define OPENISS_STATUS_H

namespace openiss
{
    /**
     * Represents various OpenISS method status codes.
     * Largerly based on NITE2, OpenNI2 and other middleware codes.
     * @author Jashanjot Singh
     * @author Serguei Mokhov
     */
    enum class OIStatus : int
    {
        // Generic status
        STATUS_DEFAULT = 0,

        // No problem or error
        //  nite::Status::STATUS_OK
        //  openni::Status::STATUS_OK
        //  tdv::nuitrack::OK
        //  k4a_result_t::K4A_RESULT_SUCCEEDED
        STATUS_OK,

        // Method or function nto implemented yet
        //  openni::Status::STATUS_NOT_IMPLEMENTED
        STATUS_NOT_IMPLEMENTED, // OpenNI2

        // Feature not supported or possible
        //  openni::Status::STATUS_NOT_SUPPORTED
        STATUS_NOT_SUPPORTED,   // OpenNI2

        // No usable device connected
        //  openni::Status::STATUS_NO_DEVICE
        STATUS_NO_DEVICE,       // OpenNI2

        // Tracker not initialized tet
        STATUS_TRACKER_NOT_INITIALIZED,

        // Error has happened
        //  nite::Status::STATUS_ERROR
        //  openni::Status::STATUS_ERROR
        //  tdv::nuitrack::EXCEPTION
        //  tdv::nuitrack::TERMINATE_EXCEPTION
        //  tdv::nuitrack::LICENSE_NOT_ACQUIRED_EXCEPTION
        //  k4a_result_t::K4A_RESULT_FAILED
        STATUS_FAIL,
        STATUS_ERROR = STATUS_FAIL, // NITE2/OpenNI2

        //  nite::Status::STATUS_BAD_USER_ID
        STATUS_BAD_USER_ID, // NiTE2

        // Problem with the memory, device, or stack 
        //  nite::Status::STATUS_OUT_OF_FLOW
        //  openni::Status::STATUS_OUT_OF_FLOW
        STATUS_OUT_OF_FLOW, // NiTE2/OpenNI2

        // For configuration objects
        //  tdv::nuitrack::BAD_CONFIG_VALUE_EXCEPTION
        BAD_CONFIG_VALUE,

        //  tdv::nuitrack::CONFIG_NOT_FOUND_EXCEPTION
        CONFIG_NOT_FOUND,

        //  tdv::nuitrack::MODUDLE_NOT_FOUND_EXCEPTION
        MODUDLE_NOT_FOUND,
        
        //  tdv::nuitrack::MODULE_NOT_INITIALIZED_EXCEPTION
        MODULE_NOT_INITIALIZED,

        //  tdv::nuitrack::MODULE_NOT_STARTED_EXCEPTION
        MODULE_NOT_STARTED
    };
}

#endif // OPENISS_STATUS_H
