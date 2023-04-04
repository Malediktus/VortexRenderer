#include "../Core.hpp"
#include "../Buffer.hpp"
#include "../VertexArray.hpp"
#include "../Texture.hpp"
#include "../Shader.hpp"

#include <glm/glm.hpp>
#include <string>

namespace Vortex::Scene {
struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    std::string Type;
    std::string Filepath;
    std::shared_ptr<Texture2D> Texture;
};

class Mesh {
public:
    VT_API Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, std::vector<Scene::Texture>& textures);
    VT_API void BindTextures(const std::shared_ptr<Shader>& shader) const;

    VT_API const std::shared_ptr<VertexArray>& GetVertexArray() {
        return m_VertexArray;
    }

private:
    const std::vector<uint32_t> m_Indices;
    const std::vector<Vertex> m_Vertices;
    const std::vector<Texture> m_Textures;
    std::shared_ptr<VertexBuffer> m_VertexBuffer;
    std::shared_ptr<IndexBuffer> m_IndexBuffer;
    std::shared_ptr<VertexArray> m_VertexArray;
};

} // namespace Vortex::Scene
