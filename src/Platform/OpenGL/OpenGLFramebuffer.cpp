#include <Vortex/Platform/OpenGL/OpenGLFramebuffer.hpp>
#include <glad/glad.h>
#include <cassert>

using namespace Vortex::OpenGL;

OpenGLFramebuffer::OpenGLFramebuffer() {
    glGenFramebuffers(1, &m_RendererID);
}

OpenGLFramebuffer::~OpenGLFramebuffer() {
    glDeleteFramebuffers(1, &m_RendererID);
}

void OpenGLFramebuffer::Bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
}

void OpenGLFramebuffer::Unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::AttachColorBuffer(const std::shared_ptr<Texture2D>& texture) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *(GLuint*) texture->GetNative(), 0);
}

void OpenGLFramebuffer::AttachDepthBuffer(const std::shared_ptr<Texture2D>& texture) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, *(GLuint*) texture->GetNative(), 0);
}

void OpenGLFramebuffer::AttachStencilBuffer(const std::shared_ptr<Texture2D>& texture) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, *(GLuint*) texture->GetNative(), 0);
}

void OpenGLFramebuffer::AttachDepthStencilBuffer(const std::shared_ptr<Renderbuffer>& renderbuffer) {
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, *(GLuint*) renderbuffer->GetNative());
}
