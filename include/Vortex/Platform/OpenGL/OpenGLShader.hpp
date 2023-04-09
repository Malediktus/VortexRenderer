#pragma once

#include "../../Shader.hpp"

#include <glm/glm.hpp>

typedef unsigned int GLenum;

namespace Vortex::OpenGL {
class OpenGLShader : public Shader {
public:
    OpenGLShader(const std::string& filepath);
    OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc, const std::string& geometrySrc);
    virtual ~OpenGLShader();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual const std::string& GetName() const override {
        return m_Name;
    }

    virtual void SetInt(const std::string& name, int value) override;

    virtual void SetFloat(const std::string& name, float value) override;
    virtual void SetFloat2(const std::string& name, const glm::vec2& value) override;
    virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
    virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;

    virtual void SetMatrix3(const std::string& name, const glm::mat3& matrix) override;
    virtual void SetMatrix4(const std::string& name, const glm::mat4& matrix) override;

private:
    std::string ReadFile(const std::string& filepath);
    std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
    void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);

    uint32_t m_RendererID;
    std::string m_Name;
};
} // namespace Vortex::OpenGL
