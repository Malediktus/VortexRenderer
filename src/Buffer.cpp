#include <Vortex/Buffer.hpp>
#include <Vortex/Core.hpp>
#include <Vortex/Renderer.hpp>
#include <Vortex/Platform/OpenGL/OpenGLBuffer.hpp>

using namespace Vortex;

std::shared_ptr<VertexBuffer> Vortex::VertexBufferCreate(float* vertices, uint32_t size) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        return std::make_shared<OpenGL::OpenGLVertexBuffer>(vertices, size);
    default:
        assert(false);
    }
    assert(false);
}

std::shared_ptr<VertexBuffer> Vortex::VertexBufferCreate(uint32_t size) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        return std::make_shared<OpenGL::OpenGLVertexBuffer>(size);
    default:
        assert(false);
    }
    assert(false);
}

std::shared_ptr<IndexBuffer> Vortex::IndexBufferCreate(uint32_t* indices, uint32_t size) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        return std::make_shared<OpenGL::OpenGLIndexBuffer>(indices, size);
    default:
        assert(false);
    }
    assert(false);
}
