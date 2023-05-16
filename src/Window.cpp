#include <Vortex/Window.hpp>

using namespace Vortex;

RendererAPI::API Window::ChooseRenderingAPI() {
    auto api = RendererAPI::API::OpenGL;

    RendererAPI::SetAPI(api);
    return api;
}
