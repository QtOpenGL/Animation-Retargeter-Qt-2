#ifdef _WIN32
#include <windows.h>
#else
#define GLX_GLXEXT_LEGACY
#include <GL/glx.h>
#include "glx/glxext.h"
#endif

#include <iostream>
#include <fstream>
#include <GL/gl.h>
#include "glext.h"

#include "example.h"

PFNGLGENBUFFERSARBPROC glGenBuffers = NULL;
PFNGLBINDBUFFERPROC glBindBuffer = NULL;
PFNGLBUFFERDATAPROC glBufferData = NULL;


Example::Example()
{

}

bool Example::init()
{
    //Just an example, this will always return true in a GL3 context
    if (!isExtensionSupported("GL_ARB_vertex_buffer_object"))
    {
        return false;
    }

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
    glClearColor(0.0f, 0.5f, 0.9f, 0.5f);

    glEnableClientState(GL_VERTEX_ARRAY);

    if (!m_terrain.loadHeightmap("data/heightmap.raw", 65))
    {
        std::cerr << "Could not load the terrain" << std::endl;
        return false;
    }

    //Return success
    return true;
}



void Example::prepare(float dt)
{

}

void Example::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //Load the identity matrix (reset to the default position and orientation)
    glLoadIdentity();

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glTranslatef(0.0f, -20.0f, 0.0f);
    glRotatef(25.0f, 1.0f, 0.0f, 0.0f);
    //Translate using our zPosition variable
    glTranslatef(0.0, 0.0, -50.0f);

    m_terrain.render();
}

void Example::shutdown()
{

}

void Example::onResize(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(52.0f, float(width) / float(height), 1.0f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

vector<string> Example::getSupportedExtensions()
{
    vector<string> extensions;

    string extensionString((const char*)glGetString(GL_EXTENSIONS));

    //This code simply splits the string up by spaces and stores each extension in the array
    string::size_type start = 0;
    string::size_type end = extensionString.find_first_of(' ');
    while(end != string::npos)
    {
        string extension = extensionString.substr(start, end - start);
        extensions.push_back(extension);
        start = end + 1;
        end = extensionString.find_first_of(' ', start);
    }

    return extensions;
}

bool Example::isExtensionSupported(const string& ext)
{
    //Get a list of extensions
    vector<string> extensions = getSupportedExtensions();

    /*Go through all the extensions and if the current one matches the one we are
    looking for then return true */
    for (vector<string>::iterator it = extensions.begin(); it != extensions.end(); ++it)
    {
        if (*it == ext)
        {
            return true;
        }
    }

    /* If we go through all the extensions and none match then return false */
    return false;
}
