#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Vortex {
class Camera {
public:
    Camera(float fov, float width, float height) {
        projection = glm::perspective(fov / 2.0f, width / height, 0.1f, 1000.0f);
        view = glm::mat4(1.0f);
        position = glm::vec3(0.0f);
        UpdateMatrices();
    }

    virtual ~Camera() = default;

    glm::mat4 GetViewProj() {
        return viewProj;
    }

    virtual void Translate(glm::vec3 v) {
        position += v;
        view = glm::translate(view, v * -1.0f);
        UpdateMatrices();
    }

protected:
    virtual void UpdateMatrices() {
        viewProj = projection * view;
    }

    glm::vec3 position;
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 viewProj;
};
} // namespace Vortex
