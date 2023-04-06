#include <Vortex/Renderer.hpp>

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
    struct SceneLight {
        SceneLight() = default;
        SceneLight(const glm::mat4& transform, const glm::vec4& color) : Transform(transform), Color(color) {
        }
        ~SceneLight() = default;

        glm::mat4 Transform;
        glm::vec4 Color;
    };

    struct SceneMesh {
        SceneMesh() = default;
        SceneMesh(const glm::mat4& transform, const std::shared_ptr<VertexArray>& vertexArray) : Transform(transform), VertexArray(vertexArray) {
        }
        ~SceneMesh() = default;

        glm::mat4 Transform;
        std::shared_ptr<VertexArray> VertexArray;
    };

    std::vector<SceneLight> lights;
    std::vector<SceneMesh> meshes;

    for (auto object : scene->GetObjects()) {
        auto transform = object->GetTransform();
        auto pointLights = object->GetPointLights();
        auto objectMeshes = object->GetMeshs();
        for (auto pointLight : pointLights) {
            SceneLight sceneLight(transform, pointLight->GetColor());
            lights.push_back(sceneLight);
        }
        for (auto mesh : objectMeshes) {
            for (auto vertexArray : mesh->GetVertexArrays()) {
                SceneMesh sceneMesh(transform, vertexArray);
                meshes.push_back(sceneMesh);
            }
        }
    }

    auto camera = scene->GetCamera();

    m_Shader->Bind();
    m_Shader->SetMatrix4("u_ViewProj", camera->GetViewProj());
    // Bind lights to shader
    // ...

    for (auto mesh : meshes) {
        auto vertexArray = mesh.VertexArray;
        glm::mat4 model = mesh.Transform;
        m_Shader->SetMatrix4("u_Model", model);
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}
