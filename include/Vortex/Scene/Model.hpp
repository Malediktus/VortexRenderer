#pragma once
#include "Mesh.hpp"

struct aiMaterial;

namespace Vortex {
class Model {
public:
    VT_API Model(const std::string& filepath);
    VT_API const std::vector<ModelMesh>& GetMeshes() const {
        return m_Meshes;
    }

private:
    void ProcessNode(void* processNode, const void* processScene);
    ModelMesh ProcessMesh(void* processMesh, const void* processScene);
    std::vector<MeshTexture> LoadMaterialTextures(aiMaterial* mat, int textureType, const std::string& typeName);

    std::vector<MeshTexture> m_LoadedTextures;
    std::vector<ModelMesh> m_Meshes;
    std::string m_Directory;
};
} // namespace Vortex
