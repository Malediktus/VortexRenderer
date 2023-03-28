#include <GLFW/glfw3.h>
#include <Vortex/Context.hpp>
#include <Vortex/Core.hpp>
#include <Vortex/Renderer.hpp>
#include <Vortex/Platform/OpenGL/OpenGLContext.hpp>
#include <cassert>

using namespace Vortex;

std::shared_ptr<Context> Vortex::ContextCreate(void* windowHandle) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        return std::make_shared<OpenGL::OpenGLContext>(windowHandle);
    default:
        assert(false);
    }
    assert(false);
}
