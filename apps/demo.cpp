#include <GLFW/glfw3.h>
#include <Vortex/Vortex.hpp>

#include <glm/ext/matrix_transform.hpp>

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

float cubeVertices[] = {
    -0.5f, -0.5f, -0.5f, // 1
    0.5f,  -0.5f, -0.5f, // 2
    0.5f,  0.5f,  -0.5f, // 3
    0.5f,  0.5f,  -0.5f, // 4
    -0.5f, 0.5f,  -0.5f, // 5
    -0.5f, -0.5f, -0.5f, // 6

    -0.5f, -0.5f, 0.5f, // 7
    0.5f,  -0.5f, 0.5f, // 8
    0.5f,  0.5f,  0.5f, // 9
    0.5f,  0.5f,  0.5f, // 10
    -0.5f, 0.5f,  0.5f, // 11
    -0.5f, -0.5f, 0.5f, // 12

    -0.5f, 0.5f,  0.5f,  // 13
    -0.5f, 0.5f,  -0.5f, // 14
    -0.5f, -0.5f, -0.5f, // 15
    -0.5f, -0.5f, -0.5f, // 16
    -0.5f, -0.5f, 0.5f,  // 17
    -0.5f, 0.5f,  0.5f,  // 18

    0.5f,  0.5f,  0.5f,  // 19
    0.5f,  0.5f,  -0.5f, // 20
    0.5f,  -0.5f, -0.5f, // 21
    0.5f,  -0.5f, -0.5f, // 22
    0.5f,  -0.5f, 0.5f,  // 23
    0.5f,  0.5f,  0.5f,  // 24

    -0.5f, -0.5f, -0.5f, // 25
    0.5f,  -0.5f, -0.5f, // 26
    0.5f,  -0.5f, 0.5f,  // 27
    0.5f,  -0.5f, 0.5f,  // 28
    -0.5f, -0.5f, 0.5f,  // 29
    -0.5f, -0.5f, -0.5f, // 30

    -0.5f, 0.5f,  -0.5f, // 31
    0.5f,  0.5f,  -0.5f, // 32
    0.5f,  0.5f,  0.5f,  // 33
    0.5f,  0.5f,  0.5f,  // 34
    -0.5f, 0.5f,  0.5f,  // 35
    -0.5f, 0.5f,  -0.5f  // 36
};

uint32_t cubeIndices[] = {0,  1,  2,   // 1
                          3,  4,  5,   // 2
                          6,  7,  8,   // 3
                          9,  10, 11,  // 4
                          12, 13, 14,  // 5
                          15, 16, 17,  // 6
                          18, 19, 20,  // 7
                          21, 22, 23,  // 8
                          24, 25, 26,  // 9
                          27, 28, 29,  // 10
                          30, 31, 32,  // 11
                          33, 34, 35}; // 12

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

        m_BufferLayout = Vortex::BufferLayout({
            {Vortex::ShaderDataType::Float3, "Position", false},
        });

        m_CubeVertexBuffer = Vortex::VertexBufferCreate(cubeVertices, sizeof(cubeVertices));
        m_CubeIndexBuffer = Vortex::IndexBufferCreate(cubeIndices, sizeof(cubeIndices));
        m_CubeVertexArray = Vortex::VertexArrayCreate();
        m_CubeVertexBuffer->SetLayout(m_BufferLayout);
        m_CubeVertexArray->AddVertexBuffer(m_CubeVertexBuffer);
        m_CubeVertexArray->SetIndexBuffer(m_CubeIndexBuffer);

        m_LightVertexBuffer = Vortex::VertexBufferCreate(cubeVertices, sizeof(cubeVertices));
        m_LightIndexBuffer = Vortex::IndexBufferCreate(cubeIndices, sizeof(cubeIndices));
        m_LightVertexArray = Vortex::VertexArrayCreate();
        m_LightVertexBuffer->SetLayout(m_BufferLayout);
        m_LightVertexArray->AddVertexBuffer(m_LightVertexBuffer);
        m_LightVertexArray->SetIndexBuffer(m_LightIndexBuffer);

        m_LightPos = glm::vec3(1.2f, 1.0f, 2.0f);
        m_LightTransform = glm::mat4(1.0f);
        m_LightTransform = glm::translate(m_LightTransform, m_LightPos);
        m_LightTransform = glm::scale(m_LightTransform, glm::vec3(0.2f));
        m_CubeTransform = glm::mat4(1.0f);

        m_ShaderLibrary = std::make_shared<Vortex::ShaderLibrary>();
        m_ShaderLibrary->Load("../../apps/assets/Shaders/Base.glsl");

        Vortex::Renderer::Init();
        Vortex::RenderCommand::SetViewport(width, height);
        Vortex::RenderCommand::SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});
    }

    bool ShouldClose() {
        return glfwWindowShouldClose(m_Window);
    }

    void Update() {
        Vortex::Renderer::BeginScene();
        {
            auto baseShader = m_ShaderLibrary->Get("Base");
            baseShader->Bind();
            baseShader->SetMatrix4("u_ViewProj", m_Camera->GetViewProj());
            baseShader->SetMatrix4("u_Model", m_CubeTransform);
            baseShader->SetFloat3("u_Color", {1.0f, 0.0f, 1.0f});
            Vortex::Renderer::Submit(m_CubeVertexArray);

            baseShader->Bind();
            baseShader->SetMatrix4("u_ViewProj", m_Camera->GetViewProj());
            baseShader->SetMatrix4("u_Model", m_LightTransform);
            baseShader->SetFloat3("u_Color", {1.0f, 1.0f, 1.0f});
            Vortex::Renderer::Submit(m_LightVertexArray);
        }
        Vortex::Renderer::EndScene();

        if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_REPEAT)
            m_Camera->MoveFront(0.02f);
        if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_REPEAT)
            m_Camera->MoveFront(-0.02f);
        if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_REPEAT)
            m_Camera->MoveSideways(-0.02f);
        if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_REPEAT)
            m_Camera->MoveSideways(0.02f);
        if (glfwGetKey(m_Window, GLFW_KEY_Q) == GLFW_PRESS)
            glfwSetWindowShouldClose(m_Window, true);

        m_Context->SwapBuffers();
        Vortex::RenderCommand::Clear();
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

    static void OnMouseMove(GLFWwindow*, double mouseX, double mouseY) {
        double deltaX = mouseX - m_LastMouseX;
        double deltaY = mouseY - m_LastMouseY;
        m_LastMouseX = mouseX;
        m_LastMouseY = mouseY;
        m_Camera->OnMouseMoved(deltaX, deltaY);
    }

    static double m_LastMouseX;
    static double m_LastMouseY;
    GLFWwindow* m_Window;
    Vortex::BufferLayout m_BufferLayout;

    glm::vec3 m_LightPos;
    glm::mat4 m_LightTransform;
    glm::mat4 m_CubeTransform;

    static std::shared_ptr<FPSCamera> m_Camera;
    std::shared_ptr<Vortex::Context> m_Context;
    std::shared_ptr<Vortex::ShaderLibrary> m_ShaderLibrary;

    std::shared_ptr<Vortex::VertexBuffer> m_CubeVertexBuffer;
    std::shared_ptr<Vortex::IndexBuffer> m_CubeIndexBuffer;
    std::shared_ptr<Vortex::VertexArray> m_CubeVertexArray;

    std::shared_ptr<Vortex::VertexBuffer> m_LightVertexBuffer;
    std::shared_ptr<Vortex::IndexBuffer> m_LightIndexBuffer;
    std::shared_ptr<Vortex::VertexArray> m_LightVertexArray;
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
