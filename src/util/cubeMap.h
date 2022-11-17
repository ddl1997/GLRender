#ifndef CUBE_MAP_H
#define CUBE_MAP_H

#include "global.h"
#include <vector>
#include <string>
#include "camera.h"
#include "shader.h"

class CubeMap {
public:

    CubeMap() : id(0), cubeMapVAO(0), cubeMapVBO(0), cubeMapShader(nullptr) {};

    CubeMap(std::vector<std::string> faceFilePathes)
    {
        if (faceFilePathes.size() <= 6)
        {
            loadCubeMap(faceFilePathes);
            float cubeMapVertices[] = {
                // positions          
                -1.0f,  1.0f, -1.0f,
                -1.0f, -1.0f, -1.0f,
                 1.0f, -1.0f, -1.0f,
                 1.0f, -1.0f, -1.0f,
                 1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,

                -1.0f, -1.0f,  1.0f,
                -1.0f, -1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f,  1.0f,
                -1.0f, -1.0f,  1.0f,

                 1.0f, -1.0f, -1.0f,
                 1.0f, -1.0f,  1.0f,
                 1.0f,  1.0f,  1.0f,
                 1.0f,  1.0f,  1.0f,
                 1.0f,  1.0f, -1.0f,
                 1.0f, -1.0f, -1.0f,

                -1.0f, -1.0f,  1.0f,
                -1.0f,  1.0f,  1.0f,
                 1.0f,  1.0f,  1.0f,
                 1.0f,  1.0f,  1.0f,
                 1.0f, -1.0f,  1.0f,
                -1.0f, -1.0f,  1.0f,

                -1.0f,  1.0f, -1.0f,
                 1.0f,  1.0f, -1.0f,
                 1.0f,  1.0f,  1.0f,
                 1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f, -1.0f,

                -1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f,  1.0f,
                 1.0f, -1.0f, -1.0f,
                 1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f,  1.0f,
                 1.0f, -1.0f,  1.0f
            };
            glGenVertexArrays(1, &cubeMapVAO);
            glGenBuffers(1, &cubeMapVBO);
            glBindVertexArray(cubeMapVAO);
            glBindBuffer(GL_ARRAY_BUFFER, cubeMapVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(cubeMapVertices), &cubeMapVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

            cubeMapShader = new Shader("cubemap", GLSL_VERTEX | GLSL_FRAGMENT);
            cubeMapShader->use();
            cubeMapShader->setInt("skybox", 0);
        }
        else
        {
            id = 0;
            cubeMapVAO = 0;
            cubeMapVBO = 0;
            cubeMapShader = nullptr;
        }
            
    }

    ~CubeMap()
    {
        delete cubeMapShader;
    }

    GLuint getId() { return id; }

    void draw(Camera* camera)
    {
        if (cubeMapShader != nullptr)
        {
            glDepthFunc(GL_LEQUAL);
            cubeMapShader->use();
            glm::mat4 view = glm::mat4(glm::mat3(camera->GetViewMatrix())); // remove translation from the view matrix
            glm::mat4 projection = camera->GetPerspectiveProjectionMatrix();
            cubeMapShader->setMat4("view", view);
            cubeMapShader->setMat4("projection", projection);
            glBindVertexArray(cubeMapVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, id);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
            glDepthFunc(GL_LESS);
        }
    }

private:
    GLuint id;
    GLuint cubeMapVAO, cubeMapVBO;
    Shader* cubeMapShader;

    void loadCubeMap(std::vector<std::string> faces)
    {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(false);
        for (GLuint i = 0; i < faces.size(); i++)
        {
            unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                );
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    }
};
#endif // !CUBE_MAP_H

