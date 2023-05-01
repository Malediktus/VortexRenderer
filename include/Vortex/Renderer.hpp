#pragma once

#include "RenderCommand.hpp"
#include "RendererAPI.hpp"
#include "Core.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Texture.hpp"
#include "Context.hpp"
#include "Scene/Scene.hpp"

#include <glm/glm.hpp>

#include <memory>
#include <string>

namespace Vortex {
/**
 * @brief The Renderer is a high level API for rendering. Init() should be called after context creation. LoadShader() and
 * AddShader() function are used for adding shaders. Submit() renders a provided VertexArray and should be surrounded by
 * BeginScene() and EndScene().
 *
 * @version 0.1
 * @author Nico Grundei (malediktusrgb@gmail.com)
 * @date 2023-03-31
 * @copyright Copyright (c) 2023
 */
class Renderer {
public:
    VT_API Renderer(const std::string& shaderPath, const int width, const int height);
    VT_API ~Renderer() = default;

    VT_API void OnResize(const int width, const int height);

    VT_API void BeginFrame();
    VT_API void EndFrame();

    VT_API void Submit(const std::shared_ptr<VertexArray>& vertexArray);
    VT_API void Submit(const std::shared_ptr<Scene>& scene);

    inline static VT_API RendererAPI::API GetAPI() {
        return RendererAPI::GetAPI();
    }

    static VT_API void SetContext(std::shared_ptr<Context> context);
    static VT_API std::shared_ptr<Context> GetContext();

private:
    std::shared_ptr<Shader> m_Shader;
    static std::shared_ptr<Context> s_Context;
};
} // namespace Vortex
