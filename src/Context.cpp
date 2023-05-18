#include <Vortex/Context.hpp>
#include <Vortex/Core.hpp>
#include <Vortex/Renderer.hpp>
#include <Vortex/Platform/OpenGL/OpenGLContext.hpp>
#include <tracy/Tracy.hpp>
#include <spdlog/spdlog.h>

using namespace Vortex;

std::shared_ptr<Context> Vortex::ContextCreate(const std::shared_ptr<Vortex::Window>& window, ProjectInformation projectInfo) {
    ZoneScoped;

    // TODO: Find better way to init logger
#ifdef VT_DEBUG
    spdlog::set_level(spdlog::level::level_enum::debug);
#else
    spdlog::set_level(spdlog::level::level_enum::warn);
#endif
    spdlog::set_pattern("[%T] %^[%l%$] %v%$");

    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        return std::make_shared<OpenGL::OpenGLContext>(window, projectInfo);
    default:
        VT_ASSERT_CHECK(false, "Invalid renderer API value returned from Renderer::GetRendererAPI()");
    }
    return nullptr;
}
