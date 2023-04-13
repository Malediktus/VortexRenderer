#include <Vortex/Renderbuffer.hpp>

#include <Vortex/Platform/OpenGL/OpenGLRenderbuffer.hpp>
#include <Vortex/Renderer.hpp>
#include <tracy/Tracy.hpp>

using namespace Vortex;

std::shared_ptr<Renderbuffer> Vortex::RenderbufferCreate(uint32_t width, uint32_t height, Renderbuffer::RenderbufferType type) {
    ZoneScoped;
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        return std::make_shared<OpenGL::OpenGLRenderbuffer>(width, height, type);
    default:
        assert(false);
    }
    assert(false);
}
