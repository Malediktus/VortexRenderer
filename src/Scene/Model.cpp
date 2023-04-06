#include <Vortex/Scene/Model.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <cstdlib>

using namespace Vortex;

Scene::Model::Model(const std::string& filepath) {
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        exit(1);
    }
    m_Directory = filepath.substr(0, filepath.find_last_of('/'));

    ProcessNode(scene->mRootNode, scene);
}

void Scene::Model::ProcessNode(void* processNode, const void* processScene) {
    aiNode* node = (aiNode*) processNode;
    const aiScene* scene = (const aiScene*) processScene;

    for (uint32_t i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_Meshes.push_back(ProcessMesh(mesh, scene));
    }
    for (uint32_t i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene);
    }
}

Scene::Mesh Scene::Model::ProcessMesh(void* processMesh, const void* processScene) {
    aiMesh* mesh = (aiMesh*) processMesh;
    const aiScene* scene = (const aiScene*) processScene;

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<Texture> textures;

    for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;
        if (mesh->mTextureCoords[0]) {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        } else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }

    for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (uint32_t j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Scene::Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Scene::Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Scene::Texture> Scene::Model::LoadMaterialTextures(aiMaterial* mat, int textureType, const std::string& typeName) {
    aiTextureType type = (aiTextureType) textureType;

    std::vector<Scene::Texture> textures;
    for (uint32_t i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string filepath = m_Directory;
        filepath.append("/");
        filepath.append(str.C_Str());
        bool skip = false;
        for (uint32_t j = 0; j < m_LoadedTextures.size(); j++) {
            if (m_LoadedTextures[j].Filepath == filepath) {
                textures.push_back(m_LoadedTextures[j]);
                skip = true;
                break;
            }
        }
        if (!skip) {
            Texture texture;
            texture.Texture = Texture2DCreate(filepath);
            texture.Type = typeName;
            texture.Filepath = str.C_Str();
            textures.push_back(texture);
            m_LoadedTextures.push_back(texture);
        }
    }
    return textures;
}
