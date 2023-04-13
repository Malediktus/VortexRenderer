#include <Vortex/Core.hpp>
#include <Vortex/Platform/OpenGL/OpenGLTexture.hpp>
#include <Vortex/Texture.hpp>
#include <Vortex/Renderer.hpp>
#include <tracy/Tracy.hpp>

using namespace Vortex;

std::shared_ptr<Texture2D> Vortex::Texture2DCreate(const std::string& path) {
    ZoneScoped;
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        return std::make_shared<OpenGL::OpenGLTexture2D>(path);
    default:
        assert(false);
    }
    assert(false);
}

std::shared_ptr<Texture2D> Vortex::Texture2DCreate(const int width, const int height) {
    ZoneScoped;
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        return std::make_shared<OpenGL::OpenGLTexture2D>(width, height);
    default:
        assert(false);
    }
    assert(false);
}

std::shared_ptr<Texture2D> Vortex::Texture2DCreate(const int width, const int height, const void* data) {
    ZoneScoped;
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        return std::make_shared<OpenGL::OpenGLTexture2D>(width, height, data);
    default:
        assert(false);
    }
    assert(false);
}
