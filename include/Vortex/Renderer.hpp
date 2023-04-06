#pragma once

#include "RenderCommand.hpp"
#include "RendererAPI.hpp"
#include "Core.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Texture.hpp"
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
    static VT_API void Init(const std::string& shaderPath);

    static VT_API void BeginFrame();
    static VT_API void EndFrame();

    static VT_API void Submit(const std::shared_ptr<VertexArray>& vertexArray);
    static VT_API void Submit(const std::shared_ptr<Scene>& scene);

    inline static VT_API RendererAPI::API GetAPI() {
        return RendererAPI::GetAPI();
    }

private:
    static std::shared_ptr<Shader> m_Shader;
};
} // namespace Vortex
