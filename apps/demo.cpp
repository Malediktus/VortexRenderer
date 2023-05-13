#define GLFW_INCLUDE_VULKAN
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

#if 0
        auto api = ChooseRenderingAPI(glfwVulkanSupported());
#else
        auto api = ChooseRenderingAPI(false);
#endif

        if (api == Vortex::RendererAPI::API::OpenGL) {
            glfwWindowHint(GLFW_SAMPLES, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        } else if (api == Vortex::RendererAPI::API::Vulkan) {
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
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

    const std::vector<const char*> GetVulkanInstanceExtensions() override {
        std::vector<const char*> result;
        uint32_t count;
        const char** extensions = glfwGetRequiredInstanceExtensions(&count);

        for (uint32_t i = 0; i < count; i++) {
            result.push_back(extensions[i]);
        }

        return result;
    }

    void* GetSurface(void* instance) override {
        VkSurfaceKHR surface;
        VkResult err = glfwCreateWindowSurface((VkInstance) instance, m_GLFWWindow, NULL, &surface);
        if (err) {
            std::cout << "Failed to create window surface!" << std::endl;
            exit(1);
        }
        return (void*) surface;
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

        Vortex::Renderer::SetContext(m_Context);
        Vortex::RenderCommand::Init();
        Vortex::RenderCommand::ConfigureAntiAliasing(true);

        m_PrimaryRenderer = std::make_shared<Vortex::Renderer>("../../apps/assets/Shaders/Light.glsl", 1280, 720, true);
        m_SecondaryRenderer = std::make_shared<Vortex::Renderer>("../../apps/assets/Shaders/Light.glsl", 1280, 720, true);
        m_PrimaryViewportWidth = 1080;
        m_PrimaryViewportHeight = 720;
        m_SecondaryViewportWidth = 1080;
        m_SecondaryViewportHeight = 720;
        m_PrimaryCamera = std::make_shared<Vortex::Camera>(90.0f, 1280.0f, 720.0f);
        m_SecondaryCamera = std::make_shared<Vortex::Camera>(90.0f, 1280.0f, 720.0f);

        auto mesh = std::make_shared<Vortex::Mesh>("../../apps/assets/Objects/Monkey/monkey.obj");
        m_Scene = std::make_shared<Vortex::Scene>();

        glm::mat4 meshTransform(1.0f);
        meshTransform = glm::translate(meshTransform, glm::vec3(0.0f, 0.0f, -4.0f));
        std::shared_ptr<Vortex::Object> meshObject = std::make_shared<Vortex::Object>(meshTransform);
        meshObject->Attach(mesh);

        glm::mat4 lightTransform(1.0f);
        lightTransform = glm::translate(lightTransform, glm::vec3(1.0f, 0.5f, -3.0f));

        std::shared_ptr<Vortex::PointLight> light = std::make_shared<Vortex::PointLight>(1.0f, 1.0f, 1.0f, glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f));
        std::shared_ptr<Vortex::Object> lightObject = std::make_shared<Vortex::Object>(lightTransform);
        lightObject->Attach(light);

        m_Scene->Append(meshObject);
        m_Scene->Append(lightObject);

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
    }

    void Update() {
        ZoneScoped;

        Vortex::RenderCommand::Clear(Vortex::RendererAPI::ClearBuffer::COLOR);
        Vortex::RenderCommand::Clear(Vortex::RendererAPI::ClearBuffer::DEPTH);

        {
            ZoneScopedN("ImGuiCollectAndrender");
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
                        glfwSetWindowShouldClose(m_Window->GetGLFWWindow(), true);
                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }

            ImGui::Begin("Render Stats");
            ImGui::Checkbox("Slow FPS", &m_ShowSlowFPS);
            if (m_ShowSlowFPS)
                ImGui::Text("FPS: %i", m_SlowFPS);
            else
                ImGui::Text("FPS: %i", m_FPS);
            ImGui::End();

            ImGui::Begin("PrimaryViewport");
            {
                m_PrimaryViewportWidth = (uint32_t) ImGui::GetContentRegionAvail().x;
                m_PrimaryViewportHeight = (uint32_t) ImGui::GetContentRegionAvail().y;
                auto texture = m_PrimaryRenderer->GetTexture();
                ImGui::Image(*(ImTextureID*) texture->GetNative(), ImVec2((float) texture->GetWidth(), (float) texture->GetHeight()), ImVec2(0, 1), ImVec2(1, 0));
            }
            ImGui::End();

            ImGui::Begin("SecondaryViewport");
            {
                m_SecondaryViewportWidth = (uint32_t) ImGui::GetContentRegionAvail().x;
                m_SecondaryViewportHeight = (uint32_t) ImGui::GetContentRegionAvail().y;
                auto texture = m_SecondaryRenderer->GetTexture();
                ImGui::Image(*(ImTextureID*) texture->GetNative(), ImVec2((float) texture->GetWidth(), (float) texture->GetHeight()), ImVec2(0, 1), ImVec2(1, 0));
            }
            ImGui::End();

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

        if (m_PrimaryViewportWidth < 0)
            m_PrimaryViewportWidth = 0;
        if (m_PrimaryViewportHeight < 0)
            m_PrimaryViewportHeight = 0;
        if (m_SecondaryViewportWidth < 0)
            m_SecondaryViewportWidth = 0;
        if (m_SecondaryViewportHeight < 0)
            m_SecondaryViewportHeight = 0;

        m_PrimaryRenderer->OnResize(m_PrimaryViewportWidth, m_PrimaryViewportHeight);
        m_PrimaryCamera->Resize(90.0f, (float) m_PrimaryViewportWidth, (float) m_PrimaryViewportHeight);

        m_PrimaryRenderer->BeginFrame();
        m_PrimaryRenderer->Submit(m_Scene, m_PrimaryCamera);
        m_PrimaryRenderer->EndFrame();

        m_SecondaryRenderer->OnResize(m_SecondaryViewportWidth, m_SecondaryViewportHeight);
        m_SecondaryCamera->Resize(90.0f, (float) m_SecondaryViewportWidth, (float) m_SecondaryViewportHeight);

        m_SecondaryRenderer->BeginFrame();
        m_SecondaryRenderer->Submit(m_Scene, m_SecondaryCamera);
        m_SecondaryRenderer->EndFrame();

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
            m_FrameCount++;
        }
        if (m_FrameCount >= 1000) {
            m_SlowFPS = m_FPS;
            m_FrameCount = 0;
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
    uint32_t m_FrameCount = 0;
    float m_Delta = 0.0f;
    uint32_t m_FPS = 0;
    uint32_t m_SlowFPS = 0;
    bool m_ShowSlowFPS = true;

    uint32_t m_PrimaryViewportWidth, m_PrimaryViewportHeight;
    uint32_t m_SecondaryViewportWidth, m_SecondaryViewportHeight;

    std::shared_ptr<Window> m_Window;
    std::shared_ptr<Vortex::Renderer> m_PrimaryRenderer;
    std::shared_ptr<Vortex::Renderer> m_SecondaryRenderer;
    std::shared_ptr<Vortex::Camera> m_PrimaryCamera;
    std::shared_ptr<Vortex::Camera> m_SecondaryCamera;
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
