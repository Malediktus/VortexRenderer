#include <Vortex/Platform/Vulkan/VulkanTexture.hpp>
#include <Vortex/Platform/Vulkan/VulkanRendererAPI.hpp>
#include <stb_image/stb_image.h>
#include <spdlog/spdlog.h>
#include <tracy/Tracy.hpp>

using namespace Vortex::Vulkan;

VulkanTexture2D::VulkanTexture2D(const std::string& path) : m_Path(path) {
    ZoneScoped;
    // spdlog::trace("Created Vulkan texture2D from file (file: {}, ID: {})", path, m_RendererID);
}

VulkanTexture2D::VulkanTexture2D(const int width, const int height, Texture2D::Texture2DUsageType usageType) : m_Path("") {
    ZoneScoped;
    // spdlog::trace("Created Vulkan texture2D from file (ID: {})", m_RendererID);
}

VulkanTexture2D::VulkanTexture2D(const int width, const int height, const void* data, Texture2D::Texture2DUsageType usageType) : m_Path("") {
    ZoneScoped;
    // spdlog::trace("Created Vulkan texture2D from file (ID: {})", m_RendererID);
}

VulkanTexture2D::~VulkanTexture2D() {
    ZoneScoped;
    // spdlog::trace("Deleted Vulkan texture2D (ID: {})", m_RendererID);
}

void VulkanTexture2D::Bind(uint32_t slot) const {
    ZoneScoped;
    // spdlog::trace("Bound Vulkan texture2D (ID: {})", m_RendererID);
}

void VulkanTexture2D::Resize(uint32_t width, uint32_t height) {
    ZoneScoped;
    // spdlog::trace("Resized Vulkan texture2D (ID: {})", m_RendererID);
}

void* VulkanTexture2D::GetNative() const {
}
