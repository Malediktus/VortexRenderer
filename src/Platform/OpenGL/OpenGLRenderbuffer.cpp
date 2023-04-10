#include <Vortex/Platform/OpenGL/OpenGLRenderbuffer.hpp>
#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <cassert>

using namespace Vortex::OpenGL;

OpenGLRenderbuffer::OpenGLRenderbuffer(uint32_t width, uint32_t height, Renderbuffer::RenderbufferType type) {
    glGenRenderbuffers(1, &m_RendererID);
    glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);
    GLuint format;
    switch (type) {
    case Renderbuffer::RenderbufferType::DEPTH24_STENCIL8: {
        format = GL_DEPTH24_STENCIL8;
    } break;
    }
    glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    spdlog::trace("Created OpenGL renderbuffer (ID: {})", m_RendererID);
}

OpenGLRenderbuffer::~OpenGLRenderbuffer() {
    glDeleteRenderbuffers(1, &m_RendererID);
    spdlog::trace("Deleted OpenGL renderbuffer (ID: {})", m_RendererID);
}

void OpenGLRenderbuffer::Bind() const {
    glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);
    spdlog::trace("Bound OpenGL renderbuffer (ID: {})", m_RendererID);
}

void OpenGLRenderbuffer::Unbind() const {
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    spdlog::trace("Unbound OpenGL renderbuffer (ID: {})", m_RendererID);
}

void* OpenGLRenderbuffer::GetNative() const {
    return (void*) &m_RendererID;
}
