#pragma once

#include "Core.hpp"

#include <memory>

namespace Vortex {
/**
 * @brief Context class needs to be initiated using a GLFW window.
 * It is responsible for holding global variable/creating contexts for
 * RenderingAPIs It also handles buffer swapping.
 *
 * @example
 * Vortex::Context context((const void* window));
 * context->Init();
 * ...
 * while(glfwWindowShouldClose(window)) {
 * ...
 * context->SwapBuffers();
 * }
 *
 * @version 0.1
 * @author Nico Grundei (malediktusrgb@gmail.com)
 * @date 2023-03-27
 * @copyright Copyright (c) 2023
 */
class Context {
  public:
    ~Context() = default;

    VT_API virtual void Init() const = 0;
    VT_API virtual void SwapBuffers() const = 0;
};

VT_API std::shared_ptr<Context> ContextCreate(void* windowHandle);
} // namespace Vortex
