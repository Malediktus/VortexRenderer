#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Vortex/Platform/OpenGL/OpenGLContext.hpp>
#include <spdlog/spdlog.h>
#include <tracy/Tracy.hpp>
#include <cassert>

using namespace Vortex::OpenGL;

void OpenGLContext::Init() const {
    ZoneScoped;
    glfwMakeContextCurrent((GLFWwindow*) m_WindowHandle);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
        assert(false);

    spdlog::info("Created OpenGL context");
    int majorVersion, minorVersion;
    glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
    spdlog::info("Using OpenGL version {}.{}", majorVersion, minorVersion);
}
