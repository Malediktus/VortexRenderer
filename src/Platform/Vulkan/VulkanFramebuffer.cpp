#include <Vortex/Platform/Vulkan/VulkanFramebuffer.hpp>
#include <Vortex/Platform/Vulkan/VulkanRendererAPI.hpp>
#include <spdlog/spdlog.h>
#include <tracy/Tracy.hpp>

using namespace Vortex::Vulkan;

VulkanFramebuffer::VulkanFramebuffer() {
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

void VulkanFramebuffer::AttachColorBuffer(const std::shared_ptr<Texture2D>& texture) {
    ZoneScoped;
    // spdlog::trace("Attached texture of type color buffer to Vulkan framebuffer (ID: {}, TextureID: {})", m_RendererID, *(GLuint*) texture->GetNative());
}

void VulkanFramebuffer::AttachDepthBuffer(const std::shared_ptr<Texture2D>& texture) {
    ZoneScoped;
    // spdlog::trace("Attached texture of type depth buffer to Vulkan framebuffer (ID: {}, TextureID: {})", m_RendererID, *(GLuint*) texture->GetNative());
}

void VulkanFramebuffer::AttachStencilBuffer(const std::shared_ptr<Texture2D>& texture) {
    ZoneScoped;
    // spdlog::trace("Attached texture of type stencil buffer to Vulkan framebuffer (ID: {}, TextureID: {})", m_RendererID, *(GLuint*) texture->GetNative());
}

void VulkanFramebuffer::AttachDepthStencilBuffer(const std::shared_ptr<Renderbuffer>& renderbuffer) {
    ZoneScoped;
    // spdlog::trace("Attached renderbuffer of type depth-stencil buffer to Vulkan framebuffer (ID: {}, RenderbufferID: {})", m_RendererID, *(GLuint*) renderbuffer->GetNative());
}
