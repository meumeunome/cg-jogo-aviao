#include "bolinha.h"

Bolinha::Bolinha(GLfloat& raio, GLfloat& x, GLfloat& y, GLfloat& r, GLfloat& g, GLfloat& b, GLint& id){
    this->raio = raio;
    this->x = x;
    this->y = y;
    this->r = r;
    this->g = g;
    this->b = b;
    this->id = id;
}

GLfloat Bolinha::getX(){
	return this->x;
}
void Bolinha::setX(GLfloat x){
	this->x = x;
}

GLfloat Bolinha::getY(){
	return this->y;
}
void Bolinha::setY(GLfloat y){
	this->y = y;
}

GLfloat Bolinha::getRaio() {
    return this->raio;
}
void Bolinha::setRaio(GLfloat raio) {
    this->raio = raio;
}

GLfloat Bolinha::getR(){
	return this->r;
}
void Bolinha::setR(GLfloat& r){
	this->r = r;
}

GLfloat Bolinha::getG(){
	return this->g;
}
void Bolinha::setG(GLfloat& g){
	this->g = g;
}

GLfloat Bolinha::getB(){
	return this->b;
}
void Bolinha::setB(GLfloat& b){
  this->b = b;
}

GLint Bolinha::getId(){
  return this->id;
}
void Bolinha::setId(GLint& id){
  this->id = id;
}

void Bolinha::desenha(){
  float theta, auxX, auxY;
  GLfloat raio = this->getRaio();

  glPushMatrix();
    glTranslatef(this->getX(), this->getY(), 0.0);
    glColor3f (this->getR(), this->getG(), this->getB());
  	glBegin(GL_POLYGON);
      for(int i = 0; i < 360; i++) {
          theta = i * M_PI / 180;
          auxX = raio * cos(theta);
          auxY = raio * sin(theta);
          glVertex2f(auxX, auxY);
      }
  	glEnd();
  glPopMatrix();
}
