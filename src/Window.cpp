#include <Vortex/Window.hpp>

using namespace Vortex;

RendererAPI::API Window::ChooseRenderingAPI(bool vulkanSupported) {
    auto api = RendererAPI::API::OpenGL;
    if (vulkanSupported)
        api = RendererAPI::API::Vulkan;

    RendererAPI::SetAPI(api);
    return api;
}
