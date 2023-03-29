#include <Vortex/Renderer.hpp>

using namespace Vortex;

std::shared_ptr<ShaderLibrary> Renderer::s_ShaderLibrary;

void Renderer::Init() {
    RenderCommand::Init();
    s_ShaderLibrary = std::make_shared<ShaderLibrary>();
}

void Renderer::LoadShader(const std::string &name, const std::string &filepath) {
    s_ShaderLibrary->Load(name, filepath);
}

void Renderer::LoadShader(const std::string &filepath) {
    s_ShaderLibrary->Load(filepath);
}

void Renderer::AddShader(const std::string &name, const std::shared_ptr<Shader>& shader) {
    s_ShaderLibrary->Add(name, shader);
}

void Renderer::AddShader(const std::shared_ptr<Shader>& shader) {
    s_ShaderLibrary->Add(shader);
}

void Renderer::BeginScene() {
}

void Renderer::EndScene() {
}

void Renderer::Submit(const std::string &shaderName, const std::shared_ptr<VertexArray>& vertexArray) {
    vertexArray->Bind();
    s_ShaderLibrary->Get(shaderName)->Bind();
    RenderCommand::DrawIndexed(vertexArray);
}
