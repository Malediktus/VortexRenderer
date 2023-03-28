#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Vortex/Platform/OpenGL/OpenGLContext.hpp>

using namespace Vortex::OpenGL;

void OpenGLContext::Init() const {
    glfwMakeContextCurrent((GLFWwindow*) m_WindowHandle);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
        assert(false);
}

void OpenGLContext::SwapBuffers() const {
    glfwSwapBuffers((GLFWwindow*) m_WindowHandle);
}
