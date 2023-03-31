cmake_minimum_required(VERSION 3.14...3.26)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)

include(cmake/submodule.cmake)
include(cmake/ccache.cmake)
include(cmake/clangformat.cmake)

download_submodules()

project(VortexRenderer VERSION 0.1.0
    DESCRIPTION "A cross platform renderer supported multiple rendering APIs"
    LANGUAGES CXX)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_CXX_STANDARD, C++17)
    set(CMAKE_CXX_STANDARD_REQUIRED, ON)
    set(CMAKE_CXX_EXTENSIONS OFF)

    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    set(XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    include(CTest)

    find_package(Doxygen)

    if(Doxygen_FOUND)
        add_subdirectory(docs)
    else()
        message(STATUS "Doxygen not found, not building docs")
    endif()
endif()

set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
add_subdirectory(extern/glfw)
add_subdirectory(extern/glm)
add_subdirectory(extern/glad)
add_subdirectory(extern/stb_image)
add_subdirectory(src)
add_subdirectory(apps)
