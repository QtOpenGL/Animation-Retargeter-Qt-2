#ifndef BOGLGP_ROBOT_HEADER
#define BOGLGP_ROBOT_HEADER

#include <GL/gl.h>
#include <GL/glu.h>
#include "glext.h"

#include <vector>
using std::vector;

extern PFNGLGENBUFFERSARBPROC glGenBuffers;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;

#pragma pack(push, 1)

struct Vertex {
    float x, y, z;
    Vertex(float x, float y, float z) 
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

struct Color {
    GLfloat r, g, b;
    Color(GLfloat r, GLfloat g, GLfloat b) 
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }    
};

#pragma pack(pop)

class Robot {
public:
    enum Buffers {
        VERTEX_BUFFER = 0,
        INDEX_BUFFER,
        COLOR_BUFFER,
        LAST_BUFFER
    };

    Robot();

    bool initialize();
    void animate(float dt);
    void render(float xPos, float yPos, float zPos);

private:
    enum Side {
        LEFT = 0,
        RIGHT,
        MAX_SIDES
    };

    enum Direction {
        FORWARD_STATE = 0,
        BACKWARD_STATE,
    };

    void renderCube(float xPos, float yPos, float zPos);
    void renderHead(float xPos, float yPos, float zPos);
    void renderArm(float xPos, float yPos, float zPos);
    void renderTorso(float xPos, float yPos, float zPos);
    void renderLeg(float xPos, float yPos, float zPos);
    void renderFoot(float xPos, float yPos, float zPos);

    vector<Vertex> m_vertices;
    vector<unsigned int> m_indices;

    static const int RED_OFFSET = 0;
    static const int BLUE_OFFSET = sizeof(Color) * 8;
    static const int YELLOW_OFFSET = sizeof(Color) * 16;
    static const int WHITE_OFFSET = sizeof(Color) * 24;

    vector<Color> m_colorBuffer;

    GLuint m_vbos[LAST_BUFFER];

    GLfloat m_armAngles[MAX_SIDES];
    GLfloat m_legAngles[MAX_SIDES];

    Direction m_armStates[MAX_SIDES];
    Direction m_legStates[MAX_SIDES];

    void initializeVertexBuffer();
    void initializeVertexColors();
};

#endif