cmake_minimum_required(VERSION 3.15...3.26.2)

set(VORTEX_BUILD_DEMO ON)
set(VORTEX_BUILD_DOCS ON)

project(Vortex VERSION 0.1.0
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
    set(CMAKE_CXX_VISIBILITY_PRESET hidden)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

if(VORTEX_BUILD_DOCS)
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
add_subdirectory(extern/assimp)
add_subdirectory(extern/glad)
add_subdirectory(extern/stb_image)
add_subdirectory(src)

if(VORTEX_BUILD_DEMO)
    add_subdirectory(apps)
endif()
