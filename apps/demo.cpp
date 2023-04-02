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

float cubeVertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f,
    0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f, -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f,  1.0f, 1.0f, -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f, 1.0f, 0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 0.0f,
    0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 0.0f, -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 1.0f,
    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f};

uint32_t cubeIndices[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};

glm::vec3 cubePositions[] = {glm::vec3(0.0f, 0.0f, 0.0f),   glm::vec3(2.0f, 5.0f, -15.0f), glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
                             glm::vec3(2.4f, -0.4f, -3.5f), glm::vec3(-1.7f, 3.0f, -7.5f), glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
                             glm::vec3(1.5f, 0.2f, -1.5f),  glm::vec3(-1.3f, 1.0f, -1.5f)};

glm::vec3 pointLightPositions[] = {glm::vec3(0.7f, 0.2f, 2.0f), glm::vec3(2.3f, -3.3f, -4.0f), glm::vec3(-4.0f, 2.0f, -12.0f), glm::vec3(0.0f, 0.0f, -3.0f)};

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
            {Vortex::ShaderDataType::Float3, "Normal", false},
            {Vortex::ShaderDataType::Float2, "TexCoords", false},
        });

        m_CubeVertexBuffer = Vortex::VertexBufferCreate(cubeVertices, sizeof(cubeVertices));
        m_CubeIndexBuffer = Vortex::IndexBufferCreate(cubeIndices, sizeof(cubeIndices));
        m_CubeVertexArray = Vortex::VertexArrayCreate();
        m_CubeDiffuseMap = Vortex::Texture2DCreate("assets/Textures/Box.png");
        m_CubeSpecularMap = Vortex::Texture2DCreate("assets/Textures/BoxSpecular.png");
        m_CubeVertexBuffer->SetLayout(m_BufferLayout);
        m_CubeVertexArray->AddVertexBuffer(m_CubeVertexBuffer);
        m_CubeVertexArray->SetIndexBuffer(m_CubeIndexBuffer);

        m_LightVertexBuffer = Vortex::VertexBufferCreate(cubeVertices, sizeof(cubeVertices));
        m_LightIndexBuffer = Vortex::IndexBufferCreate(cubeIndices, sizeof(cubeIndices));
        m_LightVertexArray = Vortex::VertexArrayCreate();
        m_LightVertexBuffer->SetLayout(m_BufferLayout);
        m_LightVertexArray->AddVertexBuffer(m_LightVertexBuffer);
        m_LightVertexArray->SetIndexBuffer(m_LightIndexBuffer);

        m_ShaderLibrary = std::make_shared<Vortex::ShaderLibrary>();
        m_ShaderLibrary->Load("assets/Shaders/Base.glsl");
        m_ShaderLibrary->Load("assets/Shaders/Light.glsl");
        m_ShaderLibrary->Load("assets/Shaders/Phong.glsl");

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
            auto phongShader = m_ShaderLibrary->Get("Phong");
            auto lightShader = m_ShaderLibrary->Get("Light");
            phongShader->Bind();
            phongShader->SetMatrix4("u_ViewProj", m_Camera->GetViewProj());
            phongShader->SetFloat3("u_ViewPos", m_Camera->GetPosition());
            phongShader->SetInt("u_Material.diffuse", 0);
            phongShader->SetInt("u_Material.specular", 1);
            phongShader->SetFloat("u_Material.shininess", 32.0f);
            // directional light
            phongShader->SetFloat3("u_DirectionalLight.direction", {-0.2f, -1.0f, -0.3f});
            phongShader->SetFloat3("u_DirectionalLight.ambient", {0.05f, 0.05f, 0.05f});
            phongShader->SetFloat3("u_DirectionalLight.diffuse", {0.4f, 0.4f, 0.4f});
            phongShader->SetFloat3("u_DirectionalLight.specular", {0.5f, 0.5f, 0.5f});
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
            // spotlight
            phongShader->SetFloat3("u_SpotLight.position", m_Camera->GetPosition());
            phongShader->SetFloat3("u_SpotLight.direction", m_Camera->GetFront());
            phongShader->SetFloat3("u_SpotLight.ambient", {0.0f, 0.0f, 0.0f});
            phongShader->SetFloat3("u_SpotLight.diffuse", {1.0f, 1.0f, 1.0f});
            phongShader->SetFloat3("u_SpotLight.specular", {1.0f, 1.0f, 1.0f});
            phongShader->SetFloat("u_SpotLight.constant", 1.0f);
            phongShader->SetFloat("u_SpotLight.linear", 0.09f);
            phongShader->SetFloat("u_SpotLight.quadratic", 0.032f);
            phongShader->SetFloat("u_SpotLight.cutOff", glm::cos(glm::radians(12.5f)));
            phongShader->SetFloat("u_SpotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

            m_CubeDiffuseMap->Bind(0);
            m_CubeSpecularMap->Bind(1);

            for (unsigned int i = 0; i < 10; i++) {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, cubePositions[i]);
                float angle = 20.0f * i;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                phongShader->SetMatrix4("u_Model", model);
                Vortex::Renderer::Submit(m_CubeVertexArray);
            }

            lightShader->Bind();
            lightShader->SetMatrix4("u_ViewProj", m_Camera->GetViewProj());
            lightShader->SetFloat3("u_Color", {1.0f, 1.0f, 1.0f});

            for (unsigned int i = 0; i < 4; i++) {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, pointLightPositions[i]);
                model = glm::scale(model, glm::vec3(0.2f));
                lightShader->SetMatrix4("u_Model", model);
                Vortex::Renderer::Submit(m_LightVertexArray);
            }
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
        if (glfwGetKey(m_Window, GLFW_KEY_SPACE) == GLFW_PRESS || glfwGetKey(m_Window, GLFW_KEY_SPACE) == GLFW_REPEAT)
            m_Camera->MoveUp(0.02f);
        if (glfwGetKey(m_Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(m_Window, GLFW_KEY_LEFT_SHIFT) == GLFW_REPEAT)
            m_Camera->MoveUp(-0.02f);
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

    static std::shared_ptr<FPSCamera> m_Camera;
    std::shared_ptr<Vortex::Context> m_Context;
    std::shared_ptr<Vortex::ShaderLibrary> m_ShaderLibrary;

    std::shared_ptr<Vortex::VertexBuffer> m_CubeVertexBuffer;
    std::shared_ptr<Vortex::IndexBuffer> m_CubeIndexBuffer;
    std::shared_ptr<Vortex::VertexArray> m_CubeVertexArray;
    std::shared_ptr<Vortex::Texture2D> m_CubeDiffuseMap;
    std::shared_ptr<Vortex::Texture2D> m_CubeSpecularMap;

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
