#ifdef _WIN32
#include <windows.h>
#else
#include <GL/glx.h>
#endif

#include <iostream>

#include "robot.h"

PFNGLGENBUFFERSARBPROC glGenBuffers = NULL;
PFNGLBINDBUFFERPROC glBindBuffer = NULL;
PFNGLBUFFERDATAPROC glBufferData = NULL;

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

Robot::Robot()
{
	m_armAngles[LEFT] = 0.0;
	m_armAngles[RIGHT] = 0.0;
	m_legAngles[LEFT] = 0.0;
	m_legAngles[RIGHT] = 0.0;

	m_armStates[LEFT] = FORWARD_STATE;
	m_armStates[RIGHT] = BACKWARD_STATE;

	m_legStates[LEFT] = FORWARD_STATE;
	m_legStates[RIGHT] = BACKWARD_STATE;
}

void Robot::initializeVertexBuffer()
{
    const float radius = 0.5f;
    //Push back 8 vertices that make up a cube
    m_vertices.push_back(Vertex(-radius, -radius,  radius));
    m_vertices.push_back(Vertex(-radius, -radius, -radius));
    m_vertices.push_back(Vertex(radius, -radius, -radius));
    m_vertices.push_back(Vertex(radius, -radius,  radius));

    m_vertices.push_back(Vertex(-radius, radius,  radius));
    m_vertices.push_back(Vertex(-radius, radius, -radius));
    m_vertices.push_back(Vertex( radius, radius, -radius));
    m_vertices.push_back(Vertex( radius, radius,  radius));

    //Push back the indices that make up the triangles for each face.
    m_indices.push_back(0);
    m_indices.push_back(2);
    m_indices.push_back(3);
    m_indices.push_back(0);
    m_indices.push_back(1);
    m_indices.push_back(2);

    m_indices.push_back(4);
    m_indices.push_back(6);
    m_indices.push_back(7);
    m_indices.push_back(4);
    m_indices.push_back(5);
    m_indices.push_back(6);

    m_indices.push_back(0);
    m_indices.push_back(4);
    m_indices.push_back(1);
    m_indices.push_back(4);
    m_indices.push_back(5);
    m_indices.push_back(1);

    m_indices.push_back(2);
    m_indices.push_back(6);
    m_indices.push_back(3);
    m_indices.push_back(6);
    m_indices.push_back(7);
    m_indices.push_back(3);

    m_indices.push_back(6);
    m_indices.push_back(1);
    m_indices.push_back(5);
    m_indices.push_back(6);
    m_indices.push_back(2);
    m_indices.push_back(1);

    m_indices.push_back(0);
    m_indices.push_back(7);
    m_indices.push_back(4);
    m_indices.push_back(0);
    m_indices.push_back(3);
    m_indices.push_back(7);


    glBindBuffer(GL_ARRAY_BUFFER, m_vbos[VERTEX_BUFFER]); //Bind the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW); //Send the data to OpenGL

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbos[INDEX_BUFFER]); //Bind the vertex buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_indices.size(), &m_indices[0], GL_STATIC_DRAW); //Send the data to OpenGL
}

void Robot::initializeVertexColors()
{
    /*
        We build an array of colors, 8 reds, 8 blues, 8 yellows and 8 whites.
        This is because there are 8 vertices in a cube, we switch the color by
        setting the BUFFER_OFFSET to glColorPointer to point at each block
        of colors before rendering with glDrawElements
    */

    float red [] = { 1.0f, 0.0f, 0.0f };
    for (int i = 0; i < 8; ++i) {
        m_colorBuffer.push_back(Color(red[0], red[1], red[2]));
    }

    float blue [] = { 0.0f, 0.0f, 1.0f };
    for (int i = 0; i < 8; ++i) {
        m_colorBuffer.push_back(Color(blue[0], blue[1], blue[2]));
    }

    float yellow [] = { 1.0f, 1.0f, 0.0f };
    for (int i = 0; i < 8; ++i) {
        m_colorBuffer.push_back(Color(yellow[0], yellow[1], yellow[2]));
    }

    float white [] = { 1.0f, 1.0f, 1.0f };
    for (int i = 0; i < 8; ++i) {
        m_colorBuffer.push_back(Color(white[0], white[1], white[2]));
    }

    //Send the color data to OpenGL
    glBindBuffer(GL_ARRAY_BUFFER, m_vbos[COLOR_BUFFER]); //Bind the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * m_colorBuffer.size(), &m_colorBuffer[0], GL_STATIC_DRAW); //Send the data to OpenGL
}

bool Robot::initialize()
{
#ifdef _WIN32
    glGenBuffers = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffers");
    glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
    glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
#else
    glGenBuffers = (PFNGLGENBUFFERSARBPROC)glXGetProcAddress((const GLubyte*)"glGenBuffers");
    glBindBuffer = (PFNGLBINDBUFFERPROC)glXGetProcAddress((const GLubyte*)"glBindBuffer");
    glBufferData = (PFNGLBUFFERDATAPROC)glXGetProcAddress((const GLubyte*)"glBufferData");
#endif

    if (!glGenBuffers || !glBindBuffer || !glBufferData)
    {
        std::cerr << "VBOs are not supported by your graphics card" << std::endl;
        return false;
    }

    glGenBuffers(LAST_BUFFER, m_vbos); //Generate a buffer for the vertices, indices and colors

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    initializeVertexBuffer(); //Build up the array of vertices for the cube
    initializeVertexColors(); //Build up an array of colors for the robot

    //Set the vertex pointer for the cube
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbos[INDEX_BUFFER]); //Bind the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_vbos[VERTEX_BUFFER]);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    //Bind the default color as white, this changes depending on the part of the robot we are drawing
    glBindBuffer(GL_ARRAY_BUFFER, m_vbos[COLOR_BUFFER]);
    glColorPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(WHITE_OFFSET)); //Default color white

    return true;
}

void Robot::render(float xPos, float yPos, float zPos)
{
    glPushMatrix();
        glTranslatef(xPos, yPos, zPos);	// move to (0, 0, -30)
        // draw head and torso parts
	    renderHead(0.0f, 3.5f, 0.0f);
        renderTorso(0.0f, 0.0f, 0.0f);

	    // move the left arm away from the torso and rotate it to give "walking" effect
	    glPushMatrix();
		    glTranslatef(0.0f, -0.5f, 0.0f);
		    glRotatef(m_armAngles[LEFT], 1.0f, 0.0f, 0.0f);
		    renderArm(2.0f, 0.0f, -0.0f);
	    glPopMatrix();

	    // move the right arm away from the torso and rotate it to give "walking" effect
	    glPushMatrix();
		    glTranslatef(0.0f, -0.5f, 0.0f);
		    glRotatef(m_armAngles[RIGHT], 1.0f, 0.0f, 0.0f);
		    renderArm(-2.0f, 0.0f, -0.0f);
	    glPopMatrix();

	    // move the left leg away from the torso and rotate it to give "walking" effect
	    glPushMatrix();
		    glTranslatef(0.0f, -0.5f, 0.0f);
		    glRotatef(m_legAngles[LEFT], 1.0f, 0.0f, 0.0f);
		    renderLeg(-1.0f, -5.0f, -0.5f);
	    glPopMatrix();

	    // move the right leg away from the torso and rotate it to give "walking" effect
	    glPushMatrix();
		    glTranslatef(0.0f, -0.5f, 0.0f);
		    glRotatef(m_legAngles[RIGHT], 1.0f, 0.0f, 0.0f);
		    renderLeg(1.0f, -5.0f, -0.5f);
	    glPopMatrix();
    glPopMatrix();
}

void Robot::renderCube(float xPos, float yPos, float zPos)
{
    glPushMatrix();
    glTranslatef(xPos, yPos, zPos);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glPopMatrix();
}

void Robot::renderHead(float xPos, float yPos, float zPos)
{
	glPushMatrix();
		glTranslatef(xPos, yPos, zPos);
		glScalef(2.0f, 2.0f, 2.0f);		// head is a 2x2x2 cube

        glBindBuffer(GL_ARRAY_BUFFER, m_vbos[COLOR_BUFFER]);
        glColorPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(WHITE_OFFSET)); //Set the color to white
		renderCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

void Robot::renderArm(float xPos, float yPos, float zPos)
{
	glPushMatrix();
		glTranslatef(xPos, yPos, zPos);
		glScalef(1.0f, 5.0f, 1.0f);		// arm is a 1x5x1 cube

        glBindBuffer(GL_ARRAY_BUFFER, m_vbos[COLOR_BUFFER]);
        glColorPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(RED_OFFSET)); //Set the color to red
		renderCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

void Robot::renderTorso(float xPos, float yPos, float zPos)
{
	glPushMatrix();
		glTranslatef(xPos, yPos, zPos);
		glScalef(3.0f, 5.0f, 2.0f);		// torso is a 3x5x2 cube

        glBindBuffer(GL_ARRAY_BUFFER, m_vbos[COLOR_BUFFER]);
        glColorPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(BLUE_OFFSET)); //Set the color to blue
		renderCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

void Robot::renderLeg(float xPos, float yPos, float zPos)
{
	glPushMatrix();
		glTranslatef(xPos, yPos, zPos);

		// draw the foot
		glPushMatrix();
			glTranslatef(0.0f, -0.5f, 1.0f);
			renderFoot(0.0f, -3.0f, 0.0f);
		glPopMatrix();

		glScalef(1.0f, 5.0f, 1.0f);		// leg is a 1x5x1 cube
        glBindBuffer(GL_ARRAY_BUFFER, m_vbos[COLOR_BUFFER]);
        glColorPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(YELLOW_OFFSET)); //Set the color to yellow
		renderCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

void Robot::renderFoot(float xPos, float yPos, float zPos)
{
	glPushMatrix();
		glTranslatef(xPos, yPos, zPos);
		glScalef(1.0f, 0.5f, 3.0f);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbos[COLOR_BUFFER]);
        glColorPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(WHITE_OFFSET)); //Set the color to white
		renderCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

void Robot::animate(float dt)
{
    const float armRotationSpeed = 20.0f;
    const float maxArmAngle = 15.0f;
    const float legRotationSpeed = 30.0f;
    const float maxLegAngle = 15.0f;

    for (unsigned int side = 0; side < MAX_SIDES; ++side) {
	    // arms
        if (m_armStates[side] == FORWARD_STATE) {
		    m_armAngles[side] += armRotationSpeed * dt;
        } else {
		    m_armAngles[side] -= armRotationSpeed * dt;
        }

        		// change state if exceeding angles
        if (m_armAngles[side] >= maxArmAngle) {
			m_armStates[side] = BACKWARD_STATE;
        } else if (m_armAngles[side] <= -maxArmAngle) {
			m_armStates[side] = FORWARD_STATE;
        }

        // legs
        if (m_legStates[side] == FORWARD_STATE) {
			m_legAngles[side] += legRotationSpeed * dt;
        } else {
			m_legAngles[side] -= legRotationSpeed * dt;
        }

		// change state if exceeding angles
        if (m_legAngles[side] >= maxLegAngle) {
			m_legStates[side] = BACKWARD_STATE;
        } else if (m_legAngles[side] <= -maxLegAngle) {
			m_legStates[side] = FORWARD_STATE;
        }
    }
}
