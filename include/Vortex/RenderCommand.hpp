#pragma once

#include "RendererAPI.hpp"
#include "Core.hpp"

namespace Vortex {
/**
 * @brief This is an abstraction layer for rendering. The relevant functions are SetClearColor() and Clear(). Init() is handled
 * automaticly by Renderer::Init().
 *
 * @version 0.1
 * @author Nico Grundei (malediktusrgb@gmail.com)
 * @date 2023-03-31
 * @copyright Copyright (c) 2023
 */
class RenderCommand {
public:
    static VT_API void Init() {
        s_RendererAPI = RendererAPICreate();
    }

    inline static VT_API void SetClearColor(const glm::vec4& color) {
        s_RendererAPI->SetClearColor(color);
    }

    inline static VT_API void SetViewport(const int width, const int height) {
        s_RendererAPI->SetViewport(width, height);
    }

    inline static VT_API void Clear() {
        s_RendererAPI->Clear();
    }

    inline static VT_API void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t count = 0) {
        s_RendererAPI->DrawIndexed(vertexArray, count);
    }

private:
    static VT_API std::shared_ptr<RendererAPI> s_RendererAPI;
};
} // namespace Vortex
