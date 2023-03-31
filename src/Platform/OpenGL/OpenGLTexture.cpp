#include <Vortex/Platform/OpenGL/OpenGLTexture.hpp>
#include <stb_image/stb_image.h>
#include <glad/glad.h>

using namespace Vortex::OpenGL;

OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : m_Path(path) {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    auto* pixels = stbi_load(path.c_str(), &width, &height, &channels, 0);
    assert(pixels);
    m_Width = width;
    m_Height = height;
    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    stbi_image_free(pixels);
}

OpenGLTexture2D::~OpenGLTexture2D() {
    glDeleteTextures(1, &m_RendererID);
}

void OpenGLTexture2D::Bind(uint32_t slot) const {
    assert(slot <= 31);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
}
