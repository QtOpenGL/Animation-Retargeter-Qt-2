#ifndef _EXAMPLE_H
#define _EXAMPLE_H

class FreeTypeFont;

class Example 
{
public:
    Example();
    virtual ~Example();

    bool init();
    void prepare(float dt);
    void render();
    void shutdown();

    void onResize(int width, int height);

private:
    float m_angle;

    FreeTypeFont* m_font;
};

#endif