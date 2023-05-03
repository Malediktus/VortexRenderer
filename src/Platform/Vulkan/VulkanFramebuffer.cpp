#include <Vortex/Platform/Vulkan/VulkanFramebuffer.hpp>
#include <Vortex/Platform/Vulkan/VulkanRendererAPI.hpp>
#include <spdlog/spdlog.h>
#include <tracy/Tracy.hpp>

using namespace Vortex::Vulkan;

VulkanFramebuffer::VulkanFramebuffer(const std::shared_ptr<Window>& window) {
    ZoneScoped;
    // spdlog::trace("Created Vulkan framebuffer (ID: {})", m_RendererID);
}

VulkanFramebuffer::~VulkanFramebuffer() {
    ZoneScoped;
    // spdlog::trace("Deleted Vulkan framebuffer (ID: {})", m_RendererID);
}

void VulkanFramebuffer::Bind() const {
    ZoneScoped;
    // spdlog::info("Bound Vulkan framebuffer (ID: {})", m_RendererID);
}

void VulkanFramebuffer::Unbind() const {
    ZoneScoped;
    // spdlog::info("Unbound Vulkan framebuffer (ID: {})", m_RendererID);
}

void VulkanFramebuffer::AttachColorTexture(const std::shared_ptr<Texture2D>& texture) {
    ZoneScoped;
}

void VulkanFramebuffer::AttachDepthTexture(const std::shared_ptr<Texture2D>& texture) {
    ZoneScoped;
}

void VulkanFramebuffer::AttachStencilTexture(const std::shared_ptr<Texture2D>& texture) {
    ZoneScoped;
}

void VulkanFramebuffer::AttachDepthStencilTexture(const std::shared_ptr<Texture2D>& texture) {
    ZoneScoped;
}

void VulkanFramebuffer::AttachColorRenderbuffer(const std::shared_ptr<Renderbuffer>& renderbuffer) {
    ZoneScoped;
}

void VulkanFramebuffer::AttachDepthRenderbuffer(const std::shared_ptr<Renderbuffer>& renderbuffer) {
    ZoneScoped;
}

void VulkanFramebuffer::AttachStencilRenderbuffer(const std::shared_ptr<Renderbuffer>& renderbuffer) {
    ZoneScoped;
}

void VulkanFramebuffer::AttachDepthStencilRenderbuffer(const std::shared_ptr<Renderbuffer>& renderbuffer) {
    ZoneScoped;
}
