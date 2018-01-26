#ifndef _EXAMPLE_H
#define _EXAMPLE_H

class Example 
{
public:
    Example();

    bool init();
    void prepare(float dt);
    void render();
    void shutdown();

    void onResize(int width, int height);

private:
    float m_rotationAngle;
};

#endif
