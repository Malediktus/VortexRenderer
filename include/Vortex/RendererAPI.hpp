#pragma once

#include "VertexArray.hpp"
#include "Core.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <cstdint>

namespace Vortex {
class RendererAPI {
public:
    enum class API { OpenGL = 0 };

    virtual VT_API void SetClearColor(const glm::vec4& color) = 0;
    virtual VT_API void Clear() = 0;

    virtual VT_API void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

    inline static VT_API RendererAPI::API GetAPI() {
        return s_API;
    }

private:
    static API s_API;
};

VT_API std::shared_ptr<RendererAPI> RendererAPICreate();
} // namespace Vortex
