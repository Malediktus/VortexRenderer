#pragma once

#include "Core.hpp"
#include "Window.hpp"

#include <memory>

namespace Vortex {
// Set to -1 if required, if not set to score to add for property
struct ProjectRequirements {
    int DiscreteGPU;
    int IntegratedGPU;
    int NoGPU;
    int GeometryShader;
    int TeslationShader;
    int SamplerAnisotropy;
    uint32_t MinHeapMegabyte;
    uint32_t HeapMegabyte;
};

/**
 * @brief The Context class is responsible for providing a rendering contex and buffer swapping.
 *
 * @version 0.1
 * @author Nico Grundei (malediktusrgb@gmail.com)
 * @date 2023-03-27
 * @copyright Copyright (c) 2023
 */
class Context {
public:
    VT_API ~Context() = default;

    virtual VT_API void Init() = 0;
    virtual VT_API void Destroy() = 0;

    virtual VT_API std::shared_ptr<Vortex::Window> GetWindow() = 0;
};

/**
 * @brief This function is the only way that should be used for creating a Context.
 *
 * @param windowHandle
 * @return std::shared_ptr<Context>
 * @version 0.1
 * @author Nico Grundei (malediktusrgb@gmail.com)
 * @date 2023-03-31
 * @copyright Copyright (c) 2023
 */
VT_API std::shared_ptr<Context> ContextCreate(const std::shared_ptr<Vortex::Window>& window, ProjectRequirements requirements);
} // namespace Vortex
