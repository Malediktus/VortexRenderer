#include <Vortex/Framebuffer.hpp>
#include <Vortex/Platform/OpenGL/OpenGLFramebuffer.hpp>
#include <Vortex/Renderer.hpp>
#include <tracy/Tracy.hpp>

using namespace Vortex;

std::shared_ptr<Framebuffer> Vortex::FramebufferCreate() {
    ZoneScoped;
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
<<<<<<< Updated upstream
        return std::make_shared<OpenGL::OpenGLFramebuffer>();
=======
        return std::make_shared<OpenGL::OpenGLFramebuffer>(window);
>>>>>>> Stashed changes
    default:
        VT_ASSERT_CHECK(false, "Invalid renderer API value returned from Renderer::GetRendererAPI()");
    }
    return nullptr;
}
