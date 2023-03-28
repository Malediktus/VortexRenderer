#pragma once

#include "Core.hpp"

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <unordered_map>

namespace Vortex {
class Shader {
public:
    virtual VT_API ~Shader() = default;

    virtual VT_API void Bind() const = 0;
    virtual VT_API void Unbind() const = 0;

    virtual VT_API const std::string& GetName() const = 0;
};

VT_API std::shared_ptr<Shader> ShaderCreate(const std::string& filepath);
VT_API std::shared_ptr<Shader> ShaderCreate(const std::string& name, const std::string& vertexSrc,
                                            const std::string& fragmentSrc);

class ShaderLibrary {
public:
    VT_API void Add(const std::string& name, const std::shared_ptr<Shader>& shader);
    VT_API void Add(const std::shared_ptr<Shader>& shader);
    VT_API std::shared_ptr<Shader> Load(const std::string& filepath);
    VT_API std::shared_ptr<Shader> Load(const std::string& name, const std::string& filepath);

    VT_API std::shared_ptr<Shader> Get(const std::string& name);

    VT_API bool Exists(const std::string& name) const;

private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
};
} // namespace Vortex