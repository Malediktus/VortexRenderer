#include "../Core.hpp"
#include "../Buffer.hpp"
#include "../VertexArray.hpp"
#include "../Texture.hpp"
#include "../Shader.hpp"

#include <glm/glm.hpp>
#include <string>

namespace Vortex {
struct MeshVertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct MeshTexture {
    std::string Type;
    std::string Filepath;
    std::shared_ptr<Texture2D> Texture;
};

class ModelMesh {
public:
    VT_API ModelMesh(const std::vector<MeshVertex>& vertices, const std::vector<uint32_t>& indices, std::vector<MeshTexture>& textures);
    VT_API void BindTextures(const std::shared_ptr<Shader>& shader) const;

    VT_API const std::shared_ptr<VertexArray>& GetVertexArray() {
        return m_VertexArray;
    }

private:
    const std::vector<uint32_t> m_Indices;
    const std::vector<MeshVertex> m_Vertices;
    const std::vector<MeshTexture> m_Textures;
    std::shared_ptr<VertexBuffer> m_VertexBuffer;
    std::shared_ptr<IndexBuffer> m_IndexBuffer;
    std::shared_ptr<VertexArray> m_VertexArray;
};

} // namespace Vortex
