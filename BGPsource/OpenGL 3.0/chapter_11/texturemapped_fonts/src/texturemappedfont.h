#ifndef BOGLGP_TEXTURE_MAPPED_FONT_H
#define BOGLGP_TEXTURE_MAPPED_FONT_H

#include <string>

#include "glee/GLee.h"
#include "targa.h"
#include "glslshader.h"

class TextureMappedFont {
public:
    TextureMappedFont(const std::string& fontTexture, int screenWidth, int screenHeight, float fontSize=16.0f);

    bool initialize();
    void printString(const std::string& str, float x, float y);

private:
    GLuint m_textureID;
    TargaImage m_texture;
    string m_textureName;
    
    GLuint m_texCoordBuffer;
    GLuint m_vertexBuffer;
    float m_fontSize;

    int m_screenWidth;
    int m_screenHeight;

    void setOrthoMode();
    void unsetOrthoMode();

    GLSLProgram* m_shaderProgram;
};

#endif