#ifndef BOLINHA_H
#define BOLINHA_H

#include <GL/glut.h>
#include <math.h>
#include "linha.h"

using namespace std;

class Bolinha
{
    private:
        GLfloat x;
        GLfloat y;
        GLfloat raio;
        GLfloat r;
        GLfloat g;
        GLfloat b;
        GLint id;

    public:
    	  // ------------ Construtor ------------ //
        Bolinha(GLfloat& raio, GLfloat& x, GLfloat& y, GLfloat& r, GLfloat& g, GLfloat& b, GLint& id);

        // ------------ Getters e Setters ------------ //
        GLfloat getX();
        void setX(GLfloat x);
        GLfloat getY();
        void setY(GLfloat y);
        GLfloat getRaio();
        void setRaio(GLfloat raio);
        GLfloat getR();
        void setR(GLfloat& r);
        GLfloat getG();
        void setG(GLfloat& g);
        GLfloat getB();
        void setB(GLfloat& b);
        GLint getId();
        void setId(GLint& id);

        // ------------ Desenho ------------ //
        void desenha();

};

#endif /* BOLINHA_H */
