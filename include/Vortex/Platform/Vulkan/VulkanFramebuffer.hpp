#include "../../Framebuffer.hpp"

namespace Vortex::Vulkan {
class VulkanFramebuffer : public Framebuffer {
public:
    VulkanFramebuffer();
    virtual ~VulkanFramebuffer();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void AttachColorBuffer(const std::shared_ptr<Texture2D>& texture) override;
    virtual void AttachDepthBuffer(const std::shared_ptr<Texture2D>& texture) override;
    virtual void AttachStencilBuffer(const std::shared_ptr<Texture2D>& texture) override;
    virtual void AttachDepthStencilBuffer(const std::shared_ptr<Renderbuffer>& renderbuffer) override;
};
} // namespace Vortex::Vulkan