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

        m_Window = glfwCreateWindow(1280, 720, "Vortex Renderer Demo", nullptr, nullptr);
        if (!m_Window) {
            std::cout << "Failed to create window!" << std::endl;
            exit(1);
        }

        // glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(m_Window, OnMouseMove);
        glfwGetCursorPos(m_Window, &m_LastMouseX, &m_LastMouseY);

        m_Camera = std::make_shared<FPSCamera>(90.0f, 1280, 720);
        m_Camera->MoveFront(-5.0f);

        m_Context = Vortex::ContextCreate(m_Window);
        Vortex::Renderer::Init(m_Context, "assets/Shaders/Light.glsl", 1280, 720);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void) io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
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

        m_Scene = std::make_shared<Vortex::Scene>(m_Camera);
        std::shared_ptr<Vortex::Mesh> mesh = std::make_shared<Vortex::Mesh>("assets/Objects/Monkey/monkey.obj");
        std::shared_ptr<Vortex::Object> meshObject = std::make_shared<Vortex::Object>();
        meshObject->Attach(mesh);
        std::shared_ptr<Vortex::PointLight> light =
            std::make_shared<Vortex::PointLight>(1.0f, 1.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        glm::mat4 lightTransform(1.0f);
        lightTransform = glm::translate(lightTransform, glm::vec3(1.0f, 0.5f, 1.0f));
        std::shared_ptr<Vortex::Object> lightObject = std::make_shared<Vortex::Object>(lightTransform);
        lightObject->Attach(light);
        m_Scene->Append(meshObject);
        m_Scene->Append(lightObject);
    }

    bool ShouldClose() {
        return glfwWindowShouldClose(m_Window);
    }

    void Update() {
        if (m_OldViewportWidth != m_ViewportWidth || m_OldViewportHeight != m_ViewportHeight) {
            m_Texture->Resize(m_ViewportWidth, m_ViewportHeight);
            Vortex::Renderer::OnResize(m_ViewportWidth, m_ViewportHeight);
            m_OldViewportWidth = m_ViewportWidth;
            m_OldViewportHeight = m_ViewportHeight;
            m_Camera->Resize(90.0f, m_ViewportWidth, m_ViewportHeight);
        }
        m_Framebuffer->Bind();

        Vortex::Renderer::BeginFrame();
        Vortex::Renderer::Submit(m_Scene);
        Vortex::Renderer::EndFrame();
        m_Framebuffer->Unbind();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        static bool dockspaceOpen = true;
        static bool opt_fullscreen_persistant = true;
        bool opt_fullscreen = opt_fullscreen_persistant;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen) {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Exit"))
                    glfwSetWindowShouldClose(m_Window, true);
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

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
        ImGui::Image(*(ImTextureID*) m_Texture->GetNative(), ImVec2(m_ViewportWidth, m_ViewportHeight), ImVec2(0, 1), ImVec2(1, 0));
        m_ViewportWidth = ImGui::GetWindowSize().x;
        m_ViewportHeight = ImGui::GetWindowSize().y - 19;
        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Not good, but imgui is temporary and only opengl here anyways
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);

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

        glfwSwapBuffers(m_Window);
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
    std::shared_ptr<Vortex::Scene> m_Scene;

    bool m_EnableDepthTest = true;
    bool m_EnableDepthMask = true;
    int m_DepthTestFunc = 2;
    bool m_EnableCulling = false;
    int m_CullingType = 0;

    uint32_t m_ViewportWidth = 1280;
    uint32_t m_ViewportHeight = 720;
    uint32_t m_OldViewportWidth = 1280;
    uint32_t m_OldViewportHeight = 720;
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
