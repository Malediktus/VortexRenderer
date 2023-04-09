#include <Vortex/Renderer.hpp>
#include <glm/gtc/matrix_access.hpp>

using namespace Vortex;

std::shared_ptr<Shader> Renderer::m_Shader;
std::shared_ptr<Context> Renderer::m_Context;

void Renderer::Init(const std::shared_ptr<Context>& context, const std::string& shaderPath, const int width, const int height) {
    m_Context = context;
    m_Context->Init();
    RenderCommand::Init();
    m_Shader = ShaderCreate(shaderPath);

    // Settings
    Vortex::RenderCommand::SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});
    Vortex::RenderCommand::ConfigureStencilTesting(false, 0xFF, 0xFF, Vortex::RendererAPI::StencilTestFunc::ALWAYS, 0x11, Vortex::RendererAPI::StencilTestAction::KEEP,
                                                   Vortex::RendererAPI::StencilTestAction::KEEP, Vortex::RendererAPI::StencilTestAction::KEEP);
    Vortex::RenderCommand::ConfigureBlending(true, Vortex::RendererAPI::BlendingFunc::SRC_ALPHA, Vortex::RendererAPI::BlendingFunc::ONE_MINUS_SRC_ALPHA,
                                             Vortex::RendererAPI::BlendingFunc::ONE, Vortex::RendererAPI::BlendingFunc::ZERO, Vortex::RendererAPI::BlendingFunc::ONE,
                                             Vortex::RendererAPI::BlendingFunc::ZERO);
    Vortex::RenderCommand::ConfigureDepthTesting(true, true, Vortex::RendererAPI::DepthTestFunc::LESS);
    Vortex::RenderCommand::ConfigureCulling(false, Vortex::RendererAPI::CullingType::BACK);
    Vortex::RenderCommand::SetViewport(width, height);
}

void Renderer::OnResize(const int width, const int height) {
    Vortex::RenderCommand::SetViewport(width, height);
}

void Renderer::BeginFrame() {
    Vortex::RenderCommand::Clear(Vortex::RendererAPI::ClearBuffer::COLOR);
    Vortex::RenderCommand::Clear(Vortex::RendererAPI::ClearBuffer::DEPTH);
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

    std::vector<std::shared_ptr<SceneLight>> sceneLights;
    std::vector<glm::mat4> sceneLightTransforms;
    std::vector<std::shared_ptr<VertexArray>> vertexArrays;
    std::vector<glm::mat4> vertexArrayTransforms;

    for (auto object : scene->GetObjects()) {
        auto transform = object->GetTransform();
        auto objectPointLights = object->GetLights();
        auto objectMeshes = object->GetMeshs();
        for (auto pointLight : objectPointLights) {
            sceneLights.push_back(pointLight);
            sceneLightTransforms.push_back(transform);
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

    uint32_t numPointLights = 0, numDirectionalLights = 0, numSpotLights = 0;

    for (uint32_t i = 0; i < sceneLights.size(); i++) {
        if (sceneLights[i]->Type == SceneLight::SceneLightType::Point) {
            PointLight* pointLight = (PointLight*) sceneLights[i].get();
            std::string prefix = "u_PointLights[" + std::to_string(i) + "].";
            glm::vec3 position = glm::vec3(glm::column(sceneLightTransforms[i], 3));
            m_Shader->SetFloat3(prefix + "position", position);
            m_Shader->SetFloat(prefix + "constant", pointLight->Constant);
            m_Shader->SetFloat(prefix + "linear", pointLight->Linear);
            m_Shader->SetFloat(prefix + "quadratic", pointLight->Quadratic);
            m_Shader->SetFloat3(prefix + "ambient", pointLight->Ambient);
            m_Shader->SetFloat3(prefix + "diffuse", pointLight->Diffuse);
            m_Shader->SetFloat3(prefix + "specular", pointLight->Specular);
            numPointLights++;
        } else if (sceneLights[i]->Type == SceneLight::SceneLightType::Directional) {
            DirectionalLight* directionalLight = (DirectionalLight*) sceneLights[i].get();
            std::string prefix = "u_DirectionalLights[" + std::to_string(i) + "].";
            glm::vec3 position = glm::vec3(glm::column(sceneLightTransforms[i], 3));
            m_Shader->SetFloat3(prefix + "position", position);
            m_Shader->SetFloat3(prefix + "ambient", directionalLight->Ambient);
            m_Shader->SetFloat3(prefix + "diffuse", directionalLight->Diffuse);
            m_Shader->SetFloat3(prefix + "specular", directionalLight->Specular);
            numDirectionalLights++;
        } else if (sceneLights[i]->Type == SceneLight::SceneLightType::Spot) {
            SpotLight* spotLight = (SpotLight*) sceneLights[i].get();
            std::string prefix = "u_SpotLights[" + std::to_string(i) + "].";
            glm::vec3 position = glm::vec3(glm::column(sceneLightTransforms[i], 3));
            glm::vec3 direction = glm::vec3(glm::column(sceneLightTransforms[i], 2));
            m_Shader->SetFloat3(prefix + "position", position);
            m_Shader->SetFloat3(prefix + "direction", direction);
            m_Shader->SetFloat(prefix + "cutOff", spotLight->CutOff);
            m_Shader->SetFloat(prefix + "outerCutOff", spotLight->OuterCutOff);
            m_Shader->SetFloat(prefix + "constant", spotLight->Constant);
            m_Shader->SetFloat(prefix + "linear", spotLight->Linear);
            m_Shader->SetFloat(prefix + "quadratic", spotLight->Quadratic);
            m_Shader->SetFloat3(prefix + "ambient", spotLight->Ambient);
            m_Shader->SetFloat3(prefix + "diffuse", spotLight->Diffuse);
            m_Shader->SetFloat3(prefix + "specular", spotLight->Specular);
            numSpotLights++;
        }
    }

    m_Shader->SetInt("u_NumPointLights", numPointLights);
    m_Shader->SetInt("u_NumDirectionalLights", numDirectionalLights);
    m_Shader->SetInt("u_NumSpotLights", numSpotLights);

    for (uint32_t i = 0; i < vertexArrays.size(); i++) {
        m_Shader->SetMatrix4("u_Model", vertexArrayTransforms[i]);
        RenderCommand::DrawIndexed(vertexArrays[i]);
    }
}
