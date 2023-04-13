#include <Vortex/Scene/Mesh.hpp>
#include <tracy/Tracy.hpp>

using namespace Vortex;

ModelMesh::ModelMesh(const std::vector<MeshVertex>& vertices, const std::vector<uint32_t>& indices, std::vector<MeshTexture>& textures)
    : m_Indices(indices), m_Vertices(vertices), m_Textures(textures) {
    ZoneScoped;
    BufferLayout layout = BufferLayout({
        {ShaderDataType::Float3, "Position", false},
        {ShaderDataType::Float3, "Normal", false},
        {ShaderDataType::Float2, "TexCoords", false},
    });
    m_VertexBuffer = VertexBufferCreate((float*) m_Vertices.data(), (uint32_t) m_Vertices.size() * sizeof(m_Vertices[0]));
    m_VertexBuffer->SetLayout(layout);
    m_IndexBuffer = IndexBufferCreate((uint32_t*) m_Indices.data(), (uint32_t) m_Indices.size() * sizeof(m_Indices[0]));
    m_VertexArray = VertexArrayCreate();
    m_VertexArray->AddVertexBuffer(m_VertexBuffer);
    m_VertexArray->SetIndexBuffer(m_IndexBuffer);
}

void ModelMesh::BindTextures(const std::shared_ptr<Shader>& shader) const {
    ZoneScoped;
    uint32_t diffuseNr = 0;
    uint32_t specularNr = 0;
    for (uint32_t i = 0; i < m_Textures.size(); i++) {
        std::string number;
        std::string name = m_Textures[i].Type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);

        shader->SetInt("u_Material." + name + "[" + number + "]", i);
        m_Textures[i].Texture->Bind(i);
    }
    shader->SetInt("u_Material.NumDiffuseTextures", diffuseNr);
    shader->SetInt("u_Material.NumSpecularTextures", specularNr);
}
