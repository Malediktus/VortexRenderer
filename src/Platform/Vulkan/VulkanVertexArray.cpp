#include <Vortex/Buffer.hpp>
#include <Vortex/Platform/Vulkan/VulkanVertexArray.hpp>
#include <Vortex/Platform/Vulkan/VulkanRendererAPI.hpp>
#include <spdlog/spdlog.h>
#include <tracy/Tracy.hpp>

using namespace Vortex::Vulkan;

VulkanVertexArray::VulkanVertexArray() {
    ZoneScoped;
}

VulkanVertexArray::~VulkanVertexArray() {
    ZoneScoped;
    // spdlog::trace("Deleted Vulkan vertex array (ID: {})", m_RendererID);
}

void VulkanVertexArray::Bind() const {
    ZoneScoped;
    // spdlog::trace("Bound Vulkan vertex array (ID: {})", m_RendererID);
}

void VulkanVertexArray::Unbind() const {
    ZoneScoped;
    // spdlog::trace("Unbount Vulkan vertex array (ID: {})", m_RendererID);
}

void VulkanVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) {
    ZoneScoped;
    // spdlog::trace("Attached vertex buffer to Vulkan vertex array (ID: {})", m_RendererID);
}

void VulkanVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) {
    ZoneScoped;
    // spdlog::trace("Set index buffer of Vulkan vertex array (ID: {})", m_RendererID);
}
