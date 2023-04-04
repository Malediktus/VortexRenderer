#include <GLFW/glfw3.h>
#include <Vortex/Vortex.hpp>
#include <Vortex/Scene/Model.hpp>

#include <glm/ext/matrix_transform.hpp>

#include <cstdlib>
#include <iostream>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

class FPSCamera : public Vortex::Camera {
public:
    FPSCamera(float fov, float width, float height) : Camera(fov, width, height) {
        m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
        m_Yaw = -90.0f;
        m_Pitch = 0.0f;
        OnMouseMoved(0.0f, 0.0f);
        UpdateMatrices();
    }

    void OnMouseMoved(float xRel, float yRel) {
        m_Yaw += xRel * m_MouseSensitivity;
        m_Pitch -= yRel * m_MouseSensitivity;
        if (m_Pitch > 89.0f)
            m_Pitch = 89.0f;
        if (m_Pitch < -89.0f)
            m_Pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(m_Pitch)) * cos(glm::radians(m_Yaw));
        front.y = sin(glm::radians(m_Pitch));
        front.z = cos(glm::radians(m_Pitch)) * sin(glm::radians(m_Yaw));
        m_LookAt = glm::normalize(front);
        UpdateMatrices();
    }

    void MoveUp(float amount) {
        Translate(m_Up * amount);
    }

    void MoveFront(float amount) {
        Translate(glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f) * m_LookAt) * amount);
        UpdateMatrices();
    }

    void MoveSideways(float amount) {
        Translate(glm::normalize(glm::cross(m_LookAt, m_Up)) * amount);
        UpdateMatrices();
    }

    const glm::vec3& GetPosition() {
        return m_Position;
    }

    const glm::vec3& GetFront() {
        return m_LookAt;
    }

protected:
    void UpdateMatrices() override {
        m_View = glm::lookAt(m_Position, m_Position + m_LookAt, m_Up);
        m_ViewProj = m_Projection * m_View;
    }

    float m_Yaw;
    float m_Pitch;
    glm::vec3 m_LookAt;
    const float m_MouseSensitivity = 0.1f;
    glm::vec3 m_Up;
};

glm::vec3 pointLightPositions[] = {glm::vec3(0.1f, 0.2f, 2.0f), glm::vec3(2.3f, -3.3f, -4.0f), glm::vec3(-4.0f, 2.0f, -12.0f), glm::vec3(0.0f, 0.0f, -3.0f)};

class VortexDemo {
public:
    VortexDemo() {
        glfwSetErrorCallback(GlfwErrorCallback);
        if (!glfwInit()) {
            std::cout << "Failed to init GLFW!" << std::endl;
            exit(1);
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow(1024, 720, "Vortex Renderer Demo", nullptr, nullptr);
        if (!m_Window) {
            std::cout << "Failed to create window!" << std::endl;
            exit(1);
        }

        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetFramebufferSizeCallback(m_Window, OnResize);
        glfwSetCursorPosCallback(m_Window, OnMouseMove);
        glfwGetCursorPos(m_Window, &m_LastMouseX, &m_LastMouseY);

        int width, height;
        glfwGetFramebufferSize(m_Window, &width, &height);
        m_Camera = std::make_shared<FPSCamera>(90.0f, (float) width, (float) height);
        m_Camera->MoveFront(-5.0f);

        m_Context = Vortex::ContextCreate(m_Window);
        m_Context->Init();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void) io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        ImGui::StyleColorsClassic();
        ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
        ImGui_ImplOpenGL3_Init("#version 150");

        m_ShaderLibrary = std::make_shared<Vortex::ShaderLibrary>();
        m_ShaderLibrary->Load("assets/Shaders/Phong.glsl");

        m_Model = std::make_shared<Vortex::Scene::Model>("assets/Objects/Monkey/monkey.obj");

        Vortex::Renderer::Init();
        Vortex::RenderCommand::SetViewport(width, height);
        Vortex::RenderCommand::SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});
        Vortex::RenderCommand::ConfigureStencilTesting(false, 0x11, 0x11, Vortex::RendererAPI::StencilTestFunc::ALWAYS, 0x11, Vortex::RendererAPI::StencilTestAction::KEEP,
                                                       Vortex::RendererAPI::StencilTestAction::KEEP, Vortex::RendererAPI::StencilTestAction::KEEP);
        Vortex::RenderCommand::ConfigureBlending(true, Vortex::RendererAPI::BlendingFunc::SRC_ALPHA, Vortex::RendererAPI::BlendingFunc::ONE_MINUS_SRC_ALPHA);
    }

    bool ShouldClose() {
        return glfwWindowShouldClose(m_Window);
    }

    void Update() {
        Vortex::Renderer::BeginScene();
        {
            auto phongShader = m_ShaderLibrary->Get("Phong");

            phongShader->Bind();
            phongShader->SetMatrix4("u_ViewProj", m_Camera->GetViewProj());
            phongShader->SetFloat3("u_ViewPos", m_Camera->GetPosition());
            phongShader->SetFloat("u_Material.shininess", 32.0f);

            phongShader->SetInt("u_NumPointLights", 4);
            phongShader->SetInt("u_NumSpotLights", 0);
            phongShader->SetInt("u_NumDirectionalLights", 0);

            // point light 1
            phongShader->SetFloat3("u_PointLights[0].position", pointLightPositions[0]);
            phongShader->SetFloat3("u_PointLights[0].ambient", {0.05f, 0.05f, 0.05f});
            phongShader->SetFloat3("u_PointLights[0].diffuse", {0.8f, 0.8f, 0.8f});
            phongShader->SetFloat3("u_PointLights[0].specular", {1.0f, 1.0f, 1.0f});
            phongShader->SetFloat("u_PointLights[0].constant", 1.0f);
            phongShader->SetFloat("u_PointLights[0].linear", 0.09f);
            phongShader->SetFloat("u_PointLights[0].quadratic", 0.032f);
            // point light 2
            phongShader->SetFloat3("u_PointLights[1].position", pointLightPositions[1]);
            phongShader->SetFloat3("u_PointLights[1].ambient", {0.05f, 0.05f, 0.05f});
            phongShader->SetFloat3("u_PointLights[1].diffuse", {0.8f, 0.8f, 0.8f});
            phongShader->SetFloat3("u_PointLights[1].specular", {1.0f, 1.0f, 1.0f});
            phongShader->SetFloat("u_PointLights[1].constant", 1.0f);
            phongShader->SetFloat("u_PointLights[1].linear", 0.09f);
            phongShader->SetFloat("u_PointLights[1].quadratic", 0.032f);
            // point light 3
            phongShader->SetFloat3("u_PointLights[2].position", pointLightPositions[2]);
            phongShader->SetFloat3("u_PointLights[2].ambient", {0.05f, 0.05f, 0.05f});
            phongShader->SetFloat3("u_PointLights[2].diffuse", {0.8f, 0.8f, 0.8f});
            phongShader->SetFloat3("u_PointLights[2].specular", {1.0f, 1.0f, 1.0f});
            phongShader->SetFloat("u_PointLights[2].constant", 1.0f);
            phongShader->SetFloat("u_PointLights[2].linear", 0.09f);
            phongShader->SetFloat("u_PointLights[2].quadratic", 0.032f);
            // point light 4
            phongShader->SetFloat3("u_PointLights[3].position", pointLightPositions[3]);
            phongShader->SetFloat3("u_PointLights[3].ambient", {0.05f, 0.05f, 0.05f});
            phongShader->SetFloat3("u_PointLights[3].diffuse", {0.8f, 0.8f, 0.8f});
            phongShader->SetFloat3("u_PointLights[3].specular", {1.0f, 1.0f, 1.0f});
            phongShader->SetFloat("u_PointLights[3].constant", 1.0f);
            phongShader->SetFloat("u_PointLights[3].linear", 0.09f);
            phongShader->SetFloat("u_PointLights[3].quadratic", 0.032f);

            phongShader->SetMatrix4("u_ViewProj", m_Camera->GetViewProj());

            glm::mat4 model = glm::mat4(1.0f);
            phongShader->SetMatrix4("u_Model", model);

            for (auto mesh : m_Model->GetMeshes()) {
                mesh.BindTextures(phongShader);
                Vortex::Renderer::Submit(mesh.GetVertexArray());
            }
        }
        Vortex::Renderer::EndScene();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Settings");
        ImGui::Text("Depth testing");
        ImGui::Checkbox("Enable", &m_EnableDepthTest);
        ImGui::Checkbox("Mask", &m_EnableDepthMask);
        ImGui::InputInt("Func", &m_DepthTestFunc);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        Vortex::RenderCommand::ConfigureDepthTesting(m_EnableDepthTest, m_EnableDepthMask, (Vortex::RendererAPI::DepthTestFunc) m_DepthTestFunc);

        if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_REPEAT)
            m_Camera->MoveFront(0.02f);
        if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_REPEAT)
            m_Camera->MoveFront(-0.02f);
        if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_REPEAT)
            m_Camera->MoveSideways(-0.02f);
        if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_REPEAT)
            m_Camera->MoveSideways(0.02f);
        if (glfwGetKey(m_Window, GLFW_KEY_SPACE) == GLFW_PRESS || glfwGetKey(m_Window, GLFW_KEY_SPACE) == GLFW_REPEAT)
            m_Camera->MoveUp(0.02f);
        if (glfwGetKey(m_Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(m_Window, GLFW_KEY_LEFT_SHIFT) == GLFW_REPEAT)
            m_Camera->MoveUp(-0.02f);
        if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        if (glfwGetKey(m_Window, GLFW_KEY_ENTER) == GLFW_PRESS)
            glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if (glfwGetKey(m_Window, GLFW_KEY_Q) == GLFW_PRESS)
            glfwSetWindowShouldClose(m_Window, true);

        m_Context->SwapBuffers();
        Vortex::RenderCommand::Clear(Vortex::RendererAPI::ClearBuffer::COLOR);
        Vortex::RenderCommand::Clear(Vortex::RendererAPI::ClearBuffer::DEPTH);
        glfwPollEvents();
    }

    ~VortexDemo() {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

private:
    static void GlfwErrorCallback(int error, const char* description) {
        std::cout << "GLFW Error (" << error << "): " << description << std::endl;
        exit(1);
    }

    static void OnResize(GLFWwindow*, int width, int height) {
        Vortex::RenderCommand::SetViewport(width, height);
    }

    static void OnMouseMove(GLFWwindow* window, double mouseX, double mouseY) {
        double deltaX = mouseX - m_LastMouseX;
        double deltaY = mouseY - m_LastMouseY;
        m_LastMouseX = mouseX;
        m_LastMouseY = mouseY;
        if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
            m_Camera->OnMouseMoved(deltaX, deltaY);
    }

    static double m_LastMouseX;
    static double m_LastMouseY;
    GLFWwindow* m_Window;
    Vortex::BufferLayout m_BufferLayout;

    static std::shared_ptr<FPSCamera> m_Camera;
    std::shared_ptr<Vortex::Context> m_Context;
    std::shared_ptr<Vortex::ShaderLibrary> m_ShaderLibrary;

    std::shared_ptr<Vortex::Scene::Model> m_Model;

    bool m_EnableDepthTest = true;
    bool m_EnableDepthMask = true;
    int m_DepthTestFunc = 2;
};

double VortexDemo::m_LastMouseX = 0;
double VortexDemo::m_LastMouseY = 0;
std::shared_ptr<FPSCamera> VortexDemo::m_Camera;

int main() {
    VortexDemo demo;
    while (!demo.ShouldClose()) {
        demo.Update();
    }
    return 0;
}
