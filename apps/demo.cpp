#include <GLFW/glfw3.h>
#include <Vortex/Vortex.hpp>

#include <cstdlib>
#include <iostream>

using namespace Vortex; // Not good practice

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

float vertices[] = {0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  // 1
                    0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,  // 2
                    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // 3
                    -0.5f, 0.5f,  0.0f, 0.0f, 1.0f}; // 4

unsigned int indices[] = {0, 1, 3,  // 2
                          1, 2, 3}; // 2

glm::mat4 transform(1.0f);

int main() {
    GLFWwindow* window = nullptr;
    window = CreateWindow(window);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    auto vortexContext = ContextCreate(window);
    vortexContext->Init();

    BufferLayout bufferLayout({{ShaderDataType::Float3, "Position", false},    // Vertex position
                               {ShaderDataType::Float2, "TexCoords", false}}); // Vertex texture coords

    auto vertexBuffer = VertexBufferCreate(vertices, sizeof(vertices));
    auto indexBuffer = IndexBufferCreate(indices, sizeof(indices));
    auto vertexArray = VertexArrayCreate();
    auto texture = Texture2DCreate("assets/Textures/Vulkano.png");
    auto camera = std::make_shared<PerspectiveCamera>(90.0f, (float) width / (float) height);

    camera->SetPosition({0.0f, 0.0f, 1.5f});
    vertexBuffer->SetLayout(bufferLayout);
    vertexArray->AddVertexBuffer(vertexBuffer);
    vertexArray->SetIndexBuffer(indexBuffer);

    Renderer::Init();
    Renderer::LoadShader("Base", "../../apps/assets/Shaders/Base.glsl");
    RenderCommand::SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});

    while (!glfwWindowShouldClose(window)) {
        glfwGetFramebufferSize(window, &width, &height);
        auto camera = std::make_shared<PerspectiveCamera>(90.0f, (float) width / (float) height);
        camera->SetPosition({0.0f, 0.0f, 1.5f});

        Renderer::BeginScene(camera, "Base");
        Renderer::Submit(vertexArray, texture, transform);
        Renderer::EndScene();

        vortexContext->SwapBuffers();
        RenderCommand::Clear();
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
