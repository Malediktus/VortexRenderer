#pragma once

#include "Core.hpp"

#include <memory>

namespace Vortex {
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

    virtual VT_API void Init() const = 0;
    virtual VT_API void SwapBuffers() const = 0;
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
VT_API std::shared_ptr<Context> ContextCreate(void* windowHandle);
} // namespace Vortex
