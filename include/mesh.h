#ifndef MESH_H
#define MESH_H
#include "texture.h"
#include <vector>
#include "glm/glm.hpp"
#include "shader.h"

struct ModelVertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh {
public:
    /*  网格数据  */
    std::vector<ModelVertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    /*  函数  */
    Mesh(std::vector<ModelVertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void Draw(Shader shader);
private:
    /*  渲染数据  */
    GLuint VAO, VBO, EBO;
    /*  函数  */
    void setupMesh();
};

#endif // !MESH_H
