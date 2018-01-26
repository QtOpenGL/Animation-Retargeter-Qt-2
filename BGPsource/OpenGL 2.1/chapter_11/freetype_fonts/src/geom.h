#ifndef BOGLGP_GEOM_H
#define BOGLGP_GEOM_H

struct Vertex 
{
    float x, y, z;
    Vertex() 
    {
        x = y = z = 0.0f;
    }

    Vertex(float x, float y, float z) 
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

struct TexCoord
{
    float s, t;
    TexCoord(float s, float t)
    {
        this->s = s;
        this->t = t;
    }
};

#endif