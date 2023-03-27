#include <GLFW/glfw3.h>
#include <Vortex/Platform/OpenGL/OpenGLContext.hpp>

using namespace Vortex::OpenGL;

void OpenGLContext::Init() const {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwMakeContextCurrent((GLFWwindow *)m_WindowHandle);
}

void OpenGLContext::SwapBuffers() const {
  glfwSwapBuffers((GLFWwindow *)m_WindowHandle);
}
