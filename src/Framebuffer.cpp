#include <Vortex/Framebuffer.hpp>
#include <Vortex/Platform/OpenGL/OpenGLFramebuffer.hpp>
#include <Vortex/Platform/Vulkan/VulkanFramebuffer.hpp>
#include <Vortex/Renderer.hpp>
#include <tracy/Tracy.hpp>

using namespace Vortex;

std::shared_ptr<Framebuffer> Vortex::FramebufferCreate() {
    ZoneScoped;
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        return std::make_shared<OpenGL::OpenGLFramebuffer>();
    case RendererAPI::API::Vulkan:
        return std::make_shared<Vulkan::VulkanFramebuffer>();
    default:
        VT_ASSERT_CHECK(false, "Invalid renderer API value returned from Renderer::GetRendererAPI()");
    }
    return nullptr;
}
