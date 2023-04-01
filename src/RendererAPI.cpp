#include <Vortex/RendererAPI.hpp>
#include <Vortex/Renderer.hpp>
#include <Vortex/Platform/OpenGL/OpenGLRendererAPI.hpp>
#include <cassert>

using namespace Vortex;

RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

std::shared_ptr<RendererAPI> Vortex::RendererAPICreate() {
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        return std::make_shared<OpenGL::OpenGLRendererAPI>();
    default:
        assert(false);
    }
    assert(false);
}
