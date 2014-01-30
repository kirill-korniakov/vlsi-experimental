# - Try to find OpenCV (using pkg-config)
#
# If OpenCV was installed in a non-defaul location, set OpenCV_ROOT
#
# Once done, this will define
#
#  OpenCV_FOUND - system has OpenCV
#  OpenCV_INCLUDE_DIRS - the OpenCV include directories
#  OpenCV_LIBRARIES - link these to use OpenCV
#  OpenCV_LIBRARY_DIRS - the OpenCV library directories
#
# Created: 10 Jan 2012 Dirk Van Haerenborgh<dirk.vanhaerenborgh@hogent.be>

if("${OpenCV_DIR}" STREQUAL "")

if(NOT "${CMAKE_PREFIX_PATH}" STREQUAL "")
    set(ENV{PKG_CONFIG_PATH} "${CMAKE_PREFIX_PATH}/lib/pkgconfig:${CMAKE_PREFIX_PATH}/lib64/pkgconfig:$ENV{PKG_CONFIG_PATH}")
endif(NOT "${CMAKE_PREFIX_PATH}" STREQUAL "")

if(NOT "${OpenCV_ROOT}" STREQUAL "")
    set(ENV{PKG_CONFIG_PATH} "${OpenCV_ROOT}/lib/pkgconfig:${OpenCV_ROOT}/lib64/pkgconfig:$ENV{PKG_CONFIG_PATH}")
endif(NOT "${OpenCV_ROOT}" STREQUAL "")

find_package(PkgConfig REQUIRED)
pkg_check_modules(OpenCV REQUIRED opencv)

# Sometimes pkgconfig returns an empty list of libraries
# In that case, all libs are listed with their absolute path in LDFLAGS
IF( "${OpenCV_LIBRARIES}" STREQUAL "" )
    SET(OpenCV_LIBRARIES ${OpenCV_LDFLAGS})
ENDIF()

#Some more variables to be compatible with OpenCVconfig.cmake
SET(OpenCV_LIBS ${OpenCV_LIBRARIES} )
SET(OpenCV_INCLUDE_DIR ${OpenCV_INCLUDE_DIRS})
SET(OpenCV_LIB_DIR ${OpenCV_LIBRARY_DIRS})
SET(OpenCV_PREFIX ${OpenCV_ROOT})

endif()
