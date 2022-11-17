#include "model.h"

void Model::loadModel(std::string path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // ����ڵ����е���������еĻ���
    for (GLuint i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // �������������ӽڵ��ظ���һ����
    for (GLuint i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<ModelVertex> vertices;
    std::vector<unsigned int> indices;
    std::map<std::string, Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        ModelVertex vertex{};
        // ������λ�á����ߺ���������
        glm::vec3 vector;
        // ����λ��
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // ����
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;
        // ����
        vector.x = mesh->mTangents[i].x;
        vector.y = mesh->mTangents[i].y;
        vector.z = mesh->mTangents[i].z;
        vertex.Tangent = vector;
        // ��������
        if (mesh->mTextureCoords[0]) // �����Ƿ����������ꣿ
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // ��������
    for (GLuint i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (GLuint j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // �������
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::Diffuse, textures);
        //textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::Specular, textures);
        //textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

void Model::loadMaterialTextures(aiMaterial* mat, aiTextureType aiType, TextureType type, std::map<std::string, Texture> textures)
{
    for (unsigned int i = 0; i < mat->GetTextureCount(aiType); i++)
    {
        aiString str;
        mat->GetTexture(aiType, i, &str);
        std::string filename = str.C_Str();
        std::string path = directory + "/" + filename;
        std::map<std::string, Texture>::iterator iter = textures.find(path);
        if (iter == textures.end())
        {
            Texture texture(path);
            texture.type = type;
            textures[path] = texture;
        }
    }
}