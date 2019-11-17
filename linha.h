#ifndef LINHA_H
#define LINHA_H

#include <GL/glut.h>
#include <math.h>
#include <iostream>

using namespace std;

class Linha
{
    private:
        GLfloat x1;
        GLfloat y1;
        GLfloat x2;
        GLfloat y2;
        GLfloat r;
        GLfloat g;
        GLfloat b;
        GLint id;

    public:
    	  // ------------ Construtor ------------ //
        Linha(GLfloat& x1, GLfloat& y1, GLfloat& x2, GLfloat& y2, GLfloat& r, GLfloat& g, GLfloat& b, GLint& id);

        // ------------ Getters e Setters ------------ //
        GLfloat getX1();
        void setX1(GLfloat& x1);
        GLfloat getY1();
        void setY1(GLfloat& y1);
        GLfloat getX2();
        void setX2(GLfloat& x2);
        GLfloat getY2();
        void setY2(GLfloat& y2);
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

#endif /* LINHA_H */
