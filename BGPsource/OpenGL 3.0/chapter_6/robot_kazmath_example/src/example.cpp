#ifdef _WIN32
#include <windows.h>
#endif

#include "example.h"
#include <cstdio>
#include "glsl_shader.h"

Example::Example()
{
    m_shader = new GLSLProgram("data/basic-fixed.vert", "data/basic-fixed.frag");
}

Example::~Example()
{
    delete m_shader;
}

bool Example::init()
{
    if (!m_shader->initialize())
    {
        std::cerr << "Could not initialize the shaders" << std::endl;
        return false;
    }

    //Prepare the vertex attributes to be defined
    m_shader->bindAttrib(0, "a_Vertex");
	m_shader->bindAttrib(1, "a_Color");

	//Re link the program
	m_shader->linkProgram();
	m_shader->bindShader();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);

    m_robot.initialize(m_shader);
    m_rotationAngle = 0.0f;

    //Return success
    return true;
}

void Example::prepare(float dt)
{
    m_rotationAngle += 10.0f * dt;
    if (m_rotationAngle > 360.0f) {
        m_rotationAngle -= 360.0f;
    }

    m_robot.animate(dt);
}

void Example::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    kmGLLoadIdentity();

	kmGLPushMatrix();							// put current matrix on stack
		kmGLLoadIdentity();					// reset matrix
		kmGLTranslatef(0.0f, 0.0f, -30.0f);	// move to (0, 0, -30)
		kmGLRotatef(m_rotationAngle, 0.0f, 1.0f, 0.0f);	// rotate the robot on its y-axis
		m_robot.render(0.0f, 0.0f, 0.0f);
	kmGLPopMatrix();
}

void Example::shutdown()
{
}

void Example::onResize(int width, int height)
{
    glViewport(0, 0, width, height);

    kmGLMatrixMode(KM_GL_PROJECTION);
    kmGLLoadIdentity();


    //gluPerspective(52.0f, float(width) / float(height), 1.0f, 1000.0f);
    kmMat4 perspective;
    kmMat4PerspectiveProjection(&perspective, 52.0f, float(width) / float(height), 1.0f, 1000.0f);
    kmGLMultMatrix(&perspective);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMultMatrixf(perspective.mat);

    glMatrixMode(GL_MODELVIEW);

    kmGLMatrixMode(KM_GL_MODELVIEW);
    kmGLLoadIdentity();
}
