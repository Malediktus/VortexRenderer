#include <GLFW/glfw3.h>
#include <Vortex/Context.hpp>
#include <Vortex/Core.hpp>
#include <Vortex/Renderer.hpp>
#include <Vortex/Platform/OpenGL/OpenGLContext.hpp>
#include <tracy/Tracy.hpp>

using namespace Vortex;

std::shared_ptr<Context> Vortex::ContextCreate(void* windowHandle) {
    ZoneScoped;
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
<<<<<<< Updated upstream
        return std::make_shared<OpenGL::OpenGLContext>(windowHandle);
=======
        return std::make_shared<OpenGL::OpenGLContext>(window, projectInfo);
>>>>>>> Stashed changes
    default:
        VT_ASSERT_CHECK(false, "Invalid renderer API value returned from Renderer::GetRendererAPI()");
    }
    return nullptr;
}
