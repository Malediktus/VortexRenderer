#include "../../Framebuffer.hpp"

namespace Vortex::OpenGL {
class OpenGLFramebuffer : public Framebuffer {
public:
    OpenGLFramebuffer();
    virtual ~OpenGLFramebuffer();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void AttachColorBuffer(const std::shared_ptr<Texture2D>& texture) override;
    virtual void AttachDepthBuffer(const std::shared_ptr<Texture2D>& texture) override;
    virtual void AttachStencilBuffer(const std::shared_ptr<Texture2D>& texture) override;
    virtual void AttachDepthStencilBuffer(const std::shared_ptr<Renderbuffer>& renderbuffer) override;

private:
    uint32_t m_RendererID;
};
} // namespace Vortex::OpenGL