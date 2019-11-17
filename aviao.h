#ifndef AVIAO_H
#define AVIAO_H

#include <GL/glut.h>
#include <math.h>
#include "bolinha.h"
#include "projetil.h"
#include "bomba.h"

using namespace std;

class Aviao : public Bolinha
{
    private:
        GLfloat angulo;
        GLfloat initAngulo;
        bool dead;
        bool voando;
        GLfloat initX;
        GLfloat initY;
        GLfloat initRaio;
        GLfloat radiusTime;
        GLfloat speed;
        GLfloat anguloHelice;
        GLfloat anguloCanhao;

        void desenhaBolinhaModelo();
        void desenhaAviaoModelo();
        void desenhaCabine();
        void desenhaAsas();
        void desenhaTraseira();
        void desenhaHelices();
        void desenhaCanhao();

    public:
    	  // ------------ Construtor ------------ //
        Aviao(GLfloat& raio, GLfloat& x, GLfloat& y, GLfloat& r, GLfloat& g, GLfloat& b, GLint& id, GLfloat& angulo);

        // ------------ Getters e Setters ------------ //
        void incrementX(GLfloat x);
        void incrementY(GLfloat y);
        void setDead(bool state);
        GLfloat isDead();
        GLfloat getAngulo();
        GLfloat getAnguloEmGrau();
        void setAngulo(GLfloat& angulo);
        void incrementAngulo(GLfloat angulo);
        bool isVoando();
        void setVoando(bool flag);
        void setInitX(GLfloat x);
        GLfloat getInitX();
        void setInitY(GLfloat y);
        GLfloat getInitY();
        void setInitRaio(GLfloat raio);
        GLfloat getInitRaio();
        void setRadiusTime(GLfloat time);
        GLfloat getRadiusTime();
        void setSpeed(GLfloat speed);
        GLfloat getSpeed();
        void incrementSpeed(GLfloat accel);
        GLfloat getAnguloHelice();
        GLfloat getAnguloHeliceEmGrau();
        void setAnguloHelice(GLfloat angulo);
        void incrementAnguloHelice(GLfloat angulo);
        GLfloat getAnguloCanhao();
        GLfloat getAnguloCanhaoEmGrau();
        void setAnguloCanhao(GLfloat angulo);
        void incrementAnguloCanhao(GLfloat angulo);
        Projetil* atirar(GLfloat tiroSpeed);
        Bomba* bombardear();
        void reiniciar();
        void initializeSpeed(Linha* linha);

        // ------------ Desenho ------------ //
        void desenha();

        // ------------ Decolagem ------------ //
        void decolar(Linha* linha, GLint& currentTime);

        // ------------ Movimento ------------ //
        void andar(GLfloat movSpeed);

};

#endif /* AVIAO_H */
