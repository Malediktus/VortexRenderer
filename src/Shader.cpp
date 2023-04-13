#include <Vortex/Core.hpp>
#include <Vortex/Platform/OpenGL/OpenGLShader.hpp>
#include <Vortex/Shader.hpp>
#include <Vortex/Renderer.hpp>
#include <tracy/Tracy.hpp>
#include <cassert>

using namespace Vortex;

std::shared_ptr<Shader> Vortex::ShaderCreate(const std::string& filepath) {
    ZoneScoped;
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        return std::make_shared<OpenGL::OpenGLShader>(filepath);
    default:
        assert(false);
    }
    assert(false);
}

std::shared_ptr<Shader> Vortex::ShaderCreate(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc, const std::string& geometrySrc) {
    ZoneScoped;
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        return std::make_shared<OpenGL::OpenGLShader>(name, vertexSrc, fragmentSrc, geometrySrc);
    default:
        assert(false);
    }
    assert(false);
}

void ShaderLibrary::Add(const std::string& name, const std::shared_ptr<Shader>& shader) {
    ZoneScoped;
    assert(!Exists(name));
    m_Shaders[name] = shader;
}

void ShaderLibrary::Add(const std::shared_ptr<Shader>& shader) {
    ZoneScoped;
    auto& name = shader->GetName();
    Add(name, shader);
}

std::shared_ptr<Shader> ShaderLibrary::Load(const std::string& filepath) {
    ZoneScoped;
    auto shader = ShaderCreate(filepath);
    Add(shader);
    return shader;
}

std::shared_ptr<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath) {
    ZoneScoped;
    auto shader = ShaderCreate(filepath);
    Add(name, shader);
    return shader;
}

std::shared_ptr<Shader> ShaderLibrary::Get(const std::string& name) {
    ZoneScoped;
    assert(Exists(name));
    return m_Shaders[name];
}

bool ShaderLibrary::Exists(const std::string& name) const {
    ZoneScoped;
    return m_Shaders.find(name) != m_Shaders.end();
}
