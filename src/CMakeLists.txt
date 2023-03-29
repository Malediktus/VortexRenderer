set(HEADER_LIST
    "${VortexRenderer_SOURCE_DIR}/include/Vortex/Vortex.hpp"
    "${VortexRenderer_SOURCE_DIR}/include/Vortex/Core.hpp"
    "${VortexRenderer_SOURCE_DIR}/include/Vortex/Context.hpp"
    "${VortexRenderer_SOURCE_DIR}/include/Vortex/RenderCommand.hpp"
    "${VortexRenderer_SOURCE_DIR}/include/Vortex/Renderer.hpp"
    "${VortexRenderer_SOURCE_DIR}/include/Vortex/RendererAPI.hpp"
    "${VortexRenderer_SOURCE_DIR}/include/Vortex/Shader.hpp"
    "${VortexRenderer_SOURCE_DIR}/include/Vortex/Buffer.hpp"
    "${VortexRenderer_SOURCE_DIR}/include/Vortex/VertexArray.hpp")

set(OPENGL_HEADERS
    "${VortexRenderer_SOURCE_DIR}/include/Vortex/Platform/OpenGL/OpenGLShader.hpp"
    "${VortexRenderer_SOURCE_DIR}/include/Vortex/Platform/OpenGL/OpenGLContext.hpp"
    "${VortexRenderer_SOURCE_DIR}/include/Vortex/Platform/OpenGL/OpenGLBuffer.hpp"
    "${VortexRenderer_SOURCE_DIR}/include/Vortex/Platform/OpenGL/OpenGLVertexArray.hpp"
    "${VortexRenderer_SOURCE_DIR}/include/Vortex/Platform/OpenGL/OpenGLRendererAPI.hpp")

set(OPENGL_SOURCES
    Platform/OpenGL/OpenGLContext.cpp
    Platform/OpenGL/OpenGLShader.cpp
    Platform/OpenGL/OpenGLBuffer.cpp
    Platform/OpenGL/OpenGLVertexArray.cpp
    Platform/OpenGL/OpenGLRendererAPI.cpp)

add_library(Vortex
    Context.cpp
    Shader.cpp
    Buffer.cpp
    VertexArray.cpp
    Renderer.cpp
    RenderCommand.cpp
    RendererAPI.cpp
    ${OPENGL_SOURCES}
    ${OPENGL_HEADERS}
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

source_group(
    TREE "${PROJECT_SOURCE_DIR}/include/Vortex/Platform/OpenGL"
    PREFIX "OpenGL Headers"
    FILES ${OPENGL_HEADERS})

source_group(
    TREE "${PROJECT_SOURCE_DIR}/src/Platform/OpenGL"
    PREFIX "OpenGL"
    FILES ${OPENGL_SOURCES})
