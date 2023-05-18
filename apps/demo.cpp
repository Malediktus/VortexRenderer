#include <GLFW/glfw3.h>
#include <Vortex/Vortex.hpp>
#include <Vortex/Scene/Model.hpp>

#include <glm/ext/matrix_transform.hpp>
#include <tracy/Tracy.hpp>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <cstdlib>
#include <iostream>

class Window : public Vortex::Window {
public:
    Window() {
        if (!glfwInit()) {
            std::cout << "Failed to init GLFW!" << std::endl;
            exit(1);
        }

        auto api = ChooseRenderingAPI();

        if (api == Vortex::RendererAPI::API::OpenGL) {
            glfwWindowHint(GLFW_SAMPLES, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        } else {
            std::cout << "Invalid renderer api!" << std::endl;
            exit(1);
        }

        m_GLFWWindow = glfwCreateWindow(1280, 720, "Vortex Renderer Demo", nullptr, nullptr);
        if (!m_GLFWWindow) {
            std::cout << "Failed to create window!" << std::endl;
            exit(1);
        }
    }

    ~Window() {
        glfwDestroyWindow(m_GLFWWindow);
        glfwTerminate();
    }

    void SetupOpenglContext() override {
        glfwMakeContextCurrent(m_GLFWWindow);
        glfwSwapInterval(0);
    }

    GLFWwindow* GetGLFWWindow() {
        return m_GLFWWindow;
    }

private:
    GLFWwindow* m_GLFWWindow;
};

class VortexDemo {
public:
    VortexDemo() {
        ZoneScoped;

        m_Window = std::make_shared<Window>();

        Vortex::ProjectInformation project;
        project.ProjectName = "Vortex Demo";
        project.ProjectVersion = glm::vec3(1, 0, 0);

        m_Context = Vortex::ContextCreate(m_Window, project);
        m_Context->Init();

        m_PerfCounterFrequency = glfwGetTimerFrequency();
        m_LastCounter = glfwGetTimerValue();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void) io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui::StyleColorsDark();
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        ImGui_ImplGlfw_InitForOpenGL(m_Window->GetGLFWWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 150");

        Vortex::Renderer::SetContext(m_Context);
        Vortex::RenderCommand::Init();
        Vortex::RenderCommand::ConfigureAntiAliasing(true);

        m_PrimaryRenderer = std::make_shared<Vortex::Renderer>(Vortex::ShaderCreate("../../apps/assets/Shaders/PBR.glsl"), 1280, 720, false);
        m_PrimaryViewportWidth = 1280;
        m_PrimaryViewportHeight = 720;
        m_PrimaryCamera = std::make_shared<Vortex::Camera>(90.0f, 1280.0f, 720.0f);
        m_PrimaryCamera->Translate(glm::vec3(0.0f, 9.0f, 20.0f));

        glm::vec3 lightPositions[] = {glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(3.0f, 0.0f, 0.0f)};
        glm::vec3 lightColors[] = {glm::vec3(0.25), glm::vec3(0.50), glm::vec3(0.75), glm::vec3(1.00)};

        auto mesh = std::make_shared<Vortex::Mesh>("../../apps/assets/Objects/Cube/Cube.obj");
        m_Scene = std::make_shared<Vortex::Scene>();

        std::shared_ptr<Vortex::Object> meshObject = std::make_shared<Vortex::Object>(glm::scale(glm::mat4(1.0f), glm::vec3(50.0f, 0.05f, 50.0f)));
        meshObject->Attach(mesh);
        m_Scene->Append(meshObject);

        auto cube1 = std::make_shared<Vortex::Mesh>("../../apps/assets/Objects/Cube/Cube.obj");
        std::shared_ptr<Vortex::Object> cube1Object = std::make_shared<Vortex::Object>(glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 8.0f, 0.0f)));
        cube1Object->Attach(cube1);
        m_Scene->Append(cube1Object);

        for (int i = 0; i < 4; i++) {
            std::shared_ptr<Vortex::PointLight> light = std::make_shared<Vortex::PointLight>(1.0f, 1.0f, 1.0f, lightColors[i], lightColors[i], lightColors[i]);
            std::shared_ptr<Vortex::Object> lightObject = std::make_shared<Vortex::Object>(glm::translate(glm::mat4(1.0f), lightPositions[i]));
            lightObject->Attach(light);
            m_Scene->Append(lightObject);
        }
    }

    void Update() {
        ZoneScoped;

        glfwGetWindowSize(m_Window->GetGLFWWindow(), (int*) &m_PrimaryViewportWidth, (int*) &m_PrimaryViewportHeight);

        if (m_PrimaryViewportWidth < 0)
            m_PrimaryViewportWidth = 0;
        if (m_PrimaryViewportHeight < 0)
            m_PrimaryViewportHeight = 0;

        m_PrimaryRenderer->OnResize(m_PrimaryViewportWidth, m_PrimaryViewportHeight);
        m_PrimaryCamera->Resize(90.0f, (float) m_PrimaryViewportWidth, (float) m_PrimaryViewportHeight);

        m_PrimaryRenderer->BeginFrame();
        m_PrimaryRenderer->Submit(m_Scene, m_PrimaryCamera);
        m_PrimaryRenderer->EndFrame();

        {
            ZoneScopedN("ImGuiCollectAndrender");
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::Begin("Render Stats");
            ImGui::Text("FPS: %i", m_FPS);
            ImGui::End();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
                GLFWwindow* backup_current_context = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup_current_context);
            }
        }
        {
            ZoneScopedN("SwapBuffer");
            glfwSwapBuffers(m_Window->GetGLFWWindow());
        }
        {
            ZoneScopedN("PollEvents");
            glfwPollEvents();
        }
        {
            ZoneScopedN("CalculateFPS");
            uint64_t endCounter = glfwGetTimerValue();
            uint64_t counterElapsed = endCounter - m_LastCounter;
            m_Delta = ((float) counterElapsed) / (float) m_PerfCounterFrequency;
            m_FPS = (uint32_t) ((float) m_PerfCounterFrequency / (float) counterElapsed);
            m_LastCounter = endCounter;
        }
    }

    bool ShouldClose() {
        return glfwWindowShouldClose(m_Window->GetGLFWWindow());
    }

    ~VortexDemo() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        m_Context->Destroy();
    }

private:
    uint64_t m_PerfCounterFrequency;
    uint64_t m_LastCounter;
    float m_Delta = 0.0f;
    uint32_t m_FPS = 0;

    uint32_t m_PrimaryViewportWidth, m_PrimaryViewportHeight;
    std::shared_ptr<Window> m_Window;
    std::shared_ptr<Vortex::Renderer> m_PrimaryRenderer;
    std::shared_ptr<Vortex::Camera> m_PrimaryCamera;
    std::shared_ptr<Vortex::Scene> m_Scene;
    std::shared_ptr<Vortex::Context> m_Context;
};

int main() {
    VortexDemo demo;
    while (!demo.ShouldClose()) {
        demo.Update();
    }
    return 0;
}
