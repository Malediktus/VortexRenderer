#include <Vortex/Platform/OpenGL/OpenGLRenderbuffer.hpp>
#include <Vortex/Platform/OpenGL/OpenGLRendererAPI.hpp>
#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <tracy/Tracy.hpp>
#include <cassert>

using namespace Vortex::OpenGL;

OpenGLRenderbuffer::OpenGLRenderbuffer(uint32_t width, uint32_t height, Renderbuffer::RenderbufferType type) {
    ZoneScoped;
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
    glCheckError();
    spdlog::trace("Created OpenGL renderbuffer (ID: {})", m_RendererID);
}

OpenGLRenderbuffer::~OpenGLRenderbuffer() {
    ZoneScoped;
    glDeleteRenderbuffers(1, &m_RendererID);
    glCheckError();
    spdlog::trace("Deleted OpenGL renderbuffer (ID: {})", m_RendererID);
}

void OpenGLRenderbuffer::Bind() const {
    ZoneScoped;
    glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);
    glCheckError();
    spdlog::trace("Bound OpenGL renderbuffer (ID: {})", m_RendererID);
}

void OpenGLRenderbuffer::Unbind() const {
    ZoneScoped;
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glCheckError();
    spdlog::trace("Unbound OpenGL renderbuffer (ID: {})", m_RendererID);
}

void* OpenGLRenderbuffer::GetNative() const {
    return (void*) &m_RendererID;
}
