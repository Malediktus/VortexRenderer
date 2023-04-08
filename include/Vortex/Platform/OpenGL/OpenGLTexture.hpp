#pragma once

#include "../../Texture.hpp"

namespace Vortex::OpenGL {
class OpenGLTexture2D : public Texture2D {
public:
    OpenGLTexture2D(const std::string& path);
    OpenGLTexture2D(const int width, const int height);
    OpenGLTexture2D(const int width, const int height, const void* data);
    virtual ~OpenGLTexture2D();

    virtual uint32_t GetWidth() const override {
        return m_Width;
    }
    virtual uint32_t GetHeight() const override {
        return m_Height;
    }

    virtual void Bind(uint32_t slot = 0) const override;
    virtual void Resize(uint32_t width, uint32_t height) override;
    virtual void* GetNative() const override;

private:
    std::string m_Path;
    uint32_t m_Width, m_Height;
    uint32_t m_RendererID;
};
} // namespace Vortex::OpenGL
