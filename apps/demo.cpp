#include <GLFW/glfw3.h>
#include <Vortex/Vortex.hpp>
#include <Vortex/Scene/Model.hpp>

#include <glm/ext/matrix_transform.hpp>
#include <tracy/Tracy.hpp>

#include <cstdlib>
#include <iostream>

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
        ZoneScoped;
        SetupGlfw();

        Vortex::Renderer::Init(m_Window, "../../apps/assets/Shaders/Light.glsl", 1280, 720);
        m_Camera = std::make_shared<FPSCamera>(90.0f, 1280, 720);
        m_Camera->MoveFront(-5.0f);
        m_MonkeyMesh = std::make_shared<Vortex::Mesh>("../../apps/assets/Objects/Monkey/monkey.obj");

        glfwSwapInterval(0);
        m_PerfCounterFrequency = glfwGetTimerFrequency();
        m_LastCounter = glfwGetTimerValue();
    }

    void Update() {
        ZoneScoped;
        auto scene = std::make_shared<Vortex::Scene>(m_Camera);

        std::shared_ptr<Vortex::Object> meshObject = std::make_shared<Vortex::Object>();
        meshObject->Attach(m_MonkeyMesh);

        glm::mat4 lightTransform(1.0f);
        lightTransform = glm::translate(lightTransform, glm::vec3(1.0f, 0.5f, 1.0f));

        std::shared_ptr<Vortex::PointLight> light = std::make_shared<Vortex::PointLight>(1.0f, 1.0f, 1.0f, glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f));
        std::shared_ptr<Vortex::Object> lightObject = std::make_shared<Vortex::Object>(lightTransform);
        lightObject->Attach(light);

        scene->Append(meshObject);
        scene->Append(lightObject);

        Vortex::Renderer::BeginFrame();
        Vortex::Renderer::Submit(scene);
        Vortex::Renderer::EndFrame();

        ProcessKeyEvents();
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

        // glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(m_Window, OnMouseMove);
        glfwGetCursorPos(m_Window, &m_LastMouseX, &m_LastMouseY);
    }

    void ProcessKeyEvents() {
        ZoneScoped;
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
        ZoneScoped;
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
    uint64_t m_PerfCounterFrequency;
    uint64_t m_LastCounter;
    uint32_t m_FrameCount = 0;
    float m_Delta = 0.0f;
    uint32_t m_FPS = 0.0f;

    static std::shared_ptr<FPSCamera> m_Camera;
    std::shared_ptr<Vortex::Mesh> m_MonkeyMesh;
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
