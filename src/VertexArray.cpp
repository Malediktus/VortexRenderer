#include <Vortex/Core.hpp>
#include <Vortex/Platform/OpenGL/OpenGLVertexArray.hpp>
#include <Vortex/VertexArray.hpp>
#include <Vortex/Renderer.hpp>

using namespace Vortex;

std::shared_ptr<VertexArray> Vortex::VertexArrayCreate() {
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        return std::make_shared<OpenGL::OpenGLVertexArray>();
    default:
        assert(false);
    }
    assert(false);
}
