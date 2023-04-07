#pragma once

#include "../../Context.hpp"

#include <GLFW/glfw3.h>

namespace Vortex::OpenGL {
class OpenGLContext : public Context {
public:
    OpenGLContext(void* windowHandle) : m_WindowHandle(windowHandle) {
    }
    virtual ~OpenGLContext() = default;

    virtual void Init() const override;

private:
    void* m_WindowHandle;
};
} // namespace Vortex::OpenGL
