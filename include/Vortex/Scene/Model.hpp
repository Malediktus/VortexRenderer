#include "Mesh.hpp"

struct aiMaterial;

namespace Vortex::Scene {
class Model {
public:
    VT_API Model(const std::string& filepath);
    VT_API const std::vector<Scene::Mesh>& GetMeshes() const {
        return m_Meshes;
    }

private:
    void ProcessNode(void* processNode, const void* processScene);
    Mesh ProcessMesh(void* processMesh, const void* processScene);
    std::vector<Scene::Texture> LoadMaterialTextures(aiMaterial* mat, int textureType, const std::string& typeName);

    std::vector<Scene::Texture> m_LoadedTextures;
    std::vector<Scene::Mesh> m_Meshes;
    std::string m_Directory;
};
} // namespace Vortex::Scene
