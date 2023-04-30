#pragma once

#include "../../RendererAPI.hpp"

namespace Vortex::Vulkan {
class VulkanRendererAPI : public RendererAPI {
public:
    VulkanRendererAPI() = default;
    virtual ~VulkanRendererAPI() = default;

    virtual void SetClearColor(const glm::vec4& color) override;
    virtual void Clear(const ClearBuffer clearBuffer) override;

    virtual void ConfigureDepthTesting(const bool enable, const bool depthMask, const DepthTestFunc func) override;
    virtual void ConfigureStencilTesting(const bool enable, const int writeMask, const int readMask, const StencilTestFunc func, const int ref,
                                         const StencilTestAction stencilFailAction, const StencilTestAction stencilPassDepthFailAction,
                                         const StencilTestAction stencilPassDepthPassAction) override;
    virtual void ConfigureBlending(const bool enable, const BlendingFunc blendingFunc1, const BlendingFunc blendingFunc2, const BlendingFunc blendingFuncR,
                                   const BlendingFunc blendingFuncG, const BlendingFunc blendingFuncB, const BlendingFunc blendingFuncA) override;
    virtual void ConfigureCulling(const bool enable, const CullingType type) override;
    virtual void ConfigureWireframeView(const bool enable) override;

    virtual void SetViewport(const int width, const int height) override;

    virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount) override;
};
} // namespace Vortex::Vulkan
