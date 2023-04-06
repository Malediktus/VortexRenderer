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

        m_Window = glfwCreateWindow(1280, 900, "Vortex Renderer Demo", nullptr, nullptr);
        if (!m_Window) {
            std::cout << "Failed to create window!" << std::endl;
            exit(1);
        }

        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(m_Window, OnMouseMove);
        glfwGetCursorPos(m_Window, &m_LastMouseX, &m_LastMouseY);

        m_Camera = std::make_shared<FPSCamera>(90.0f, 1280, 720);
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

        auto renderbuffer = Vortex::RenderbufferCreate(1280, 720, Vortex::Renderbuffer::RenderbufferType::DEPTH24_STENCIL8);

        m_Texture = Vortex::Texture2DCreate(1280, 720);
        m_Framebuffer = Vortex::FramebufferCreate();
        m_Framebuffer->Bind();
        m_Framebuffer->AttachColorBuffer(m_Texture);
        m_Framebuffer->AttachDepthStencilBuffer(renderbuffer);
        m_Framebuffer->Unbind();

        Vortex::Renderer::Init("assets/Shaders/Base.glsl");
        Vortex::RenderCommand::SetViewport(1280, 720);
        Vortex::RenderCommand::SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});
        Vortex::RenderCommand::ConfigureStencilTesting(false, 0x11, 0x11, Vortex::RendererAPI::StencilTestFunc::ALWAYS, 0x11, Vortex::RendererAPI::StencilTestAction::KEEP,
                                                       Vortex::RendererAPI::StencilTestAction::KEEP, Vortex::RendererAPI::StencilTestAction::KEEP);
        Vortex::RenderCommand::ConfigureBlending(true, Vortex::RendererAPI::BlendingFunc::SRC_ALPHA, Vortex::RendererAPI::BlendingFunc::ONE_MINUS_SRC_ALPHA,
                                                 Vortex::RendererAPI::BlendingFunc::ONE, Vortex::RendererAPI::BlendingFunc::ZERO, Vortex::RendererAPI::BlendingFunc::ONE,
                                                 Vortex::RendererAPI::BlendingFunc::ZERO);
    }

    bool ShouldClose() {
        return glfwWindowShouldClose(m_Window);
    }

    void Update() {
        m_Framebuffer->Bind();
        Vortex::RenderCommand::Clear(Vortex::RendererAPI::ClearBuffer::COLOR);
        Vortex::RenderCommand::Clear(Vortex::RendererAPI::ClearBuffer::DEPTH);

        std::shared_ptr<Vortex::Scene> scene = std::make_shared<Vortex::Scene>(m_Camera);
        std::shared_ptr<Vortex::Mesh> mesh = std::make_shared<Vortex::Mesh>("assets/Objects/Monkey/monkey.obj");
        std::shared_ptr<Vortex::Object> object = std::make_shared<Vortex::Object>();
        object->Attach(mesh);
        scene->Append(object);

        Vortex::Renderer::BeginFrame();
        Vortex::Renderer::Submit(scene);
        Vortex::Renderer::EndFrame();
        m_Framebuffer->Unbind();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Settings");
        ImGui::Text("Depth testing");
        ImGui::Checkbox("EnableDepthTest", &m_EnableDepthTest);
        ImGui::Checkbox("DepthTestMask", &m_EnableDepthMask);
        ImGui::InputInt("DepthTestFunc", &m_DepthTestFunc);

        ImGui::Text("Culling");
        ImGui::Checkbox("EnableCulling", &m_EnableCulling);
        ImGui::InputInt("CullingType", &m_CullingType);
        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Viewport");
        ImGui::Image(*(ImTextureID*) m_Texture->GetNative(), ImVec2(1280, 720), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        Vortex::RenderCommand::ConfigureDepthTesting(m_EnableDepthTest, m_EnableDepthMask, (Vortex::RendererAPI::DepthTestFunc) m_DepthTestFunc);
        Vortex::RenderCommand::ConfigureCulling(m_EnableCulling, (Vortex::RendererAPI::CullingType) m_CullingType);

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

    static std::shared_ptr<FPSCamera> m_Camera;
    std::shared_ptr<Vortex::Context> m_Context;

    std::shared_ptr<Vortex::Framebuffer> m_Framebuffer;
    std::shared_ptr<Vortex::Texture2D> m_Texture;

    bool m_EnableDepthTest = true;
    bool m_EnableDepthMask = true;
    int m_DepthTestFunc = 2;
    bool m_EnableCulling = false;
    int m_CullingType = 0;
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
