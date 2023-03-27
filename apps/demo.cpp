#include <GLFW/glfw3.h>
#include <Vortex/Context.hpp>
#include <cstdlib>
#include <iostream>

static void GlfwErrorCallback(int error, const char *description) {
  std::cout << "GLFW Error (" << error << "): " << description << std::endl;
  exit(1);
}

int main() {
  GLFWwindow *window;
  glfwSetErrorCallback(GlfwErrorCallback);
  if (!glfwInit()) {
    std::cout << "Failed to init GLFW!" << std::endl;
    return 1;
  }

  window =
      glfwCreateWindow(1024, 720, "Vortex Renderer Demo", nullptr, nullptr);
  if (!window) {
    std::cout << "Failed to create window!" << std::endl;
    return 1;
  }

  auto context = Vortex::ContextCreate(window);
  context->Init();

  while (!glfwWindowShouldClose(window)) {
    context->SwapBuffers();
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
