#pragma once

#include "RenderCommand.hpp"
#include "RendererAPI.hpp"
#include "Core.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Texture.hpp"

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
    static VT_API void Init();

    static VT_API void LoadShader(const std::string& name, const std::string& filepath);
    static VT_API void LoadShader(const std::string& filepath);
    static VT_API void AddShader(const std::string& name, const std::shared_ptr<Shader>& shader);
    static VT_API void AddShader(const std::shared_ptr<Shader>& shader);

    static VT_API void BeginScene(const std::shared_ptr<Camera>& camera, const std::string& shaderName);
    static VT_API void EndScene();

    static VT_API void Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Texture2D>& texture,
                              glm::mat4& transform);

    inline static VT_API RendererAPI::API GetAPI() {
        return RendererAPI::GetAPI();
    }

private:
    struct SceneData {
        const std::shared_ptr<Camera> SceneCamera;
        const std::string SceneShader;
        SceneData(const std::shared_ptr<Camera>& camera, const std::string& shader) : SceneCamera(camera), SceneShader(shader) {
        }
    };

    static std::shared_ptr<ShaderLibrary> s_ShaderLibrary;
    static std::shared_ptr<SceneData> s_SceneData;
};
} // namespace Vortex
