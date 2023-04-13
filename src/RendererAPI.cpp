#include <Vortex/RendererAPI.hpp>
#include <Vortex/Renderer.hpp>
#include <Vortex/Platform/OpenGL/OpenGLRendererAPI.hpp>
#include <tracy/Tracy.hpp>

using namespace Vortex;

RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

std::shared_ptr<RendererAPI> Vortex::RendererAPICreate() {
    ZoneScoped;
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        return std::make_shared<OpenGL::OpenGLRendererAPI>();
    default:
        VT_ASSERT_CHECK(false, "Invalid renderer API value returned from Renderer::GetRendererAPI()");
    }
    return nullptr;
}
