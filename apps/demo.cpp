#include <GLFW/glfw3.h>
#include <Vortex/Vortex.hpp>
#include <Vortex/Scene/Model.hpp>

#include <glm/ext/matrix_transform.hpp>
#include <tracy/Tracy.hpp>

#include <cstdlib>
#include <iostream>

class VortexDemo {
public:
    VortexDemo() {
        ZoneScoped;
        SetupGlfw();

        Vortex::Renderer::Init(m_Window, "../../apps/assets/Shaders/Light.glsl", 1280, 720);
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

        Vortex::Renderer::BeginFrame();
        Vortex::Renderer::Submit(scene);
        Vortex::Renderer::EndFrame();

        {
            ZoneScopedN("SwapBuffer");
            glfwSwapBuffers(m_Window);
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
            glfwSetWindowTitle(m_Window, title.c_str());
            m_FrameCount = 0;
        }
    }

    bool ShouldClose() {
        return glfwWindowShouldClose(m_Window);
    }

    void SetupGlfw() {
        ZoneScoped;
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
    }

    ~VortexDemo() {
        Vortex::Renderer::Shutdown();
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

private:
    static void GlfwErrorCallback(int error, const char* description) {
        std::cout << "GLFW Error (" << error << "): " << description << std::endl;
        exit(1);
    }

    GLFWwindow* m_Window;
    uint64_t m_PerfCounterFrequency;
    uint64_t m_LastCounter;
    uint32_t m_FrameCount = 0;
    float m_Delta = 0.0f;
    uint32_t m_FPS = 0.0f;

    std::shared_ptr<Vortex::Camera> m_Camera;
    std::shared_ptr<Vortex::Mesh> m_MonkeyMesh;
};

int main() {
    VortexDemo demo;
    while (!demo.ShouldClose()) {
        demo.Update();
    }
    return 0;
}
