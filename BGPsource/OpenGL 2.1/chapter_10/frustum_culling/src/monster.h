#ifndef BOGLGP_MONSTER_H
#define BOGLGP_MONSTER_H

#include "md2model.h"

class GLSLProgram;

class Monster {
public:
    Monster(MD2Model* model, float x, float y, float z):
    x(x),
    y(y),
    z(z) {
        this->m_model = model;
    }

    void render(GLSLProgram* program) {
        glPushMatrix();
            glTranslatef(x, y, z);
            m_model->render(program);
        glPopMatrix();
    }

    void getXYZ(float &x, float &y, float &z)
    {
        x = this->x;
        y = this->y;
        z = this->z;
    }

private:
    MD2Model* m_model;
    float x, y, z;
};


#endif