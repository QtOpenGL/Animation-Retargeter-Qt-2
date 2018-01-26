#ifdef _WIN32
#include <windows.h>
#include "glwindow.h"
#else
#include <GL/glx.h>
#include "glxwindow.h"
#endif

#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include "glext.h"
#include "example.h"



#define BUFFER_OFFSET(i) ((char *)NULL + (i))

PFNGLGENBUFFERSARBPROC glGenBuffers = NULL;
PFNGLBINDBUFFERPROC glBindBuffer = NULL;
PFNGLBUFFERDATAPROC glBufferData = NULL;

Example::Example()
{

}

void Example::initializeCube()
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

    m_colors.push_back(Color(1.0f, 0.0f, 0.0f)); //Red
    m_colors.push_back(Color(1.0f, 1.0f, 0.0f)); //Yellow
    m_colors.push_back(Color(0.0f, 1.0f, 0.0f)); //Green
    m_colors.push_back(Color(0.0f, 1.0f, 1.0f)); //
    m_colors.push_back(Color(0.0f, 0.0f, 1.0f)); //Blue
    m_colors.push_back(Color(1.0f, 0.0f, 1.0f)); //Purple
    m_colors.push_back(Color(1.0f, 1.0f, 1.0f)); //White
    m_colors.push_back(Color(0.5f, 0.5f, 0.5f)); //Grey

    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    glGenBuffers(MAX_BUFFERS, &m_vbos[0]);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbos[VERTEX_BUFFER]); //Bind the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW); //Send the data to OpenGL

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbos[INDEX_BUFFER]); //Bind the vertex buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_indices.size(), &m_indices[0], GL_STATIC_DRAW); //Send the data to OpenGL

    glBindBuffer(GL_ARRAY_BUFFER, m_vbos[COLOR_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * m_colors.size(), &m_colors[0], GL_STATIC_DRAW); //Send the data to OpenGL

    //Bind the color array, and set the color pointer to point at it
    glBindBuffer(GL_ARRAY_BUFFER, m_vbos[COLOR_BUFFER]);
    glColorPointer(3, GL_FLOAT, 0, 0);

    //Bind the vertex array and set the vertex pointer to point at it
    glBindBuffer(GL_ARRAY_BUFFER, m_vbos[VERTEX_BUFFER]);
    glVertexPointer(3, GL_FLOAT, 0, 0);
}

void Example::renderCube()
{
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
}

bool Example::init()
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

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);

    initializeCube();

    glLineWidth(2);

    //Return success
    return true;
}

void Example::prepare(float dt)
{
#ifdef _WIN32
    static float lastTimeSpacePressed = GLWindow::getCurrentTime();
    float currentTime = GLWindow::getCurrentTime(); //Get the current time
#else
    static float lastTimeSpacePressed = SimpleGLXWindow::getCurrentTime();
    float currentTime = SimpleGLXWindow::getCurrentTime(); //Get the current time
#endif

    const float keyWait = 300.0f; //How long before we allow another key press to register


    /*
     If the key is down, and some time has passed
     since the last press we toggle the perspective.
     If we don't use a timer then the value will toggle
     too quickly because several frames may pass during the
     time the key is pressed down
    */
#ifdef _WIN32
    bool spaceBarPressed = GLWindow::isKeyPressed(VK_SPACE);
#else
    bool spaceBarPressed = SimpleGLXWindow::isKeyPressed(XK_space);
#endif

    if (spaceBarPressed &&
        currentTime > (lastTimeSpacePressed + keyWait)) {
        m_perspective = !m_perspective;
        updateProjection();
        lastTimeSpacePressed = currentTime;
    }
}

void Example::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //Load the identity matrix (reset to the default position and orientation)
    glLoadIdentity();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// position and size the cube
	glTranslatef(0.0, 0.0, -2.5);
	glRotatef(15.0, 1.0, 0.0, 0.0);
	glRotatef(30.0f, 0.0, 1.0, 0.0);
	glScalef(0.75, 0.75, 0.75);
    renderCube();

}

void Example::shutdown()
{

}

void Example::updateProjection()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (m_perspective) {
        // set the perspective with the appropriate aspect ratio
		glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 1000.0);
    } else {
        // set up an orthographic projection with the same near clip plane
		glOrtho(-1.0, 1.0, -1.0, 1.0, 1.0, 1000.0);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Example::onResize(int width, int height)
{
    if (height == 0)
    {
        height = 1;
    }

    // set the viewport to the new window size
    glViewport(0, 0, width, height);

    //set the projection based on the m_perspective flag
    updateProjection();
}
