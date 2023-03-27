#include <GLFW/glfw3.h>
#include <Vortex/Context.hpp>
#include <Vortex/Core.hpp>
#include <Vortex/Platform/OpenGL/OpenGLContext.hpp>

using namespace Vortex;

std::shared_ptr<Context> Vortex::ContextCreate(void *windowHandle) {
  RenderingAPI CurrentRenderingAPI = RenderingAPI::OpenGL;

  switch (CurrentRenderingAPI) {
  case RenderingAPI::OpenGL:
    return std::make_shared<OpenGL::OpenGLContext>(windowHandle);
  }
  assert(false);
}
