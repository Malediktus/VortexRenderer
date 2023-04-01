#pragma once

#include "../../RendererAPI.hpp"

namespace Vortex::OpenGL {
class OpenGLRendererAPI : public RendererAPI {
public:
    OpenGLRendererAPI() = default;
    virtual ~OpenGLRendererAPI() = default;

    virtual void SetClearColor(const glm::vec4& color) override;
    virtual void Clear() override;

    virtual void SetViewport(const int width, const int height) override;

    virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount) override;
};
} // namespace Vortex::OpenGL
