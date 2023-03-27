cmake_minimum_required(VERSION 3.14...3.26)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/submodule.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/ccache.cmake")

download_submodules()

project(VortexRenderer VERSION 0.1.0
    DESCRIPTION "A cross platform renderer supported multiple rendering APIs"
    LANGUAGES CXX)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_CXX_STANDARD, C++17)
    set(CMAKE_CXX_STANDARD_REQUIRED, ON)
    set(CMAKE_CXX_EXTENSIONS OFF)

    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    include(CTest)

    find_package(Doxygen)

    if(Doxygen_FOUND)
        add_subdirectory(docs)
    else()
        message(STATUS "Doxygen not found, not building docs")
    endif()
endif()

add_subdirectory(src)
add_subdirectory(apps)
