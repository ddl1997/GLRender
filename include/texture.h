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

#endif
