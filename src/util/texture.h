#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <iostream>

class TextureImporter
{
public:
    // 加载2D纹理贴图
    static GLuint importTexture2D(const char* filepath, GLint level, GLint border);
};

enum class TextureType {
    Diffuse = 0,
    Specular = 1
};

class Texture {
private:
    GLuint id;
    std::string path;

public:
    TextureType type;

    Texture() : id(0) {}

    Texture(std::string filepath)
    {
        id = TextureImporter::importTexture2D(filepath.c_str(), 0, 0);
        path = filepath;
    }

    GLuint getId() { return id; }
};

#endif
