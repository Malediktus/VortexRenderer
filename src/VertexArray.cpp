#include <Vortex/Core.hpp>
#include <Vortex/Platform/OpenGL/OpenGLVertexArray.hpp>
#include <Vortex/VertexArray.hpp>
#include <Vortex/Renderer.hpp>
#include <tracy/Tracy.hpp>

using namespace Vortex;

std::shared_ptr<VertexArray> Vortex::VertexArrayCreate() {
    ZoneScoped;
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        return std::make_shared<OpenGL::OpenGLVertexArray>();
    default:
        VT_ASSERT_CHECK(false, "Invalid renderer API value returned from Renderer::GetRendererAPI()");
    }
    return nullptr;
}
