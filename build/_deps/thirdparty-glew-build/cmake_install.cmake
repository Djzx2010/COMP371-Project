# Install script for directory: C:/Users/STRIX/Desktop/School/University/Winter 2021/COMP 371 - Computer Graphics/Assignment/build/_deps/thirdparty-glew-src/build/cmake

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/STRIX/Desktop/School/University/Winter 2021/COMP 371 - Computer Graphics/Assignment/dist")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/STRIX/Desktop/School/University/Winter 2021/COMP 371 - Computer Graphics/Assignment/build/lib/Debug/glew32d.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/STRIX/Desktop/School/University/Winter 2021/COMP 371 - Computer Graphics/Assignment/build/bin/Debug/glew32d.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/STRIX/Desktop/School/University/Winter 2021/COMP 371 - Computer Graphics/Assignment/build/lib/Debug/libglew32d.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/STRIX/Desktop/School/University/Winter 2021/COMP 371 - Computer Graphics/Assignment/build/bin/Debug/glewinfo.exe")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/STRIX/Desktop/School/University/Winter 2021/COMP 371 - Computer Graphics/Assignment/build/bin/Debug/visualinfo.exe")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "C:/Users/STRIX/Desktop/School/University/Winter 2021/COMP 371 - Computer Graphics/Assignment/build/_deps/thirdparty-glew-src/build/cmake/../../glew.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "C:/Users/STRIX/Desktop/School/University/Winter 2021/COMP 371 - Computer Graphics/Assignment/build/bin/Debug/glew32d.pdb")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/GL" TYPE FILE FILES
    "C:/Users/STRIX/Desktop/School/University/Winter 2021/COMP 371 - Computer Graphics/Assignment/build/_deps/thirdparty-glew-src/build/cmake/../../include/GL/wglew.h"
    "C:/Users/STRIX/Desktop/School/University/Winter 2021/COMP 371 - Computer Graphics/Assignment/build/_deps/thirdparty-glew-src/build/cmake/../../include/GL/glew.h"
    "C:/Users/STRIX/Desktop/School/University/Winter 2021/COMP 371 - Computer Graphics/Assignment/build/_deps/thirdparty-glew-src/build/cmake/../../include/GL/glxew.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/glew/glew-targets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/glew/glew-targets.cmake"
         "C:/Users/STRIX/Desktop/School/University/Winter 2021/COMP 371 - Computer Graphics/Assignment/build/_deps/thirdparty-glew-build/CMakeFiles/Export/lib/cmake/glew/glew-targets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/glew/glew-targets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/glew/glew-targets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/glew" TYPE FILE FILES "C:/Users/STRIX/Desktop/School/University/Winter 2021/COMP 371 - Computer Graphics/Assignment/build/_deps/thirdparty-glew-build/CMakeFiles/Export/lib/cmake/glew/glew-targets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/glew" TYPE FILE FILES "C:/Users/STRIX/Desktop/School/University/Winter 2021/COMP 371 - Computer Graphics/Assignment/build/_deps/thirdparty-glew-build/CMakeFiles/Export/lib/cmake/glew/glew-targets-debug.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/glew" TYPE FILE FILES
    "C:/Users/STRIX/Desktop/School/University/Winter 2021/COMP 371 - Computer Graphics/Assignment/build/_deps/thirdparty-glew-src/build/cmake/glew-config.cmake"
    "C:/Users/STRIX/Desktop/School/University/Winter 2021/COMP 371 - Computer Graphics/Assignment/build/_deps/thirdparty-glew-src/build/cmake/CopyImportedTargetProperties.cmake"
    )
endif()

