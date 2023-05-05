#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <Vortex/Vortex.hpp>
#include <Vortex/Scene/Model.hpp>

#include <glm/ext/matrix_transform.hpp>
#include <tracy/Tracy.hpp>

#include <cstdlib>
#include <iostream>

class Window : public Vortex::Window {
public:
    Window() {
        if (!glfwInit()) {
            std::cout << "Failed to init GLFW!" << std::endl;
            exit(1);
        }

        auto api = ChooseRenderingAPI(glfwVulkanSupported());

        if (api == Vortex::RendererAPI::API::OpenGL) {
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

        Vortex::ProjectRequirements requirements;
        requirements.DiscreteGPU = 30000;
        requirements.IntegratedGPU = 5000;
        requirements.NoGPU = 0;
        requirements.MinHeapMegabyte = 256;
        requirements.HeapMegabyte = 1;
        requirements.GeometryShader = 0;
        requirements.TeslationShader = 0;
        requirements.SamplerAnisotropy = 0;

        m_Context = Vortex::ContextCreate(m_Window, requirements);
        m_Context->Init();

        Vortex::Renderer::SetContext(m_Context);
        Vortex::RenderCommand::Init();

        m_Renderer = std::make_shared<Vortex::Renderer>("../../apps/assets/Shaders/Light.glsl", 1280, 720, false);
        m_Camera = std::make_shared<Vortex::Camera>(90.0f, 1280, 720);
        m_MonkeyMesh = std::make_shared<Vortex::Mesh>("../../apps/assets/Objects/Monkey/monkey.obj");

        // glfwSwapInterval(0);
        m_PerfCounterFrequency = glfwGetTimerFrequency();
        m_LastCounter = glfwGetTimerValue();
    }

    void Update() {
        ZoneScoped;
        auto scene = std::make_shared<Vortex::Scene>(m_Camera);

        glm::mat4 meshTransform(1.0f);
        meshTransform = glm::translate(meshTransform, glm::vec3(0.0f, 0.0f, -4.0f));
        std::shared_ptr<Vortex::Object> meshObject = std::make_shared<Vortex::Object>(meshTransform);
        meshObject->Attach(m_MonkeyMesh);

        glm::mat4 lightTransform(1.0f);
        lightTransform = glm::translate(lightTransform, glm::vec3(1.0f, 0.5f, -3.0f));

        std::shared_ptr<Vortex::PointLight> light = std::make_shared<Vortex::PointLight>(1.0f, 1.0f, 1.0f, glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f));
        std::shared_ptr<Vortex::Object> lightObject = std::make_shared<Vortex::Object>(lightTransform);
        lightObject->Attach(light);

        scene->Append(meshObject);
        scene->Append(lightObject);

        m_Renderer->BeginFrame();
        m_Renderer->Submit(scene);
        m_Renderer->EndFrame();

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
            ZoneScopedN("UpdateTitle");
            std::string title = "Vortex Renderer Demo FPS: ";
            title.append(std::to_string(m_FPS));
            glfwSetWindowTitle(m_Window->GetGLFWWindow(), title.c_str());
            m_FrameCount = 0;
        }
    }

    bool ShouldClose() {
        return glfwWindowShouldClose(m_Window->GetGLFWWindow());
    }

    ~VortexDemo() {
        m_Context->Destroy();
    }

private:
    uint64_t m_PerfCounterFrequency;
    uint64_t m_LastCounter;
    uint32_t m_FrameCount = 0;
    float m_Delta = 0.0f;
    uint32_t m_FPS = 0.0f;

    std::shared_ptr<Window> m_Window;
    std::shared_ptr<Vortex::Renderer> m_Renderer;
    std::shared_ptr<Vortex::Camera> m_Camera;
    std::shared_ptr<Vortex::Mesh> m_MonkeyMesh;
    std::shared_ptr<Vortex::Context> m_Context;
};

int main() {
    VortexDemo demo;
    while (!demo.ShouldClose()) {
        demo.Update();
    }
    return 0;
}
