#include "projetil.h"

Projetil::Projetil(GLfloat& raio, GLfloat& x, GLfloat& y, GLfloat& r, GLfloat& g, GLfloat& b, GLint& id, GLfloat& speed, GLfloat& angulo) : Bolinha(raio, x, y, r, g, b, id){
    this->angulo = angulo;
    this->speed = speed;
}

void Projetil::incrementX(GLfloat x){
	this->setX( this->getX() + (this->getSpeed() * x) );
}

void Projetil::incrementY(GLfloat y){
  this->setY( this->getY() + (this->getSpeed() * y) );
}

GLfloat Projetil::getAngulo(){
	return this->angulo;
}
GLfloat Projetil::getAnguloEmGrau(){
  return this->angulo * 180 / M_PI;
}
void Projetil::setAngulo(GLfloat& angulo){
  this->angulo = angulo;
}

void Projetil::setSpeed(GLfloat speed){
  this->speed = speed;
}
GLfloat Projetil::getSpeed(){
  return this->speed;
}

void Projetil::andar(GLfloat movSpeed){

  GLfloat movSpeed_x;
  GLfloat movSpeed_y;

  movSpeed_x = cos(this->angulo) * movSpeed;
  movSpeed_y = sin(this->angulo) * movSpeed;

  incrementX(movSpeed_x);
  incrementY(movSpeed_y);
}
