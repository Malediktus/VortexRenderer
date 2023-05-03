#include <Vortex/Platform/Vulkan/VulkanRendererAPI.hpp>
#include <spdlog/spdlog.h>
#include <tracy/Tracy.hpp>

using namespace Vortex::Vulkan;

void VulkanRendererAPI::SetClearColor(const glm::vec4& color) {
    ZoneScoped;
    (void) color;
    // spdlog::trace("Set Vulkan clear color to ({}, {}, {}, {})", color.r, color.g, color.b, color.a);
}

void VulkanRendererAPI::Clear(const ClearBuffer clearBuffer) {
    ZoneScoped;
    (void) clearBuffer;
}

void VulkanRendererAPI::SetViewport(const int width, const int height) {
    ZoneScoped;
    (void) width;
    (void) height;
    // spdlog::trace("Set Vulkan viewport size to ({}, {})", width, height);
}

void VulkanRendererAPI::ConfigureDepthTesting(const bool enable, const bool depthMask, const DepthTestFunc func) {
    ZoneScoped;
    (void) enable;
    (void) depthMask;
    (void) func;
    // spdlog::trace("Configured Vulkan depth testing: (enable: {}, mask: {}, func: {})", enable, depthMask, (int) func);
}

void VulkanRendererAPI::ConfigureStencilTesting(const bool enable, const int writeMask, const int readMask, const StencilTestFunc func, const int ref,
                                                const StencilTestAction stencilFailAction, const StencilTestAction stencilPassDepthFailAction,
                                                const StencilTestAction stencilPassDepthPassAction) {
    ZoneScoped;
    // spdlog::trace(
    //"Configured Vulkan stencil testing: (enable: {}, writeMask: {}, readMask: {}, ref: {}, stencilFailAction: {}, stencilPassDepthFailAction: {}, stencilPassDepthPassAction:
    //{})", enable, writeMask, readMask, ref, (int) stencilFailAction, (int) stencilPassDepthFailAction, (int) stencilPassDepthPassAction);
}

void VulkanRendererAPI::ConfigureBlending(const bool enable, const BlendingFunc blendingFunc1, const BlendingFunc blendingFunc2, const BlendingFunc blendingFuncR,
                                          const BlendingFunc blendingFuncG, const BlendingFunc blendingFuncB, const BlendingFunc blendingFuncA) {
    ZoneScoped;
    (void) enable;
    (void) blendingFunc1;
    (void) blendingFunc2;
    (void) blendingFuncR;
    (void) blendingFuncG;
    (void) blendingFuncB;
    (void) blendingFuncA;
    // spdlog::trace(
    //"Configured Vulkan blending testing: (enable: {}, blendingFunc1: {}, blendingFunc2: {}, blendingFuncR: {}, blendingFuncG: {}, blendingFuncB: {}, blendingFuncA: {})",
    // enable, (int) blendingFunc1, (int) blendingFunc2, (int) blendingFuncR, (int) blendingFuncG, (int) blendingFuncB, (int) blendingFuncA);
}

void VulkanRendererAPI::ConfigureCulling(const bool enable, const CullingType type) {
    ZoneScoped;
    (void) enable;
    (void) type;
    // spdlog::trace("Configured Vulkan culling: (enable: {}, type: {})", enable, (int) type);
}

void VulkanRendererAPI::ConfigureWireframeView(const bool enable) {
    (void) enable;
}

void VulkanRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount) {
    ZoneScoped;
    (void) vertexArray;
    (void) indexCount;
    // spdlog::trace("Drew Vulkan indexed vertex array (elements: {})", count);
}
