#include <Vortex/Renderer.hpp>
#include <glm/gtc/matrix_access.hpp>

using namespace Vortex;

std::shared_ptr<Shader> Renderer::m_Shader;

void Renderer::Init(const std::string& shaderPath) {
    RenderCommand::Init();
    m_Shader = ShaderCreate(shaderPath);
}

void Renderer::BeginFrame() {
}

void Renderer::EndFrame() {
}

void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray) {
    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
}

void Renderer::Submit(const std::shared_ptr<Scene>& scene) {
    // Possible optimization:
    // - Iterate over scene objects and set all lights in the shader
    // - Iterate again and render all vertexArrays
    // Benefit: No need to store a mat4, which is realy huge, for each light and vertex array

    std::vector<std::shared_ptr<PointLight>> pointLights;
    std::vector<glm::mat4> pointLightTransforms;
    std::vector<std::shared_ptr<VertexArray>> vertexArrays;
    std::vector<glm::mat4> vertexArrayTransforms;

    for (auto object : scene->GetObjects()) {
        auto transform = object->GetTransform();
        auto objectPointLights = object->GetPointLights();
        auto objectMeshes = object->GetMeshs();
        for (auto pointLight : objectPointLights) {
            pointLights.push_back(pointLight);
            pointLightTransforms.push_back(transform);
        }
        for (auto mesh : objectMeshes) {
            for (auto vertexArray : mesh->GetVertexArrays()) {
                vertexArrays.push_back(vertexArray);
                vertexArrayTransforms.push_back(transform);
            }
        }
    }

    auto camera = scene->GetCamera();

    m_Shader->Bind();
    m_Shader->SetMatrix4("u_ViewProj", camera->GetViewProj());
    m_Shader->SetFloat3("u_ViewPos", camera->GetPosition());
    m_Shader->SetInt("u_NumPointLights", pointLights.size());
    for (uint32_t i = 0; i < pointLights.size(); i++) {
        auto pointLight = pointLights[i];
        std::string prefix = "u_PointLights[" + std::to_string(i) + "].";
        glm::vec3 position = glm::vec3(glm::column(pointLightTransforms[i], 3));
        m_Shader->SetFloat3(prefix + "position", position);
        m_Shader->SetFloat(prefix + "constant", pointLight->Constant);
        m_Shader->SetFloat(prefix + "linear", pointLight->Linear);
        m_Shader->SetFloat(prefix + "quadratic", pointLight->Quadratic);
        m_Shader->SetFloat3(prefix + "ambient", pointLight->Ambient);
        m_Shader->SetFloat3(prefix + "diffuse", pointLight->Diffuse);
        m_Shader->SetFloat3(prefix + "specular", pointLight->Specular);
    }

    for (uint32_t i = 0; i < vertexArrays.size(); i++) {
        m_Shader->SetMatrix4("u_Model", vertexArrayTransforms[i]);
        RenderCommand::DrawIndexed(vertexArrays[i]);
    }
}
