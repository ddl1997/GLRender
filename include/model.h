#ifndef MODEL_H
#define MODEL_H
#include "shader.h"
#include "mesh.h"
#include <vector>
#include "assimp/Importer.hpp"
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
    /*  ����   */
    Model(char* path)
    {
        loadModel(path);
    }
    void Draw(Shader shader);
private:
    /*  ģ������  */
    std::vector<Mesh> meshes;
    std::string directory;
    /*  ����   */
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType aiType, TextureType type);
};

#endif // !MODEL_H
