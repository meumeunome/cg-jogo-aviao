#ifndef PROJETIL_H
#define PROJETIL_H

#include <GL/glut.h>
#include <math.h>
#include "bolinha.h"

using namespace std;

class Projetil : public Bolinha
{
    private:
        GLfloat angulo;
        GLfloat speed;

    public:
    	  // ------------ Construtor ------------ //
        Projetil(GLfloat& raio, GLfloat& x, GLfloat& y, GLfloat& r, GLfloat& g, GLfloat& b, GLint& id, GLfloat& speed, GLfloat& angulo);

        // ------------ Getters e Setters ------------ //
        void incrementX(GLfloat x);
        void incrementY(GLfloat y);
        GLfloat getAngulo();
        GLfloat getAnguloEmGrau();
        void setAngulo(GLfloat& angulo);
        void incrementAngulo(GLfloat angulo);
        void setSpeed(GLfloat speed);
        GLfloat getSpeed();
        void incrementSpeed(GLfloat accel);

        // ------------ Movimento ------------ //
        void andar(GLfloat movSpeed);

};

#endif /* PROJETIL_H */
