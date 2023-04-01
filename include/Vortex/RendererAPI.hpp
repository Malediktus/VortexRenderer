#pragma once

#include "VertexArray.hpp"
#include "Core.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <cstdint>

namespace Vortex {
/**
 * @brief This is a rendering abstraction and should not be used by the client because its realy low level.
 *
 * @version 0.1
 * @author Nico Grundei (malediktusrgb@gmail.com)
 * @date 2023-03-31
 * @copyright Copyright (c) 2023
 */
class RendererAPI {
public:
    enum class API { OpenGL = 0 };

    virtual VT_API void SetClearColor(const glm::vec4& color) = 0;
    virtual VT_API void Clear() = 0;

    virtual VT_API void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;

    inline static VT_API RendererAPI::API GetAPI() {
        return s_API;
    }

private:
    static API s_API;
};

/**
 * @brief This function is the only way that should be used for creating a RendererAPI.
 *
 * @return std::shared_ptr<RendererAPI>
 * @version 0.1
 * @author Nico Grundei (malediktusrgb@gmail.com)
 * @date 2023-03-31
 * @copyright Copyright (c) 2023
 */
VT_API std::shared_ptr<RendererAPI> RendererAPICreate();
} // namespace Vortex
