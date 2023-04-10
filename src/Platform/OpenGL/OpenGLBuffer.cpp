#include <Vortex/Platform/OpenGL/OpenGLBuffer.hpp>
#include <spdlog/spdlog.h>
#include <glad/glad.h>

using namespace Vortex::OpenGL;

OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) {
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    spdlog::trace("Created OpenGL vertex buffer of type dynamic (ID: {})", m_RendererID);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) {
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    spdlog::trace("Created OpenGL vertex buffer of type static (ID: {})", m_RendererID);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
    glDeleteBuffers(1, &m_RendererID);
    spdlog::trace("Deleted OpenGL vertex buffer (ID: {})", m_RendererID);
}

void OpenGLVertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    spdlog::trace("Bound OpenGL vertex buffer (ID: {})", m_RendererID);
}

void OpenGLVertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    spdlog::trace("Unbound OpenGL vertex buffer (ID: {})", m_RendererID);
}

void OpenGLVertexBuffer::SetData(const void* data, uint32_t size) {
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    spdlog::trace("Set data for OpenGL vertex buffer (ID: {})", m_RendererID);
}

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) {
    m_Count = count / sizeof(uint32_t);
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    spdlog::trace("Created OpenGL index buffer of type static (ID: {})", m_RendererID);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() {
    glDeleteBuffers(1, &m_RendererID);
    spdlog::trace("Deleted OpenGL index buffer (ID: {})", m_RendererID);
}

void OpenGLIndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    spdlog::trace("Bound OpenGL index buffer (ID: {})", m_RendererID);
}

void OpenGLIndexBuffer::Unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    spdlog::trace("Unbound OpenGL index buffer (ID: {})", m_RendererID);
}
