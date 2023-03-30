#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Vortex {
class Camera {
public:
    Camera(const glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
        : m_ProjectionMatrix(projectionMatrix), m_ViewMatrix(viewMatrix) {
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }
    virtual ~Camera() = default;

    virtual const glm::vec3& GetPosition() const {
        return m_Position;
    }
    virtual void SetPosition(const glm::vec3& position) {
        m_Position = position;
        CalculateMatrices();
    }

    virtual const float GetRotation() const {
        return m_Rotation;
    }
    virtual void SetRotation(const float rotation) {
        m_Rotation = rotation;
        CalculateMatrices();
    }

    virtual const glm::mat4& GetProjectionMatrix() const {
        return m_ProjectionMatrix;
    }
    virtual const glm::mat4& GetViewMatrix() const {
        return m_ViewMatrix;
    }
    virtual const glm::mat4& GetViewProjectionMatrix() const {
        return m_ViewProjectionMatrix;
    }

private:
    virtual void CalculateMatrices() {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
                              glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

        m_ViewMatrix = glm::inverse(transform);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }


    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ViewProjectionMatrix;

    glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
    float m_Rotation = 0.0f;
};

class OrthographicCamera : public Camera {
public:
    OrthographicCamera(const float left, const float right, const float bottom, const float top)
        : Camera(glm::ortho(left, right, bottom, top, -1.0f, 1.0f), glm::mat4(1.0f)) {
    }
};

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(const float fov, const float aspectRatio)
        : Camera(glm::perspective(fov / 2.0f, aspectRatio, 0.1f, 1000.0f), glm::mat4(1.0f)) {
    }
};
} // namespace Vortex
