#include "aviao.h"

Aviao::Aviao(GLfloat& raio, GLfloat& x, GLfloat& y, GLfloat& r, GLfloat& g, GLfloat& b, GLint& id, GLfloat& angulo) : Bolinha(raio, x, y, r, g, b, id){
    this->angulo = angulo;
    this->initAngulo = angulo;
    this->voando = false;
    this->initX = x;
    this->initY = y;
    this->initRaio = raio;
    this->radiusTime = 0.0;
    this->speed = 0;
    this->anguloHelice = 0;
    this->anguloCanhao = 0;
    this->dead = false;
    this->turning = 0.0;
}

void Aviao::incrementX(GLfloat x){
	this->setX( this->getX() + (this->getSpeed() * x) );
}

void Aviao::incrementY(GLfloat y){
  this->setY( this->getY() + (this->getSpeed() * y) );
}

void Aviao::setDead(bool state){
  this->dead = state;
}
bool Aviao::isDead(){
  return this->dead;
}

void Aviao::setTurning(GLfloat state){
  this->turning = state;
}
GLfloat Aviao::getTurning(){
  return this->turning;
}

GLfloat Aviao::getAngulo(){
	return this->angulo;
}
GLfloat Aviao::getAnguloEmGrau(){
  return this->angulo * 180 / M_PI;
}
void Aviao::setAngulo(GLfloat& angulo){
  this->angulo = angulo;
}
void Aviao::incrementAngulo(GLfloat angulo){
  this->angulo += angulo;
}

GLfloat Aviao::getInitAngulo(){
  return this->initAngulo;
}
void Aviao::setInitAngulo(GLfloat& angulo){
  this->angulo = initAngulo;
}

bool Aviao::isVoando(){
  return this->voando;
}
void Aviao::setVoando(bool flag){
  this->voando = flag;
}

void Aviao::setInitX(GLfloat x){
  this->initX = x;
}
GLfloat Aviao::getInitX(){
  return this->initX;
}

void Aviao::setInitY(GLfloat y){
  this->initY = y;
}
GLfloat Aviao::getInitY(){
  return this->initY;
}

void Aviao::setInitRaio(GLfloat raio){
  this->initRaio = raio;
}
GLfloat Aviao::getInitRaio(){
  return this->initRaio;
}

void Aviao::setRadiusTime(GLfloat time){
  this->radiusTime = time;
}
GLfloat Aviao::getRadiusTime(){
  return this->radiusTime;
}

void Aviao::setSpeed(GLfloat speed){
  this->speed = speed;
}
GLfloat Aviao::getSpeed(){
  return this->speed;
}
void Aviao::incrementSpeed(GLfloat accel){
  if(this->speed + accel < 0)
    this->speed = 0;
  else
    this->speed += accel;
}

GLfloat Aviao::getAnguloHelice(){
  return this->anguloHelice;
}
GLfloat Aviao::getAnguloHeliceEmGrau(){
  return this->anguloHelice * 180 / M_PI;
}
void Aviao::setAnguloHelice(GLfloat angulo){
  this->anguloHelice = angulo;
}
void Aviao::incrementAnguloHelice(GLfloat angulo){
  this->anguloHelice += angulo;
}

GLfloat Aviao::getAnguloCanhao(){
  return this->anguloCanhao;
}
GLfloat Aviao::getAnguloCanhaoEmGrau(){
  return this->anguloCanhao * 180 / M_PI;
}
void Aviao::setAnguloCanhao(GLfloat angulo){
  this->anguloCanhao = angulo;
}
void Aviao::incrementAnguloCanhao(GLfloat angulo){
  GLfloat anguloFinal = this->anguloCanhao + angulo;
  if(anguloFinal <= M_PI/4 && anguloFinal >= -M_PI/4)
    this->anguloCanhao = anguloFinal;
}

void Aviao::reiniciar(){
  this->setRaio(initRaio);
  this->setX(initX);
  this->setY(initY);
  this->angulo = initAngulo;
  this->speed = 0.0;
  this->voando = false;
  this->dead = false;
  this->anguloCanhao = 0.0;
  this->anguloHelice = 0.0;
}

void Aviao::initializeSpeed(Linha* linha){
  GLfloat totalTime = 4.0;

  GLfloat dx = linha->getX2() - linha->getX1();
  GLfloat dy = linha->getY2() - linha->getY1();

  GLfloat accelX = 2*dx / pow(totalTime,2);
  GLfloat accelY = 2*dy / pow(totalTime,2);

  this->setSpeed(sqrt(pow(accelX, 2) + pow(accelY, 2)) * totalTime);
}

void Aviao::decolar(Linha* linha, GLint& currentTime){
  GLfloat totalTime = 4.0;

  GLfloat dx = linha->getX2() - linha->getX1();
  GLfloat dy = linha->getY2() - linha->getY1();

  GLfloat accelX = 2*dx / pow(totalTime,2);
  GLfloat accelY = 2*dy / pow(totalTime,2);

  GLfloat dist = sqrt(pow(dx, 2) + pow(dy, 2));
  this->setSpeed(sqrt(pow(accelX, 2) + pow(accelY, 2)) * totalTime);

  GLfloat accel = 2 * dist / pow(totalTime, 2);
  GLfloat pista_getToMiddleTime = sqrt(dist / accel);

  GLfloat radiusSpeed = this->getInitRaio() / (totalTime - pista_getToMiddleTime);

  //INICIAR VOO
  GLfloat moveX = (accelX * pow(currentTime/1000.0, 2)) / 2;
  this->setX( this->getInitX() + moveX );
  GLfloat moveY = (accelY * pow(currentTime/1000.0, 2)) / 2;
  this->setY( this->getInitY() + moveY );

  //Se o tempo de voo for maior que o tempo em que o aviao está no meio da pista, começa a crescer o raio
  if(currentTime/1000.0 >= pista_getToMiddleTime){
    GLfloat timeRaio = (currentTime - this->getRadiusTime()) / 1000.0;
    GLfloat moveRaio = radiusSpeed * timeRaio;
    this->setRaio( this->getInitRaio() + moveRaio );
  }else{
    this->setRadiusTime(currentTime);
  }

}

void Aviao::andar(GLfloat movSpeed){

  GLfloat movSpeed_x;
  GLfloat movSpeed_y;

  movSpeed_x = cos(this->angulo) * movSpeed;
  movSpeed_y = sin(this->angulo) * movSpeed;

  incrementX(movSpeed_x);
  incrementY(movSpeed_y);
}

Projetil* Aviao::atirar(GLfloat tiroSpeed){
  GLfloat raio = this->getRaio();
  GLfloat raioProjetil = raio/6;
  GLfloat angulo = this->getAngulo() + this->getAnguloCanhao();
  GLfloat x_canhao = this->getX() + raio*cos(this->getAngulo()) + raio/2*cos(angulo);
  GLfloat y_canhao = this->getY() + raio*sin(this->getAngulo()) + raio/2*sin(angulo);
  GLfloat r = 0.8;
  GLfloat g = 0.8;
  GLfloat b = 0.8;
  GLint id = this->getId();
  GLfloat speed = this->getSpeed()*tiroSpeed;

  Projetil* projetil = new Projetil(raioProjetil, x_canhao, y_canhao, r, g, b, id, speed, angulo);

  return projetil;
}

Bomba* Aviao::bombardear(){
  GLfloat raioBomba = this->getRaio()*2/5;
  GLfloat angulo = this->getAngulo();
  GLfloat x = this->getX();
  GLfloat y = this->getY();
  GLfloat r = 0.5;
  GLfloat g = 0.5;
  GLfloat b = 0.5;
  GLint id = this->getId();
  GLfloat speed = this->getSpeed();

  Bomba* bomba = new Bomba(raioBomba, x, y, r, g, b, id, speed, angulo);

  return bomba;
}

void Aviao::desenha(){
  glPushMatrix();
    glTranslatef(this->getX(), this->getY(), 0.0);
    glRotatef(-90, 0.0, 0.0, 1.0);
    glRotatef(this->getAnguloEmGrau(), 0.0, 0.0, 1.0);

    // this->desenhaBolinhaModelo();
    this->desenhaAviaoModelo();
    this->desenhaCabine();
    this->desenhaAsas();
    this->desenhaTraseira();
    this->desenhaHelices();
    this->desenhaCanhao();

  glPopMatrix();
}

//FUNÇÕES DE DESENHO
void Aviao::desenhaBolinhaModelo(){
  float theta, auxX, auxY;
  GLfloat raio = this->getRaio();
  glColor3f (this->getR(), this->getG(), this->getB());
	glBegin(GL_LINE_LOOP);
    for(int i = 0; i < 360; i++) {
        theta = i * M_PI / 180;
        auxX = raio * cos(theta);
        auxY = raio * sin(theta);
        glVertex2f(auxX, auxY);
    }
	glEnd();
}

void Aviao::desenhaAviaoModelo(){

  glPushMatrix();
    float x,y,z;
    GLfloat raioMenor = this->getRaio()/3;
    GLfloat raioMaior = this->getRaio();

    glColor3f (this->getR(), this->getG(), this->getB());
  	glBegin(GL_POLYGON);
      for(int t = 0; t <= 360; t++){
        x = raioMenor * sin(t);
        y = raioMaior * cos(t);
        z = 0;
        glVertex3f(x,y,z);
      }
    glEnd();
  glPopMatrix();
}

void Aviao::desenhaCabine(){
  float x,y,z;
  GLfloat raioMenor = this->getRaio()/6;
  GLfloat raioMaior = this->getRaio()/2;

  glPushMatrix();
  glTranslatef(0.0, raioMaior/2, 0.0);
    glColor3f (0.0, 0.0, 0.0);
  	glBegin(GL_POLYGON);
      for(int t = 0; t <= 360; t++){
        x = raioMenor * sin(t);
        y = raioMaior * cos(t);
        z = 0;
        glVertex3f(x,y,z);
      }
    glEnd();
  glPopMatrix();

  glPushMatrix();
    glColor3f (this->getR(), this->getG(), this->getB());
  	glBegin(GL_POLYGON);
      for(int t = 0; t <= 360; t++){
        x = raioMenor * sin(t);
        y = raioMaior * cos(t);
        z = 0;
        glVertex3f(x,y,z);
      }
    glEnd();
  glPopMatrix();
}

void Aviao::desenhaAsas(){
  GLfloat raioAviao = this->getRaio();
  GLfloat posFinalAsaEmY = raioAviao;
  GLfloat espessuraMaior = raioAviao/2;
  GLfloat espessuraMenor = raioAviao/4;
  GLfloat posInicialAsaEmX = -raioAviao/4;
  GLfloat posInicialAsaEmY = raioAviao/4;

  glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
		glVertex3f(posInicialAsaEmY, posInicialAsaEmX, 0.0);
		glVertex3f(posInicialAsaEmY, posInicialAsaEmX + espessuraMaior, 0.0);
		glVertex3f(posFinalAsaEmY, posInicialAsaEmX + espessuraMaior, 0.0);
		glVertex3f(posFinalAsaEmY, posInicialAsaEmX + espessuraMenor, 0.0);
	glEnd();
	glBegin(GL_POLYGON);
		glVertex3f(-posInicialAsaEmY, posInicialAsaEmX, 0.0);
		glVertex3f(-posInicialAsaEmY, posInicialAsaEmX + espessuraMaior, 0.0);
		glVertex3f(-posFinalAsaEmY, posInicialAsaEmX + espessuraMaior, 0.0);
		glVertex3f(-posFinalAsaEmY, posInicialAsaEmX + espessuraMenor, 0.0);
	glEnd();
}

void Aviao::desenhaTraseira(){
  GLfloat raioAviao = this->getRaio();
  GLfloat posFinalAsaEmY = raioAviao*2/3;
  GLfloat espessuraMaior = raioAviao/3;
  GLfloat espessuraMenor = raioAviao/6;
  GLfloat posInicialAsaEmX = -raioAviao/6;
  GLfloat posInicialAsaEmY = raioAviao/6;

  glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(0.0, -raioAviao*3/4, 0.0);
  	glBegin(GL_POLYGON);
  		glVertex3f(posInicialAsaEmY, posInicialAsaEmX, 0.0);
  		glVertex3f(posInicialAsaEmY, posInicialAsaEmX + espessuraMaior, 0.0);
  		glVertex3f(posFinalAsaEmY, posInicialAsaEmX + espessuraMaior, 0.0);
  		glVertex3f(posFinalAsaEmY, posInicialAsaEmX + espessuraMenor, 0.0);
  	glEnd();
  	glBegin(GL_POLYGON);
  		glVertex3f(-posInicialAsaEmY, posInicialAsaEmX, 0.0);
  		glVertex3f(-posInicialAsaEmY, posInicialAsaEmX + espessuraMaior, 0.0);
  		glVertex3f(-posFinalAsaEmY, posInicialAsaEmX + espessuraMaior, 0.0);
  		glVertex3f(-posFinalAsaEmY, posInicialAsaEmX + espessuraMenor, 0.0);
  	glEnd();
  glPopMatrix();
}

void Aviao::desenhaHelices(){
  GLfloat raioAviao = this->getRaio();
  GLfloat posInicialAsaEmY = raioAviao/4;
  GLfloat posInicialAsaEmX = raioAviao*5/8;
  GLfloat tamanhoHorizontal = raioAviao*2/6;
  GLfloat tamanhoVertical = raioAviao/4;
  GLfloat tamanhoBarra = raioAviao/4;
  GLfloat espessuraBarra = raioAviao*1/6;
  GLfloat angulo = this->getAnguloHelice();
  incrementAnguloHelice(0.05*this->getSpeed());

  glPushMatrix();
  glTranslatef(posInicialAsaEmX, posInicialAsaEmY, 0.0);
    glColor3f(0.3, 0.3, 0.3);
    glBegin(GL_POLYGON);
      glVertex3f(espessuraBarra/2, 0.0, 0.0);
      glVertex3f(espessuraBarra/2, tamanhoBarra, 0.0);
      glVertex3f(-espessuraBarra/2, tamanhoBarra, 0.0);
      glVertex3f(-espessuraBarra/2, 0.0, 0.0);
    glEnd();
  glTranslatef(0.0, tamanhoBarra, 0.0);
  glRotatef(angulo, 0.0, 1.0, 0.0);
    glColor3f(1.0, 1.0, 0.0);
  	glBegin(GL_TRIANGLES);
  		glVertex3f(0.0, 0.0, 0.0);
  		glVertex3f(-tamanhoVertical, -tamanhoHorizontal/2, 0.0);
  		glVertex3f(-tamanhoVertical, tamanhoHorizontal/2, 0.0);
  	glEnd();
    glBegin(GL_TRIANGLES);
  		glVertex3f(0.0, 0.0, 0.0);
  		glVertex3f(tamanhoVertical, -tamanhoHorizontal/2, 0.0);
  		glVertex3f(tamanhoVertical, tamanhoHorizontal/2, 0.0);
  	glEnd();
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-posInicialAsaEmX, posInicialAsaEmY, 0.0);
    glColor3f(0.3, 0.3, 0.3);
    glBegin(GL_POLYGON);
      glVertex3f(espessuraBarra/2, 0.0, 0.0);
      glVertex3f(espessuraBarra/2, tamanhoBarra, 0.0);
      glVertex3f(-espessuraBarra/2, tamanhoBarra, 0.0);
      glVertex3f(-espessuraBarra/2, 0.0, 0.0);
    glEnd();
  glTranslatef(0.0, tamanhoBarra, 0.0);
  glRotatef(-angulo, 0.0, 1.0, 0.0);
    glColor3f(1.0, 1.0, 0.0);
  	glBegin(GL_TRIANGLES);
  		glVertex3f(0.0, 0.0, 0.0);
  		glVertex3f(-tamanhoVertical, -tamanhoHorizontal/2, 0.0);
  		glVertex3f(-tamanhoVertical, tamanhoHorizontal/2, 0.0);
  	glEnd();
    glBegin(GL_TRIANGLES);
  		glVertex3f(0.0, 0.0, 0.0);
  		glVertex3f(tamanhoVertical, -tamanhoHorizontal/2, 0.0);
  		glVertex3f(tamanhoVertical, tamanhoHorizontal/2, 0.0);
  	glEnd();
  glPopMatrix();
}

void Aviao::desenhaCanhao(){
  GLfloat raioAviao = this->getRaio();
  GLfloat espessuraCanhao = raioAviao*1/5;
  GLfloat tamanhoCanhao = raioAviao*1/2;
  GLfloat angulo = this->getAnguloCanhaoEmGrau();

  glPushMatrix();
  glTranslatef(0.0, raioAviao, 0.0);
  glRotatef(angulo, 0.0, 0.0, 1.0);
    glColor3f(0.0, 0.0, 0.0);
  	glBegin(GL_POLYGON);
  		glVertex3f(espessuraCanhao/2, 0.0, 0.0);
  		glVertex3f(espessuraCanhao/2, tamanhoCanhao, 0.0);
  		glVertex3f(-espessuraCanhao/2, tamanhoCanhao, 0.0);
  		glVertex3f(-espessuraCanhao/2, 0.0, 0.0);
  	glEnd();
  glPopMatrix();
}
