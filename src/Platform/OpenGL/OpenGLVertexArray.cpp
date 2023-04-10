#include <Vortex/Buffer.hpp>
#include <Vortex/Platform/OpenGL/OpenGLVertexArray.hpp>
#include <Vortex/Platform/OpenGL/OpenGLRendererAPI.hpp>
#include <spdlog/spdlog.h>
#include <glad/glad.h>

using namespace Vortex::OpenGL;

namespace Vortex::Utils {
static GLenum ShaderDataTypeToOpenGLBaseType(Vortex::ShaderDataType type) {
    switch (type) {
    case Vortex::ShaderDataType::Float:
        return GL_FLOAT;
    case Vortex::ShaderDataType::Float2:
        return GL_FLOAT;
    case Vortex::ShaderDataType::Float3:
        return GL_FLOAT;
    case Vortex::ShaderDataType::Float4:
        return GL_FLOAT;
    case Vortex::ShaderDataType::Mat3:
        return GL_FLOAT;
    case Vortex::ShaderDataType::Mat4:
        return GL_FLOAT;
    case Vortex::ShaderDataType::Int:
        return GL_INT;
    case Vortex::ShaderDataType::Int2:
        return GL_INT;
    case Vortex::ShaderDataType::Int3:
        return GL_INT;
    case Vortex::ShaderDataType::Int4:
        return GL_INT;
    case Vortex::ShaderDataType::Bool:
        return GL_BOOL;
    default:
        assert(false);
    }
    return 0;
}
} // namespace Vortex::Utils

OpenGLVertexArray::OpenGLVertexArray() {
    glGenVertexArrays(1, &m_RendererID);
    glCheckError();
    spdlog::trace("Created OpenGL vertex array (ID: {})", m_RendererID);
}

OpenGLVertexArray::~OpenGLVertexArray() {
    glDeleteVertexArrays(1, &m_RendererID);
    glCheckError();
    spdlog::trace("Deleted OpenGL vertex array (ID: {})", m_RendererID);
}

void OpenGLVertexArray::Bind() const {
    glBindVertexArray(m_RendererID);
    glCheckError();
    spdlog::trace("Bound OpenGL vertex array (ID: {})", m_RendererID);
}

void OpenGLVertexArray::Unbind() const {
    glBindVertexArray(0);
    glCheckError();
    spdlog::trace("Unbount OpenGL vertex array (ID: {})", m_RendererID);
}

void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) {
    assert(vertexBuffer->GetLayout().GetElements().size());

    glBindVertexArray(m_RendererID);
    vertexBuffer->Bind();

    uint32_t index = 0;
    const auto& layout = vertexBuffer->GetLayout();
    for (const auto& element : layout) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, element.GetComponentCount(), Vortex::Utils::ShaderDataTypeToOpenGLBaseType(element.Type), element.Normalized ? GL_TRUE : GL_FALSE,
                              layout.GetStride(), (const void*) element.Offset);
        index++;
    }
    m_VertexBuffers.push_back(vertexBuffer);

    glCheckError();
    spdlog::trace("Attached vertex buffer to OpenGL vertex array (ID: {})", m_RendererID);
}

void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) {
    glBindVertexArray(m_RendererID);
    indexBuffer->Bind();
    m_IndexBuffer = indexBuffer;

    glCheckError();
    spdlog::trace("Set index buffer of OpenGL vertex array (ID: {})", m_RendererID);
}
