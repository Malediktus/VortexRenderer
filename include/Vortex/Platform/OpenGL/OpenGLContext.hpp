#pragma once

#include "../../Context.hpp"

#include <GLFW/glfw3.h>

namespace Vortex::OpenGL {
class OpenGLContext : public Context {
public:
    OpenGLContext(void* windowHandle) : m_WindowHandle(windowHandle) {
    }
    ~OpenGLContext() = default;

    virtual void Init() const override;
    virtual void SwapBuffers() const override;

private:
    void* m_WindowHandle;
};
} // namespace Vortex::OpenGL
