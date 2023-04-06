#pragma once

#include <vector>
#include <string>
#include <memory>
#include <glm/glm.hpp>

#include "../Camera.hpp"
#include "../VertexArray.hpp"
#include "Model.hpp"

namespace Vortex {
class PointLight {
public:
    PointLight() : m_Color(1.0f, 1.0f, 1.0f, 1.0f) {
    }
    PointLight(const glm::vec4& color) : m_Color(color) {
    }
    ~PointLight() = default;

    const glm::vec4& GetColor() {
        return m_Color;
    }

private:
    glm::vec4 m_Color;
};

class Mesh {
public:
    Mesh(const std::shared_ptr<VertexArray>& vertexArray) {
        m_VertexArrays.push_back(vertexArray);
    }
    Mesh(const std::string& filepath) {
        Model model(filepath);
        auto meshes = model.GetMeshes();
        for (auto mesh : meshes)
            m_VertexArrays.push_back(mesh.GetVertexArray());
    }
    ~Mesh() = default;

    void Append(const std::shared_ptr<VertexArray>& vertexArray) {
        m_VertexArrays.push_back(vertexArray);
    }

    const std::vector<std::shared_ptr<VertexArray>>& GetVertexArrays() {
        return m_VertexArrays;
    }

private:
    std::vector<std::shared_ptr<VertexArray>> m_VertexArrays;
};

class Object {
public:
    Object() : m_Transform(1.0f) {
    }
    Object(const glm::mat4& transform) : m_Transform(transform) {
    }
    ~Object() = default;

    void Attach(const std::shared_ptr<PointLight>& pointLight) {
        m_PointLights.push_back(pointLight);
    }

    void Attach(const std::shared_ptr<Mesh>& mesh) {
        m_Meshs.push_back(mesh);
    }

    const glm::mat4& GetTransform() {
        return m_Transform;
    }

    const std::vector<std::shared_ptr<PointLight>>& GetPointLights() {
        return m_PointLights;
    }

    const std::vector<std::shared_ptr<Mesh>>& GetMeshs() {
        return m_Meshs;
    }

private:
    glm::mat4 m_Transform;
    std::vector<std::shared_ptr<PointLight>> m_PointLights;
    std::vector<std::shared_ptr<Mesh>> m_Meshs;
};

class Scene {
public:
    Scene(std::shared_ptr<Camera> camera) : m_Camera(camera) {
    }
    ~Scene() = default;

    void Append(const std::shared_ptr<Object>& object) {
        m_Objects.push_back(object);
    }

    const std::vector<std::shared_ptr<Object>>& GetObjects() {
        return m_Objects;
    }

    const std::shared_ptr<Camera>& GetCamera() {
        return m_Camera;
    }

private:
    std::vector<std::shared_ptr<Object>> m_Objects;
    std::shared_ptr<Camera> m_Camera;
};
} // namespace Vortex
