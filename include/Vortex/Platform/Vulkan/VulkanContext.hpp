#pragma once

#include "../../Context.hpp"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <memory>

namespace Vortex::Vulkan {
class VulkanContext : public Context {
public:
    VulkanContext(void* windowHandle) : m_WindowHandle(windowHandle) {
        (void) m_WindowHandle;
    }
    virtual ~VulkanContext() = default;

    virtual void Init() override;
    virtual void Destroy() override;

    virtual VkInstance GetInstance() const {
        return m_Instance;
    }

private:
    void* m_WindowHandle;
    VkInstance m_Instance;
    VkDebugUtilsMessengerEXT m_DebugMessenger;
};
} // namespace Vortex::Vulkan