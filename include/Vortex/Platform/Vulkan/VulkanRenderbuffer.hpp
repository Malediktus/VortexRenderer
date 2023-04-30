#include "../../Renderbuffer.hpp"

namespace Vortex::Vulkan {
class VulkanRenderbuffer : public Renderbuffer {
public:
    VulkanRenderbuffer(uint32_t width, uint32_t height, Renderbuffer::RenderbufferType type);
    virtual ~VulkanRenderbuffer();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void* GetNative() const override;
};
} // namespace Vortex::Vulkan
