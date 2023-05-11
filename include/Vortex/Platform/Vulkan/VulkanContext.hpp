#pragma once

#include "../../Context.hpp"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <memory>

namespace Vortex::Vulkan {
class VulkanContext : public Context {
public:
    VulkanContext(const std::shared_ptr<Vortex::Window>& window, ProjectInformation projectInfo) : m_Window(window), m_ProjectInfo(projectInfo) {
    }
    virtual ~VulkanContext() = default;

    virtual void Init() override;
    virtual void Destroy() override;

    virtual VkInstance GetInstance() const {
        return m_Instance;
    }

    virtual std::shared_ptr<Vortex::Window> GetWindow() override {
        return m_Window;
    }

private:
    std::shared_ptr<Vortex::Window> m_Window;
    ProjectInformation m_ProjectInfo;

    VkInstance m_Instance;
    VkDebugUtilsMessengerEXT m_DebugMessenger;
    VkPhysicalDevice m_PhysicalDevice;
    VkDevice m_Device;
    VkQueue m_GraphicsQueue;
    VkSurfaceKHR m_WindowSurface;
};
} // namespace Vortex::Vulkan
