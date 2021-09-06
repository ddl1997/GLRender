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
    /*  ��������  */
    std::vector<ModelVertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    /*  ����  */
    Mesh(std::vector<ModelVertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void Draw(Shader shader);
private:
    /*  ��Ⱦ����  */
    GLuint VAO, VBO, EBO;
    /*  ����  */
    void setupMesh();
};

#endif // !MESH_H
