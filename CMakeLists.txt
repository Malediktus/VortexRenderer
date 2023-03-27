cmake_minimum_required(VERSION 3.14...3.26)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)

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

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw
)

FetchContent_GetProperties(glfw)

if(NOT glfw_POPULATED)
    FetchContent_Populate(glfw)

    set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
    set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
    set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
    set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")

    add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
endif()

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm
    GIT_TAG master
)

FetchContent_MakeAvailable(glm)

add_subdirectory(extern/Glad)
add_subdirectory(src)
add_subdirectory(apps)
