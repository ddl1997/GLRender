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
    // 加载HDR辐照度贴图
    static GLuint importHDR(const char* filepath);
};

enum class TextureType {
    Diffuse = 0,
    Specular = 1,
    HDR = 9
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

    Texture(std::string filepath, TextureType texType)
    {
        switch (texType)
        {
        case TextureType::HDR:
            id = TextureImporter::importHDR(filepath.c_str());
            break;
        default:
            id = TextureImporter::importTexture2D(filepath.c_str(), 0, 0);
        }
        path = filepath;
    }

    GLuint getId() { return id; }
};

#endif
