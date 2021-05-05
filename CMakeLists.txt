cmake_minimum_required(VERSION 3.11)

project(COMP371Project)

include(ExternalProject)

# Set install directory
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/dist CACHE PATH ${CMAKE_SOURCE_DIR}/dist FORCE)
set(CMAKE_BUILD_TYPE "Debug")
if(WIN32)
    set(CMAKE_CONFIGURATION_TYPES "Debug" CACHE STRING "Debug" FORCE)
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)

include(BuildGLEW)
include(BuildGLFW)
include(BuildGLM)

# COMP371Assignment
set(EXEC COMP371Assignment)

set(ASSETS assets)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

file(GLOB SRC src/*.cpp)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/dependency/include")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/dependency/openiss_include")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/dependency/openiss_core")

link_directories("${CMAKE_CURRENT_SOURCE_DIR}/dependency/lib/Debug")

add_executable(${EXEC} ${SRC})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT COMP371Assignment)

target_include_directories(${EXEC} PRIVATE include)

target_link_libraries(${EXEC} OpenGL::GL glew_s glfw glm)

target_link_libraries(${EXEC} openiss-static.lib)




list(APPEND BIN ${EXEC})
# end COMP371Assignment

# install files to install location
install(TARGETS ${BIN} DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY ${ASSETS} DESTINATION ${CMAKE_INSTALL_PREFIX})
