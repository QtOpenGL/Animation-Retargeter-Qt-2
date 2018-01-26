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

    Vertex(const Vertex& v) {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;        
    }

    Vertex& operator=(const Vertex& v) {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;        

        return *this;
    }
};

struct TexCoord
{
    float s, t;
    TexCoord():
    s(0.0f),
    t(0.0f) {
        
    }
    TexCoord(float s, float t)
    {
        this->s = s;
        this->t = t;
    }
};

#endif