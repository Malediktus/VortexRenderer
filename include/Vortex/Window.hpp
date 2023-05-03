#pragma once

#include <vector>
#include "RendererAPI.hpp"

namespace Vortex {
class Window {
public:
    virtual ~Window() = default;

    virtual const std::vector<const char*> GetVulkanInstanceExtensions() = 0;
    virtual void* GetSurface(void* instance) = 0;
    virtual void SetupOpenglContext() = 0;

    static VT_API RendererAPI::API ChooseRenderingAPI(bool vulkanSupported);
};
} // namespace Vortex
