#pragma once

#include "RenderCommand.hpp"
#include "RendererAPI.hpp"
#include "Core.hpp"

namespace Vortex {
class Renderer {
public:
    static VT_API void BeginScene();
    static VT_API void EndScene();

    static VT_API void Submit(const std::shared_ptr<VertexArray>& vertexArray);

    inline static VT_API RendererAPI::API GetAPI() {
        return RendererAPI::GetAPI();
    }
};
} // namespace Vortex
