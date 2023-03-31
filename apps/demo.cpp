#include <GLFW/glfw3.h>
#include <Vortex/Vortex.hpp>

#include <cstdlib>
#include <iostream>

static void GlfwErrorCallback(int error, const char* description) {
    std::cout << "GLFW Error (" << error << "): " << description << std::endl;
    exit(1);
}

int main() {
    GLFWwindow* window;
    glfwSetErrorCallback(GlfwErrorCallback);
    if (!glfwInit()) {
        std::cout << "Failed to init GLFW!" << std::endl;
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1024, 720, "Vortex Renderer Demo", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create window!" << std::endl;
        return 1;
    }

    auto context = Vortex::ContextCreate(window);
    context->Init();

    Vortex::Renderer::Init();
    Vortex::RenderCommand::SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});

    float vertices[] = {0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,
                        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f};
    unsigned int indices[] = {0, 1, 3, 1, 2, 3};

    std::shared_ptr<Vortex::VertexArray> vertexArray = Vortex::VertexArrayCreate();
    Vortex::BufferLayout bufferLayout(
        {{Vortex::ShaderDataType::Float3, "Position", false}, {Vortex::ShaderDataType::Float2, "TexCoords", false}});
    std::shared_ptr<Vortex::VertexBuffer> vertexBuffer = Vortex::VertexBufferCreate(vertices, sizeof(vertices));
    vertexBuffer->SetLayout(bufferLayout);
    vertexArray->AddVertexBuffer(vertexBuffer);
    std::shared_ptr<Vortex::IndexBuffer> indexBuffer = Vortex::IndexBufferCreate(indices, sizeof(indices));
    vertexArray->SetIndexBuffer(indexBuffer);
    Vortex::Renderer::LoadShader("Base", "assets/Shaders/Base.glsl");

    std::shared_ptr<Vortex::PerspectiveCamera> camera = std::make_shared<Vortex::PerspectiveCamera>(90.0f, 1080.0f / 720.0f);
    std::shared_ptr<Vortex::Texture2D> texture = Vortex::Texture2DCreate("assets/Textures/Vulkano.png");
    camera->SetPosition({0.0f, 0.0f, 1.5f});

    glm::mat4 transform(1.0f);

    while (!glfwWindowShouldClose(window)) {
        Vortex::RenderCommand::Clear();

        Vortex::Renderer::BeginScene(camera, "Base");
        Vortex::Renderer::Submit(vertexArray, texture, transform);
        Vortex::Renderer::EndScene();

        context->SwapBuffers();
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
