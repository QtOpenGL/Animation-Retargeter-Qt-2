#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#include "example.h"

Example::Example()
{
    m_rotationAngle = 0.0f;
}

bool Example::init()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

    //Return success
    return true;
}

void Example::prepare(float dt)
{
    const float SPEED = 15.0f;
    m_rotationAngle += SPEED * dt;
    if (m_rotationAngle > 360.0f)
    {
        m_rotationAngle -= 360.0f;
    }
}

void Example::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glRotatef(m_rotationAngle, 0, 0, 1);

	glBegin(GL_TRIANGLES);
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	    glVertex3f(-1.0f, -0.5f, -4.0f);
		glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
		glVertex3f(1.0f, -0.5f, -4.0f);
		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
		glVertex3f(0.0f,  0.5f, -4.0f);
	glEnd();
}

void Example::shutdown()
{

}

void Example::onResize(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f, float(width) / float(height), 1.0f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
