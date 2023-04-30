#pragma once

#include <Vortex/VertexArray.hpp>

namespace Vortex::Vulkan {
class VulkanVertexArray : public VertexArray {
public:
    VulkanVertexArray();
    virtual ~VulkanVertexArray();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
    virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

    virtual void SetPrimitive(Primitive primitive) override {
        m_Primitive = primitive;
    }

    virtual Primitive GetPrimitive() const override {
        return m_Primitive;
    }

    virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override {
        return m_VertexBuffers;
    }
    virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override {
        return m_IndexBuffer;
    }

private:
    Primitive m_Primitive;
    std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
    std::shared_ptr<IndexBuffer> m_IndexBuffer;
};
} // namespace Vortex::Vulkan
