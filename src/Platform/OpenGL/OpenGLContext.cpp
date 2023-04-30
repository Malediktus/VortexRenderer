#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Vortex/Platform/OpenGL/OpenGLContext.hpp>
#include <spdlog/spdlog.h>
#include <tracy/Tracy.hpp>

using namespace Vortex::OpenGL;

void OpenGLContext::Init() {
    ZoneScoped;
    glfwMakeContextCurrent((GLFWwindow*) m_WindowHandle);
    VT_ASSERT(gladLoadGLLoader((GLADloadproc) glfwGetProcAddress), "Failed to initialize OpenGL Context");

    spdlog::info("Created OpenGL context");
    int majorVersion, minorVersion;
    glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
    spdlog::info("Using OpenGL version {}.{}", majorVersion, minorVersion);
}
