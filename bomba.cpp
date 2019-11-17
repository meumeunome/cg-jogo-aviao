#include "bomba.h"

Bomba::Bomba(GLfloat& raio, GLfloat& x, GLfloat& y, GLfloat& r, GLfloat& g, GLfloat& b, GLint& id, GLfloat& speed, GLfloat& angulo) : Bolinha(raio, x, y, r, g, b, id){
    this->angulo = angulo;
    this->raioInit = raio;
    this->speed = speed;
    this->initialTime = glutGet(GLUT_ELAPSED_TIME);
}

void Bomba::incrementX(GLfloat x){
	this->setX( this->getX() + (this->getSpeed() * x) );
}

void Bomba::incrementY(GLfloat y){
  this->setY( this->getY() + (this->getSpeed() * y) );
}

GLfloat Bomba::getAngulo(){
	return this->angulo;
}
GLfloat Bomba::getAnguloEmGrau(){
  return this->angulo * 180 / M_PI;
}
void Bomba::setAngulo(GLfloat& angulo){
  this->angulo = angulo;
}

void Bomba::setSpeed(GLfloat speed){
  this->speed = speed;
}
GLfloat Bomba::getSpeed(){
  return this->speed;
}

GLfloat Bomba::getRaioInit(){
  return this->raioInit;
}

void Bomba::decrementRaio(){
  GLfloat radiusSpeed = this->getRaioInit() / 2.0;

  GLfloat timeRaio = (glutGet(GLUT_ELAPSED_TIME) - this->initialTime)/1000.0;
  GLfloat moveRaio = radiusSpeed * timeRaio / 2;
  this->setRaio( this->getRaioInit() - moveRaio );
}

void Bomba::andar(GLfloat movSpeed){

  GLfloat movSpeed_x;
  GLfloat movSpeed_y;

  movSpeed_x = cos(this->angulo) * movSpeed;
  movSpeed_y = sin(this->angulo) * movSpeed;

  incrementX(movSpeed_x);
  incrementY(movSpeed_y);

  decrementRaio();
}
