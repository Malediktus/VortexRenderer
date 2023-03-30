#include <Vortex/Renderer.hpp>

using namespace Vortex;

std::shared_ptr<ShaderLibrary> Renderer::s_ShaderLibrary;
std::shared_ptr<Renderer::SceneData> Renderer::s_SceneData;

void Renderer::Init() {
    RenderCommand::Init();
    s_ShaderLibrary = std::make_shared<ShaderLibrary>();
}

void Renderer::LoadShader(const std::string& name, const std::string& filepath) {
    s_ShaderLibrary->Load(name, filepath);
}

void Renderer::LoadShader(const std::string& filepath) {
    s_ShaderLibrary->Load(filepath);
}

void Renderer::AddShader(const std::string& name, const std::shared_ptr<Shader>& shader) {
    s_ShaderLibrary->Add(name, shader);
}

void Renderer::AddShader(const std::shared_ptr<Shader>& shader) {
    s_ShaderLibrary->Add(shader);
}

void Renderer::BeginScene(const std::shared_ptr<Camera>& camera, const std::string& shaderName) {
    s_SceneData = std::make_shared<Renderer::SceneData>(camera, shaderName);
}

void Renderer::EndScene() {
}

void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray) {
    vertexArray->Bind();
    const auto& shader = s_ShaderLibrary->Get(s_SceneData->SceneShader);
    shader->Bind();
    shader->UploadUniformMat4("uViewProj", s_SceneData->SceneCamera->GetViewProjectionMatrix());
    RenderCommand::DrawIndexed(vertexArray);
}
