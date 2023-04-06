#pragma once

#include "Core.hpp"
#include "Texture.hpp"
#include <memory>

namespace Vortex {
class Renderbuffer {
public:
    enum RenderbufferType { DEPTH24_STENCIL8 = 0 };

    virtual ~Renderbuffer() = default;

    virtual VT_API void Bind() const = 0;
    virtual VT_API void Unbind() const = 0;

    virtual void* GetNative() const = 0;
};

VT_API std::shared_ptr<Renderbuffer> RenderbufferCreate(uint32_t width, uint32_t height, Renderbuffer::RenderbufferType type);
} // namespace Vortex
