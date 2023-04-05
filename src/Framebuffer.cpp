#include <Vortex/Framebuffer.hpp>
#include <Vortex/Platform/OpenGL/OpenGLFramebuffer.hpp>
#include <Vortex/Renderer.hpp>

using namespace Vortex;

std::shared_ptr<Framebuffer> Vortex::FramebufferCreate() {
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        return std::make_shared<OpenGL::OpenGLFramebuffer>();
    default:
        assert(false);
    }
    assert(false);
}
