#include <Vortex/Core.hpp>
#include <Vortex/Platform/OpenGL/OpenGLTexture.hpp>
#include <Vortex/Texture.hpp>
#include <Vortex/Renderer.hpp>
#include <tracy/Tracy.hpp>

using namespace Vortex;

std::shared_ptr<Texture2D> Vortex::Texture2DCreate(const int width, const int height, Texture2D::Texture2DUsageType usageType) {
    ZoneScoped;
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        return std::make_shared<OpenGL::OpenGLTexture2D>(width, height, usageType);
    default:
        VT_ASSERT_CHECK(false, "Invalid renderer API value returned from Renderer::GetRendererAPI()");
    }
    return nullptr;
}

std::shared_ptr<Texture2D> Vortex::Texture2DCreate(const int width, const int height, const void* data, Texture2D::Texture2DUsageType usageType) {
    ZoneScoped;
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        return std::make_shared<OpenGL::OpenGLTexture2D>(width, height, data, usageType);
    default:
        VT_ASSERT_CHECK(false, "Invalid renderer API value returned from Renderer::GetRendererAPI()");
    }
    return nullptr;
}
