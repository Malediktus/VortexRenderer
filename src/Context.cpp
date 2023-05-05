#include <GLFW/glfw3.h>
#include <Vortex/Context.hpp>
#include <Vortex/Core.hpp>
#include <Vortex/Renderer.hpp>
#include <Vortex/Platform/OpenGL/OpenGLContext.hpp>
#include <Vortex/Platform/Vulkan/VulkanContext.hpp>
#include <tracy/Tracy.hpp>
#include <spdlog/spdlog.h>

using namespace Vortex;

std::shared_ptr<Context> Vortex::ContextCreate(const std::shared_ptr<Vortex::Window>& window, ProjectRequirements requirements) {
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
        return std::make_shared<OpenGL::OpenGLContext>(window, requirements);
    case RendererAPI::API::Vulkan:
        return std::make_shared<Vulkan::VulkanContext>(window, requirements);
    default:
        VT_ASSERT_CHECK(false, "Invalid renderer API value returned from Renderer::GetRendererAPI()");
    }
    return nullptr;
}
