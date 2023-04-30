set(HEADER_LIST
    "${PROJECT_SOURCE_DIR}/include/Vortex/Vortex.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Core.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Platform.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Context.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/RenderCommand.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Renderer.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/RendererAPI.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Shader.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Camera.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Buffer.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Texture.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/VertexArray.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Framebuffer.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Renderbuffer.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Scene/Scene.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Scene/Mesh.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Scene/Model.hpp")

set(SOURCE_LIST
    Context.cpp
    Shader.cpp
    Buffer.cpp
    Camera.cpp
    Texture.cpp
    VertexArray.cpp
    Renderer.cpp
    RenderCommand.cpp
    RendererAPI.cpp
    Framebuffer.cpp
    Renderbuffer.cpp
    Asserts.cpp
    Scene/Mesh.cpp
    Scene/Model.cpp)

set(OPENGL_HEADERS
    "${PROJECT_SOURCE_DIR}/include/Vortex/Platform/OpenGL/OpenGLShader.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Platform/OpenGL/OpenGLContext.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Platform/OpenGL/OpenGLBuffer.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Platform/OpenGL/OpenGLTexture.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Platform/OpenGL/OpenGLVertexArray.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Platform/OpenGL/OpenGLRendererAPI.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Platform/OpenGL/OpenGLFramebuffer.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Platform/OpenGL/OpenGLRenderbuffer.hpp")

set(VULKAN_HEADERS
    "${PROJECT_SOURCE_DIR}/include/Vortex/Platform/Vulkan/VulkanShader.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Platform/Vulkan/VulkanContext.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Platform/Vulkan/VulkanBuffer.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Platform/Vulkan/VulkanTexture.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Platform/Vulkan/VulkanVertexArray.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Platform/Vulkan/VulkanRendererAPI.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Platform/Vulkan/VulkanFramebuffer.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Platform/Vulkan/VulkanRenderbuffer.hpp")

set(OPENGL_SOURCES
    Platform/OpenGL/OpenGLContext.cpp
    Platform/OpenGL/OpenGLShader.cpp
    Platform/OpenGL/OpenGLBuffer.cpp
    Platform/OpenGL/OpenGLTexture.cpp
    Platform/OpenGL/OpenGLVertexArray.cpp
    Platform/OpenGL/OpenGLRendererAPI.cpp
    Platform/OpenGL/OpenGLFramebuffer.cpp
    Platform/OpenGL/OpenGLRenderbuffer.cpp)

set(VULKAN_SOURCES
    Platform/Vulkan/VulkanContext.cpp
    Platform/Vulkan/VulkanShader.cpp
    Platform/Vulkan/VulkanBuffer.cpp
    Platform/Vulkan/VulkanTexture.cpp
    Platform/Vulkan/VulkanVertexArray.cpp
    Platform/Vulkan/VulkanRendererAPI.cpp
    Platform/Vulkan/VulkanFramebuffer.cpp
    Platform/Vulkan/VulkanRenderbuffer.cpp)

add_library(Vortex ${HEADER_LIST} ${SOURCE_LIST} ${OPENGL_SOURCES} ${OPENGL_HEADERS} ${VULKAN_SOURCES} ${VULKAN_HEADERS})

target_include_directories(Vortex PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_definitions(Vortex PRIVATE VT_EXPORT_API)
target_compile_definitions(Vortex PRIVATE $<$<CONFIG:Debug>:VT_DEBUG>)
target_compile_definitions(Vortex PRIVATE $<$<CONFIG:Release>:VT_RELEASE>)
target_compile_features(Vortex PUBLIC cxx_std_17)

if(MSVC)
    target_compile_options(Vortex PRIVATE /W4 /WX)
else()
    target_compile_options(Vortex PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_link_libraries(Vortex PUBLIC glm TracyClient)
target_link_libraries(Vortex PRIVATE glfw glad Vulkan::Vulkan stb_image assimp spdlog::spdlog)

source_group(
    TREE "${PROJECT_SOURCE_DIR}/include/Vortex"
    PREFIX "Header Files"
    FILES ${HEADER_LIST})

source_group(
    TREE "${PROJECT_SOURCE_DIR}/src"
    PREFIX "Source Files"
    FILES ${SOURCE_LIST})

source_group(
    TREE "${PROJECT_SOURCE_DIR}/include/Vortex/Platform/OpenGL"
    PREFIX "Header Files/OpenGL"
    FILES ${OPENGL_HEADERS})

source_group(
    TREE "${PROJECT_SOURCE_DIR}/src/Platform/OpenGL"
    PREFIX "Source Files/OpenGL"
    FILES ${OPENGL_SOURCES})

source_group(
    TREE "${PROJECT_SOURCE_DIR}/include/Vortex/Platform/Vulkan"
    PREFIX "Header Files/Vulkan"
    FILES ${VULKAN_HEADERS})

source_group(
    TREE "${PROJECT_SOURCE_DIR}/src/Platform/Vulkan"
    PREFIX "Source Files/Vulkan"
    FILES ${VULKAN_SOURCES})

add_library(Vortex::Vortex ALIAS Vortex)
