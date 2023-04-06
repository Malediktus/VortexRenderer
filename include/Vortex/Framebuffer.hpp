#pragma once

#include "Core.hpp"
#include "Texture.hpp"
#include "Renderbuffer.hpp"
#include <memory>

namespace Vortex {
class Framebuffer {
public:
    virtual ~Framebuffer() = default;

    virtual VT_API void Bind() const = 0;
    virtual VT_API void Unbind() const = 0;

    virtual VT_API void AttachColorBuffer(const std::shared_ptr<Texture2D>& texture) = 0;
    virtual VT_API void AttachDepthBuffer(const std::shared_ptr<Texture2D>& texture) = 0;
    virtual VT_API void AttachStencilBuffer(const std::shared_ptr<Texture2D>& texture) = 0;
    virtual VT_API void AttachDepthStencilBuffer(const std::shared_ptr<Renderbuffer>& renderbuffer) = 0;
};

VT_API std::shared_ptr<Framebuffer> FramebufferCreate();
} // namespace Vortex
