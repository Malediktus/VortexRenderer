#include <Vortex/Platform/OpenGL/OpenGLFramebuffer.hpp>
#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <cassert>

using namespace Vortex::OpenGL;

OpenGLFramebuffer::OpenGLFramebuffer() {
    glGenFramebuffers(1, &m_RendererID);
    spdlog::trace("Created OpenGL framebuffer (ID: {})", m_RendererID);
}

OpenGLFramebuffer::~OpenGLFramebuffer() {
    glDeleteFramebuffers(1, &m_RendererID);
    spdlog::trace("Deleted OpenGL framebuffer (ID: {})", m_RendererID);
}

void OpenGLFramebuffer::Bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    spdlog::info("Bound OpenGL framebuffer (ID: {})", m_RendererID);
}

void OpenGLFramebuffer::Unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    spdlog::info("Unbound OpenGL framebuffer (ID: {})", m_RendererID);
}

void OpenGLFramebuffer::AttachColorBuffer(const std::shared_ptr<Texture2D>& texture) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *(GLuint*) texture->GetNative(), 0);
    spdlog::trace("Attached texture of type color buffer to OpenGL framebuffer (ID: {}, TextureID: {})", m_RendererID, *(GLuint*) texture->GetNative());
}

void OpenGLFramebuffer::AttachDepthBuffer(const std::shared_ptr<Texture2D>& texture) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, *(GLuint*) texture->GetNative(), 0);
    spdlog::trace("Attached texture of type depth buffer to OpenGL framebuffer (ID: {}, TextureID: {})", m_RendererID, *(GLuint*) texture->GetNative());
}

void OpenGLFramebuffer::AttachStencilBuffer(const std::shared_ptr<Texture2D>& texture) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, *(GLuint*) texture->GetNative(), 0);
    spdlog::trace("Attached texture of type stencil buffer to OpenGL framebuffer (ID: {}, TextureID: {})", m_RendererID, *(GLuint*) texture->GetNative());
}

void OpenGLFramebuffer::AttachDepthStencilBuffer(const std::shared_ptr<Renderbuffer>& renderbuffer) {
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, *(GLuint*) renderbuffer->GetNative());
    spdlog::trace("Attached renderbuffer of type depth-stencil buffer to OpenGL framebuffer (ID: {}, RenderbufferID: {})", m_RendererID, *(GLuint*) renderbuffer->GetNative());
}
