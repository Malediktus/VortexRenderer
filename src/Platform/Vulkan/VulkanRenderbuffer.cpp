#include <Vortex/Platform/Vulkan/VulkanRenderbuffer.hpp>
#include <Vortex/Platform/Vulkan/VulkanRendererAPI.hpp>
#include <spdlog/spdlog.h>
#include <tracy/Tracy.hpp>

using namespace Vortex::Vulkan;

VulkanRenderbuffer::VulkanRenderbuffer(uint32_t width, uint32_t height, Renderbuffer::RenderbufferUsageType usageType) {
    (void) width;
    (void) height;
    (void) usageType;
    ZoneScoped;
    // spdlog::trace("Created Vulkan renderbuffer (ID: {})", m_RendererID);
}

VulkanRenderbuffer::~VulkanRenderbuffer() {
    ZoneScoped;
    // spdlog::trace("Deleted Vulkan renderbuffer (ID: {})", m_RendererID);
}

void VulkanRenderbuffer::Bind() const {
    ZoneScoped;
    // spdlog::trace("Bound Vulkan renderbuffer (ID: {})", m_RendererID);
}

void VulkanRenderbuffer::Unbind() const {
    ZoneScoped;
    // spdlog::trace("Unbound Vulkan renderbuffer (ID: {})", m_RendererID);
}

void* VulkanRenderbuffer::GetNative() const {
    return nullptr;
}
