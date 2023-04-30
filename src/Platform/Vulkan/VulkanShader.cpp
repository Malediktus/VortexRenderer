#include <Vortex/Platform/Vulkan/VulkanShader.hpp>
#include <Vortex/Platform/Vulkan/VulkanRendererAPI.hpp>
#include <array>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>
#include <tracy/Tracy.hpp>
#include <iostream>
#include <vector>

using namespace Vortex::Vulkan;

VulkanShader::VulkanShader(const std::string& filepath) {
    ZoneScoped;
}

VulkanShader::VulkanShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc, const std::string& geometrySrc) : m_Name(name) {
    ZoneScoped;
}

VulkanShader::~VulkanShader() {
    ZoneScoped;
    // spdlog::trace("Deleted Vulkan shader program (ID: {})", m_RendererID); // Dont now why but this segfaults (maybe lifetime related)
}

void VulkanShader::Bind() const {
    ZoneScoped;
    // spdlog::trace("Bound Vulkan shader program (ID: {})", m_RendererID);
}

void VulkanShader::Unbind() const {
    ZoneScoped;
    // spdlog::trace("Unbound Vulkan shader program (ID: {})", m_RendererID);
}

void VulkanShader::SetInt(const std::string& name, int value) {
    ZoneScoped;
}

void VulkanShader::SetFloat(const std::string& name, float value) {
    ZoneScoped;
}

void VulkanShader::SetFloat2(const std::string& name, const glm::vec2& value) {
    ZoneScoped;
}

void VulkanShader::SetFloat3(const std::string& name, const glm::vec3& value) {
    ZoneScoped;
}

void VulkanShader::SetFloat4(const std::string& name, const glm::vec4& value) {
    ZoneScoped;
}

void VulkanShader::SetMatrix3(const std::string& name, const glm::mat3& matrix) {
    ZoneScoped;
}

void VulkanShader::SetMatrix4(const std::string& name, const glm::mat4& matrix) {
    ZoneScoped;
}

void VulkanShader::UploadInt(const std::string& name, int value) {
    ZoneScoped;
    // spdlog::trace("Uploaded int to Vulkan shader program (name: {}, value: {}, ID: {})", name, value, m_RendererID);
}

void VulkanShader::UploadFloat(const std::string& name, float value) {
    ZoneScoped;
    // spdlog::trace("Uploaded float to Vulkan shader program (name: {}, value: {}, ID: {})", name, value, m_RendererID);
}

void VulkanShader::UploadFloat2(const std::string& name, const glm::vec2& value) {
    ZoneScoped;
    // spdlog::trace("Uploaded vector2 to Vulkan shader program (name: {}, value: ({}, {}), ID: {})", name, value.x, value.y, m_RendererID);
}

void VulkanShader::UploadFloat3(const std::string& name, const glm::vec3& value) {
    ZoneScoped;
    // spdlog::trace("Uploaded vector3 to Vulkan shader program (name: {}, value: ({}, {}, {}), ID: {})", name, value.x, value.y, value.z, m_RendererID);
}

void VulkanShader::UploadFloat4(const std::string& name, const glm::vec4& value) {
    ZoneScoped;
    // spdlog::trace("Uploaded vector4 to Vulkan shader program (name: {}, value: ({}, {}, {}, {}), ID: {})", name, value.x, value.y, value.z, value.w, m_RendererID);
}

void VulkanShader::UploadMatrix3(const std::string& name, const glm::mat3& matrix) {
    ZoneScoped;
    // spdlog::trace("Uploaded matrix3x3 to Vulkan shader program (name: {}, ID: {})", name, m_RendererID);
}

void VulkanShader::UploadMatrix4(const std::string& name, const glm::mat4& matrix) {
    ZoneScoped;
    // spdlog::trace("Uploaded matrix4x4 to Vulkan shader program (name: {}, ID: {})", name, m_RendererID);
}

void VulkanShader::UploadUniformQueues() {
    ZoneScoped;
}
