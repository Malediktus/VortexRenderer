set(HEADER_LIST
    "${VortexRenderer_SOURCE_DIR}/include/Vortex/Vortex.hpp"
    "${VortexRenderer_SOURCE_DIR}/include/Vortex/Core.hpp"
    "${VortexRenderer_SOURCE_DIR}/include/Vortex/Context.hpp"
    "${VortexRenderer_SOURCE_DIR}/include/Vortex/Buffer.hpp"
    "${VortexRenderer_SOURCE_DIR}/include/Vortex/VertexArray.hpp"
    "${VortexRenderer_SOURCE_DIR}/include/Vortex/Platform/OpenGL/OpenGLContext.hpp"
    "${VortexRenderer_SOURCE_DIR}/include/Vortex/Platform/OpenGL/OpenGLBuffer.hpp"
    "${VortexRenderer_SOURCE_DIR}/include/Vortex/Platform/OpenGL/OpenGLVertexArray.hpp")

add_library(Vortex
    Context.cpp
    Buffer.cpp
    VertexArray.cpp
    Platform/OpenGL/OpenGLContext.cpp
    Platform/OpenGL/OpenGLBuffer.cpp
    Platform/OpenGL/OpenGLVertexArray.cpp
    ${HEADER_LIST})

target_include_directories(Vortex PUBLIC ../include)
target_compile_definitions(Vortex PRIVATE VT_EXPORT_API)
target_compile_features(Vortex PUBLIC cxx_std_17)
target_link_libraries(Vortex PRIVATE glfw glad)
target_link_libraries(Vortex PUBLIC glm)

source_group(
    TREE "${PROJECT_SOURCE_DIR}/include"
    PREFIX "Header Files"
    FILES ${HEADER_LIST})
