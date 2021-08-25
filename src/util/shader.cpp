#include "shader.h"

Shader::Shader(std::string shaderName, int shaderList)
{
    this->id = glCreateProgram();
    std::string fileSrc = ShaderBaseDir + "/" + shaderName;

    if (shaderList & GLSL_VERTEX) {
        glAttachShader(this->id, loadGlslFile(fileSrc + ".vert", GL_VERTEX_SHADER));
    }
    if (shaderList & GLSL_GEOMETRY) {
        glAttachShader(this->id, loadGlslFile(fileSrc + ".geom", GL_GEOMETRY_SHADER));
    }
    if (shaderList & GLSL_FRAGMENT) {
        glAttachShader(this->id, loadGlslFile(fileSrc + ".frag", GL_FRAGMENT_SHADER));
    }
    
    glLinkProgram(id);
    checkCompileErrors(id, "program");
}

GLuint Shader::loadGlslFile(std::string fileSrc, GLuint shaderType)
{
    GLuint shader;

    // read shader file
    std::ifstream file;
    file.open(fileSrc, std::ios::in);
    file.seekg(0, std::ios::end);
    size_t length = file.tellg();
    file.seekg(0, std::ios::beg);
    char* shaderStr = (char*)malloc(sizeof(char) * (length + 1));
    file.read(shaderStr, length);
    shaderStr[length] = '\0';
    file.close();

    // create and compile shader
    shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderStr, NULL);
    glCompileShader(shader);
    checkCompileErrors(shader, getShaderTypeString(shaderType));

    return shader;
}