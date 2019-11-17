#include "linha.h"

Linha::Linha(GLfloat& x1, GLfloat& y1, GLfloat& x2, GLfloat& y2, GLfloat& r, GLfloat& g, GLfloat& b, GLint& id){
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
    this->r = r;
    this->g = g;
    this->b = b;
    this->id = id;
}

GLfloat Linha::getX1(){
	return this->x1;
}

void Linha::setX1(GLfloat& x1){
	this->x1 = x1;
}

GLfloat Linha::getY1(){
	return this->y1;
}

void Linha::setY1(GLfloat& y1){
	this->y1 = y1;
}

GLfloat Linha::getX2(){
	return this->x2;
}

void Linha::setX2(GLfloat& x2){
	this->x2 = x2;
}

GLfloat Linha::getY2(){
	return this->y2;
}

void Linha::setY2(GLfloat& y2){
	this->y2 = y2;
}

GLfloat Linha::getR(){
	return this->r;
}

void Linha::setR(GLfloat& r){
	this->r = r;
}

GLfloat Linha::getG(){
	return this->g;
}

void Linha::setG(GLfloat& g){
	this->g = g;
}

GLfloat Linha::getB(){
	return this->b;
}

void Linha::setB(GLfloat& b){
  this->b = b;
}

GLint Linha::getId(){
  return this->id;
}

void Linha::setId(GLint& id){
  this->id = id;
}

void Linha::desenha(){

  glPushMatrix();
    glColor3f (this->r, this->g, this->b);
    glTranslatef(this->x1, this->y1, 0.0);
    glPointSize(3.0);
  	glBegin(GL_LINES);
      glVertex2d(0.0, 0.0);
      glVertex2d(this->x2 - this->x1, this->y2 - this->y1);
  	glEnd();
  glPopMatrix();
}
