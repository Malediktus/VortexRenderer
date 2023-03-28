#pragma once

#include "Buffer.hpp"
#include "Core.hpp"

#include <memory>

namespace Vortex {
class VertexArray {
public:
    virtual ~VertexArray() {
    }

    virtual VT_API void Bind() const = 0;
    virtual VT_API void Unbind() const = 0;

    virtual VT_API void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
    virtual VT_API void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

    virtual VT_API const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
    virtual VT_API const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;
};

VT_API std::shared_ptr<VertexArray> VertexArrayCreate();
} // namespace Vortex
