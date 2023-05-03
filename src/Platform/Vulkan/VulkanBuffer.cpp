#include <Vortex/Platform/Vulkan/VulkanBuffer.hpp>
#include <Vortex/Platform/Vulkan/VulkanRendererAPI.hpp>
#include <spdlog/spdlog.h>
#include <tracy/Tracy.hpp>

using namespace Vortex::Vulkan;

VulkanVertexBuffer::VulkanVertexBuffer(uint32_t size) {
    ZoneScoped;
    (void) size;
    // spdlog::trace("Created Vulkan vertex buffer of type dynamic (ID: {})", m_RendererID);
}

VulkanVertexBuffer::VulkanVertexBuffer(float* vertices, uint32_t size) {
    ZoneScoped;
    (void) size;
    (void) vertices;
    // spdlog::trace("Created Vulkan vertex buffer of type static (ID: {})", m_RendererID);
}

VulkanVertexBuffer::~VulkanVertexBuffer() {
    ZoneScoped;
    // spdlog::trace("Deleted Vulkan vertex buffer (ID: {})", m_RendererID);
}

void VulkanVertexBuffer::Bind() const {
    ZoneScoped;
    // spdlog::trace("Bound Vulkan vertex buffer (ID: {})", m_RendererID);
}

void VulkanVertexBuffer::Unbind() const {
    ZoneScoped;
    // spdlog::trace("Unbound Vulkan vertex buffer (ID: {})", m_RendererID);
}

void VulkanVertexBuffer::SetData(const void* data, uint32_t size) {
    ZoneScoped;
    // spdlog::trace("Set data for Vulkan vertex buffer (ID: {})", m_RendererID);
}

VulkanIndexBuffer::VulkanIndexBuffer(uint32_t* indices, uint32_t count) {
    ZoneScoped;
    (void) indices;
    (void) count;
    // spdlog::trace("Created Vulkan index buffer of type static (ID: {})", m_RendererID);
}

VulkanIndexBuffer::~VulkanIndexBuffer() {
    ZoneScoped;
    // spdlog::trace("Deleted Vulkan index buffer (ID: {})", m_RendererID);
}

void VulkanIndexBuffer::Bind() const {
    ZoneScoped;
    // spdlog::trace("Bound Vulkan index buffer (ID: {})", m_RendererID);
}

void VulkanIndexBuffer::Unbind() const {
    ZoneScoped;
    // spdlog::trace("Unbound Vulkan index buffer (ID: {})", m_RendererID);
}
