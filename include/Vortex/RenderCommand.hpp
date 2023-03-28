#pragma once

#include "RendererAPI.hpp"
#include "Core.hpp"

namespace Vortex {
class RenderCommand {
public:
    static VT_API void Init() {
        s_RendererAPI = RendererAPICreate();
    }

    inline static VT_API void SetClearColor(const glm::vec4& color) {
        s_RendererAPI->SetClearColor(color);
    }

    inline static VT_API void Clear() {
        s_RendererAPI->Clear();
    }

    inline static VT_API void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) {
        s_RendererAPI->DrawIndexed(vertexArray);
    }

private:
    static VT_API std::shared_ptr<RendererAPI> s_RendererAPI;
};
} // namespace Vortex
