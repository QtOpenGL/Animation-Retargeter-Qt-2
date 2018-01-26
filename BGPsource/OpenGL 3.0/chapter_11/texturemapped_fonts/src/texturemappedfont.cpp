#ifdef WIN32
#include <windows.h>
#endif

#include "texturemappedfont.h"
#include <GL/glu.h>

TextureMappedFont::TextureMappedFont(const string& textureName, int screenWidth, int screenHeight, float fontSize)
{ 
    m_textureName = textureName;
    m_shaderProgram = new GLSLProgram("data/font.vert", "data/font.frag");
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
    m_fontSize = fontSize;
}

bool TextureMappedFont::initialize()
{
    if (!m_texture.load(m_textureName)) {
        std::cerr << "Couldn't load the font texture" << std::endl;
        return false;

    }
        
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, m_texture.getWidth(), 
                      m_texture.getHeight(), GL_RGB, GL_UNSIGNED_BYTE, m_texture.getImageData());


    float vertices [] = {
        0.0f, 0.0f,
        m_fontSize, 0.0f, 
        m_fontSize, m_fontSize,
        0.0f, m_fontSize
    };

    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, &vertices[0], GL_STATIC_DRAW);

    //Just initialize with something for now, the tex coords are updated
    //for each character printed
    float texCoords [] = {
        0.0f, 0.0f,
        0.0f, 0.0f, 
        0.0f, 0.0f,
        0.0f, 0.0f
    };

    glGenBuffers(1, &m_texCoordBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, &texCoords[0], GL_DYNAMIC_DRAW);

    if (!m_shaderProgram->initialize()) {
        std::cerr << "The shader program could not be initialized" << std::endl;
        return false;
    }

    m_shaderProgram->bindAttrib(0, "a_Vertex");
    m_shaderProgram->bindAttrib(1, "a_TexCoord0");
    m_shaderProgram->linkProgram();

    m_shaderProgram->sendUniform("texture0", 0);

    return true;
}

void TextureMappedFont::printString(const string& str, float x, float y) 
{
    static float modelviewMatrix[16];
    static float projectionMatrix[16];

    m_shaderProgram->bindShader();

    setOrthoMode();

    float texCoords[8];

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    glBindTexture(GL_TEXTURE_2D, m_textureID);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glTranslatef(x, y, 0.0); //Position our text
    for(string::size_type i = 0; i < str.size(); ++i) 
    {
        const float oneOverSixteen = 1.0f / 16.0f;

        int ch = int(str[i]);
        float xPos = float(ch % 16) * oneOverSixteen;
        float yPos = float(ch / 16) * oneOverSixteen;

        texCoords[0] = xPos;
        texCoords[1] = 1.0f - yPos - oneOverSixteen;

        texCoords[2] = xPos + oneOverSixteen;
        texCoords[3] = 1.0f - yPos - oneOverSixteen;

        texCoords[4] = xPos + oneOverSixteen;
        texCoords[5] = 1.0f - yPos - 0.001f;

        texCoords[6] = xPos;
        texCoords[7] = 1.0f - yPos - 0.001f;

        glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 8, &texCoords[0]);

        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
        glVertexAttribPointer((GLint)0, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
        glVertexAttribPointer((GLint)1, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMatrix);
        glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);

        m_shaderProgram->sendUniform4x4("modelview_matrix", modelviewMatrix);
        m_shaderProgram->sendUniform4x4("projection_matrix", projectionMatrix);

        glDrawArrays(GL_QUADS, 0, 4);
        
        glTranslatef(m_fontSize * 0.80f, 0, 0); //Move along a bit for the next character
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    unsetOrthoMode();
}

void TextureMappedFont::setOrthoMode()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, m_screenWidth, 0, m_screenHeight, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}

void TextureMappedFont::unsetOrthoMode()
{
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}