#pragma once

#include "RenderCommand.hpp"
#include "RendererAPI.hpp"
#include "Core.hpp"
#include "Shader.hpp"

#include <memory>
#include <string>

namespace Vortex {
class Renderer {
public:
    static VT_API void Init();
    
    static VT_API void LoadShader(const std::string &name, const std::string &filepath);
    static VT_API void LoadShader(const std::string &filepath);
    static VT_API void AddShader(const std::string &name, const std::shared_ptr<Shader>& shader);
    static VT_API void AddShader(const std::shared_ptr<Shader>& shader);
    
    static VT_API void BeginScene();
    static VT_API void EndScene();

    static VT_API void Submit(const std::string &shaderName, const std::shared_ptr<VertexArray>& vertexArray);

    inline static VT_API RendererAPI::API GetAPI() {
        return RendererAPI::GetAPI();
    }
    
private:
    static std::shared_ptr<ShaderLibrary> s_ShaderLibrary;
};
} // namespace Vortex
