#include <GLFW/glfw3.h>
#include <Vortex/Vortex.hpp>

#include <glm/ext/matrix_transform.hpp>

#include <cstdlib>
#include <iostream>

using namespace Vortex; // Not good practice

class FPSCamera : public Camera {
public:
    FPSCamera(float fov, float width, float height) : Camera(fov, width, height) {
        up = glm::vec3(0.0f, 1.0f, 0.0f);
        yaw = -90.0f;
        pitch = 0.0f;
        OnMouseMoved(0.0f, 0.0f);
        UpdateMatrices();
    }

    void OnMouseMoved(float xRel, float yRel) {
        yaw += xRel * mouseSensitivity;
        pitch -= yRel * mouseSensitivity;
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
        front.y = sin(glm::radians(pitch));
        front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
        lookAt = glm::normalize(front);
        UpdateMatrices();
    }

    void MoveFront(float amount) {
        Translate(glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f) * lookAt) * amount);
        UpdateMatrices();
    }

    void MoveSideways(float amount) {
        Translate(glm::normalize(glm::cross(lookAt, up)) * amount);
        UpdateMatrices();
    }

protected:
    void UpdateMatrices() override {
        view = glm::lookAt(position, position + lookAt, up);
        viewProj = projection * view;
    }

    float yaw;
    float pitch;
    glm::vec3 lookAt;
    const float mouseSensitivity = 0.3f;
    glm::vec3 up;
};

static void GlfwErrorCallback(int error, const char* description) {
    std::cout << "GLFW Error (" << error << "): " << description << std::endl;
    exit(1);
}

GLFWwindow* CreateWindow(GLFWwindow* window) {
    glfwSetErrorCallback(GlfwErrorCallback);
    if (!glfwInit()) {
        std::cout << "Failed to init GLFW!" << std::endl;
        exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1024, 720, "Vortex Renderer Demo", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create window!" << std::endl;
        exit(1);
    }
    return window;
}

float lightVertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 1
    0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 2
    0.5f,  0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 3
    0.5f,  0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 4
    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 5
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 6

    -0.5f, -0.5f, 0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 7
    0.5f,  -0.5f, 0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 8
    0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 9
    0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 10
    -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 11
    -0.5f, -0.5f, 0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 12

    -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 13
    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 14
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 15
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 16
    -0.5f, -0.5f, 0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 17
    -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 18

    0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 19
    0.5f,  0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 20
    0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 21
    0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 22
    0.5f,  -0.5f, 0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 23
    0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 24

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 25
    0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 26
    0.5f,  -0.5f, 0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 27
    0.5f,  -0.5f, 0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 28
    -0.5f, -0.5f, 0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 29
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 30

    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 31
    0.5f,  0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 32
    0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 33
    0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 34
    -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 35
    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f  // 36
};

float cubeVertices[] = {
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 1
    0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 2
    0.5f,  0.5f,  -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 3
    0.5f,  0.5f,  -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 4
    -0.5f, 0.5f,  -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 5
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 6

    -0.5f, -0.5f, 0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 7
    0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 8
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 9
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 10
    -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 11
    -0.5f, -0.5f, 0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 12

    -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 13
    -0.5f, 0.5f,  -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 14
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 15
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 16
    -0.5f, -0.5f, 0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 17
    -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 18

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 19
    0.5f,  0.5f,  -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 20
    0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 21
    0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 22
    0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 23
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 24

    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 25
    0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 26
    0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 27
    0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 28
    -0.5f, -0.5f, 0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 29
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 30

    -0.5f, 0.5f,  -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 31
    0.5f,  0.5f,  -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 32
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 33
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 34
    -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 35
    -0.5f, 0.5f,  -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f  // 36
};

unsigned int cubeIndices[] = {0,  1,  2,   //
                              3,  4,  5,   //
                              6,  7,  8,   //
                              9,  10, 11,  //
                              12, 13, 14,  //
                              15, 16, 17,  //
                              18, 19, 20,  //
                              21, 22, 23,  //
                              24, 25, 26,  //
                              27, 28, 29,  //
                              30, 31, 32,  //
                              33, 34, 35}; //

glm::mat4 lightTransform(1.0f);
std::shared_ptr<FPSCamera> camera;

static void OnResize(GLFWwindow*, int width, int height) {
    // auto position = camera->GetPosition();
    // auto rotation = camera->GetRotation();
    // camera = std::make_shared<FPSCamera>(90.0f, (float) width / (float) height);
    // camera->SetPosition(position);
    // camera->SetRotation(rotation);
    RenderCommand::SetViewport(width, height);
}

double lastX = 0, lastY = 0;
double deltaX = 0, deltaY = 0;

static void OnMouseMove(GLFWwindow*, double xpos, double ypos) {
    deltaX = xpos - lastX;
    deltaY = ypos - lastY;

    camera->OnMouseMoved(deltaX, deltaY);

    lastX = xpos;
    lastY = ypos;
}

int main() {
    GLFWwindow* window = nullptr;
    window = CreateWindow(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, OnResize);
    glfwSetCursorPosCallback(window, OnMouseMove);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    auto vortexContext = ContextCreate(window);
    vortexContext->Init();

    BufferLayout bufferLayout({{ShaderDataType::Float3, "Position", false},    // Vertex position
                               {ShaderDataType::Float4, "Color", false},       // Vertex color
                               {ShaderDataType::Float2, "TexCoords", false}}); // Vertex texture coords

    auto cubeVertexBuffer = VertexBufferCreate(cubeVertices, sizeof(cubeVertices));
    auto cubeIndexBuffer = IndexBufferCreate(cubeIndices, sizeof(cubeIndices));
    auto cubeVertexArray = VertexArrayCreate();
    cubeVertexBuffer->SetLayout(bufferLayout);
    cubeVertexArray->AddVertexBuffer(cubeVertexBuffer);
    cubeVertexArray->SetIndexBuffer(cubeIndexBuffer);

    auto lightVertexBuffer = VertexBufferCreate(lightVertices, sizeof(lightVertices));
    auto lightIndexBuffer = IndexBufferCreate(cubeIndices, sizeof(cubeIndices));
    auto lightVertexArray = VertexArrayCreate();
    lightVertexBuffer->SetLayout(bufferLayout);
    lightVertexArray->AddVertexBuffer(lightVertexBuffer);
    lightVertexArray->SetIndexBuffer(lightIndexBuffer);
    lightTransform = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 0.0f, 0.0f));

    camera = std::make_shared<FPSCamera>(90.0f, (float) width, (float) height);
    camera->MoveFront(-5.0f);

    Renderer::Init();
    Renderer::LoadShader("Base", "../../apps/assets/Shaders/Base.glsl");
    RenderCommand::SetViewport(width, height);
    RenderCommand::SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});

    while (!glfwWindowShouldClose(window)) {
        Renderer::BeginScene(camera, "Base");
        Renderer::Submit(cubeVertexArray);
        Renderer::Submit(lightVertexArray, lightTransform);
        Renderer::EndScene();

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_REPEAT)
            camera->MoveFront(0.02f);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_REPEAT)
            camera->MoveFront(-0.02f);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_REPEAT)
            camera->MoveSideways(-0.02f);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_REPEAT)
            camera->MoveSideways(0.02f);

        vortexContext->SwapBuffers();
        RenderCommand::Clear();
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
