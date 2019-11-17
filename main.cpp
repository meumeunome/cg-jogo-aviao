#include <GL/glut.h>
#include <list>
#include <string>
#include <iostream>
#include "tinyxml/tinyxml.h"
#include <math.h>
#include "linha.h"
#include "aviao.h"

//Cores padrões
GLfloat red[] = {1.0, 0.0, 0.0};
GLfloat green[] = {0.0, 1.0, 0.0};
GLfloat blue[] = {0.0, 0.0, 1.0};
GLfloat orange[] = {1.0, 0.7, 0.0};

//Jogador
Aviao* aviaoJogador;

//Circulo da arena
Bolinha* bolinhaArena;

//Linha da pista de decolagem
Linha* linhaDecolagem;

//Lista de inimigos
list<Bolinha*> inimigosAereos;
list<Bolinha*> inimigosTerrestres;

//Lista de projeteis
list<Projetil*> projeteis;

//Lista de bombas
list<Bomba*> bombas;

//Variaveis auxiliares
std::string pathToArena = "";
GLfloat playerSpeed;
GLfloat decolagemOldTime_Fixer;
GLfloat flyingOldTime_Fixer;
bool decolando = false;
int oldMouseX = -1;


//Flags dos botoes
int flags[256];

//Funcoes auxiliares
void readConfig(char* path);
void readArena();
GLfloat* getColor(std::string cor);
int haveColision();
bool projetilForaDaArena(Projetil* projetil);
void deslocarProjeteis(GLfloat movSpeed);
void deslocarBombas(GLfloat movSpeed);
void teleportar(Aviao* aviao);
void restart();

void keyPress(unsigned char key, int x, int y) {
	flags[key] = 1;
}

void keyUp(unsigned char key, int x, int y) {
	if(key >= 'A' && key <= 'Z')
		key = key + 32;
	flags[key] = 0;
}

void passiveMotionFunc(int x, int y){
	if(aviaoJogador->isVoando()){
		aviaoJogador->incrementAnguloCanhao( ( x - oldMouseX ) * 0.01 );
		oldMouseX = x;
	}
}

void motionFunc(int x, int y){
	if(aviaoJogador->isVoando()){
		aviaoJogador->incrementAnguloCanhao( ( x - oldMouseX ) * 0.01 );
		oldMouseX = x;
	}
}

void mouse(int button, int state, int x, int y){
	if(!aviaoJogador->isVoando() || state != 0)
		return;
	switch( button ){
		case 0:
			projeteis.push_front(aviaoJogador->atirar());
			break;
		case 2:
			bombas.push_front(aviaoJogador->bombardear());
			break;
	}
}

void display(){

  /* Limpar todos os pixels */
  glClear(GL_COLOR_BUFFER_BIT);

  /* Desenha a arena */
  bolinhaArena->desenha();

  /* Desenha a pista de decolagem */
  linhaDecolagem->desenha();

  /* Desenha os inimigos terrestres */
  for(list<Bolinha*>::iterator aux = inimigosTerrestres.begin(); aux != inimigosTerrestres.end(); ++aux){
    (*aux)->desenha();
  }

  /* Desenha os inimigos aereos */
  for(list<Bolinha*>::iterator aux = inimigosAereos.begin(); aux != inimigosAereos.end(); ++aux){
    (*aux)->desenha();
  }

	/* Desenha os projeteis em jogo */
	for(list<Projetil*>::iterator aux = projeteis.begin(); aux != projeteis.end(); ++aux){
    (*aux)->desenha();
  }

	/* Desenha as bombas em jogo */
	for(list<Bomba*>::iterator aux = bombas.begin(); aux != bombas.end(); ++aux){
    (*aux)->desenha();
  }

  /* Desenha o jogador */
  aviaoJogador->desenha();

  glutSwapBuffers();
}

void idle(){

	if(flags['r'] == 1 && ( aviaoJogador->isVoando() || decolando )){
		restart();
		return;
	}
	if(aviaoJogador->isDead())
		return;
  //------------ Andar ----------------------
	if(aviaoJogador->isVoando()){
		GLfloat currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
		GLfloat difference = currentTime - flyingOldTime_Fixer;
		flyingOldTime_Fixer = currentTime;

		GLfloat movSpeed = playerSpeed * difference;
		GLfloat angSpeed = (90 * M_PI)/180 * difference;
		GLfloat accel = 100 * difference;

		if(flags['a'] == 1)
			aviaoJogador->incrementAngulo(-angSpeed);
		if(flags['d'] == 1)
			aviaoJogador->incrementAngulo(angSpeed);
		if(flags['+'] == 1)
			aviaoJogador->incrementSpeed(accel);
		if(flags['-'] == 1)
			aviaoJogador->incrementSpeed(-accel);

		deslocarProjeteis(movSpeed);

		deslocarBombas(movSpeed);

		aviaoJogador->andar(movSpeed);
		switch(haveColision()){
			case 1:
				aviaoJogador->setDead(true);
				break;
			case 2:
				aviaoJogador->andar(-movSpeed);
				teleportar(aviaoJogador);
		}

	}else{
		if(flags['u'] == 1 && !decolando){
			decolagemOldTime_Fixer = glutGet(GLUT_ELAPSED_TIME);
			decolando = true;
		}
		if(decolando){
			GLint currentTime = glutGet(GLUT_ELAPSED_TIME) - decolagemOldTime_Fixer;

			aviaoJogador->decolar(linhaDecolagem, currentTime);

			if(currentTime > 4000){
				aviaoJogador->setVoando(true);
				decolando = false;
				flyingOldTime_Fixer = glutGet(GLUT_ELAPSED_TIME)/1000.0;
			}
		}
	}

  glutPostRedisplay();
}

void init (float corR, float corG, float corB){
  	/* selecionar cor de fundo (parametros cores R, G e B) */
  	glClearColor (corR, corG, corB, 0.0);

  	/* inicializar sistema de viz. */
  	glMatrixMode(GL_PROJECTION);
  	glLoadIdentity();
  	glOrtho(bolinhaArena->getX() - bolinhaArena->getRaio(),
            bolinhaArena->getX() + bolinhaArena->getRaio(),
            bolinhaArena->getY() + bolinhaArena->getRaio(),
            bolinhaArena->getY() - bolinhaArena->getRaio(),
            -1.0,
            1.0);
}

int main(int argc, char** argv){

    readConfig(argv[1]);
    readArena();

    glutInit(&argc, argv);
  	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
  	glutInitWindowSize (bolinhaArena->getRaio()*2, bolinhaArena->getRaio()*2);
  	glutInitWindowPosition (50, 50);
  	glutCreateWindow ("Arena");

  	init(0.0, 0.0, 0.0);
  	glutDisplayFunc(display);
    glutKeyboardFunc(keyPress);
    glutKeyboardUpFunc(keyUp);
		glutPassiveMotionFunc(passiveMotionFunc);
		glutMotionFunc(motionFunc);
		glutMouseFunc(mouse);
    glutIdleFunc(idle);
		flyingOldTime_Fixer = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
  	glutMainLoop();

    // C ANSI requer que main retorne um inteiro
    return 0;
}

void readConfig(char *path){
  TiXmlDocument config( strcat(path, "config.xml") );
  config.LoadFile();

  TiXmlElement *aplicacao = config.RootElement();

  //Encerra se não encontrar arquivo
  if(aplicacao ==  NULL){
    std::cout << "Arquivo 'config.xml' nao encontrado!\n";
    exit(1);
  }

  TiXmlElement *arquivoDaArena = NULL;
  TiXmlElement *jogador = NULL;
  TiXmlElement *nomeArena = NULL;
  TiXmlElement *tipoArena = NULL;
  TiXmlElement *caminhoArena = NULL;

  //----- Resgata valores-----------------------------------
  //Captura do arquivo xml
  arquivoDaArena = aplicacao->FirstChildElement( "arquivoDaArena" );
  jogador = aplicacao->FirstChildElement( "jogador" );

  //Valores para se obter a arena
  nomeArena = arquivoDaArena->FirstChildElement( "nome" );
  tipoArena = arquivoDaArena->FirstChildElement( "tipo" );
  caminhoArena = arquivoDaArena->FirstChildElement( "caminho" );

  //Valor da velocidade final do jogador
  playerSpeed = atof(jogador->Attribute( "vel" ));
  //----- /Resgata valores----------------------------------

  if(caminhoArena->GetText() != NULL)
    pathToArena = (std::string)caminhoArena->GetText() + '/' + (std::string)nomeArena->GetText() + '.' + (std::string)tipoArena->GetText();
  else
    pathToArena = (std::string)nomeArena->GetText() + '.' + (std::string)tipoArena->GetText();

}

void readArena(){

  TiXmlDocument config( pathToArena.c_str() );
  config.LoadFile();

  TiXmlElement *arena = config.RootElement();

  //Encerra se não encontrar arquivo
  if(arena ==  NULL){
    std::cout << "Arquivo da arena nao encontrado!\n";
    exit(1);
  }

	//Lê a "line"
  GLfloat x1, y1, x2, y2, r, g, b;
	GLint id;
  TiXmlElement *line = arena->FirstChildElement( "line" );
  x1 = atof(line->Attribute( "x1" ));
  y1 = atof(line->Attribute( "y1" ));
  x2 = atof(line->Attribute( "x2" ));
  y2 = atof(line->Attribute( "y2" ));
  id = atoi(line->Attribute( "id" ));

  r = g = b = 0.0;
  linhaDecolagem = new Linha(x1, y1, x2, y2, r, g, b, id);

  //Lê os "circles"
  TiXmlElement *circle;
  GLfloat raio, x, y, *cores;
  Bolinha *bolinha;
  for(circle = arena->FirstChildElement( "circle" ); circle != NULL; circle = circle->NextSiblingElement("circle")){
    cores = getColor(circle->Attribute( "fill" ));
    raio = atof(circle->Attribute( "r" ));
    x = atof(circle->Attribute( "cx" ));
    y = atof(circle->Attribute( "cy" ));
    id = atoi(circle->Attribute( "id" ));

		if(cores == green){
			GLfloat angulo = atan2(y2 - y1, x2 - x1);
      aviaoJogador = new Aviao(raio, x, y, cores[0], cores[1], cores[2], id, angulo);
    }else{
    	bolinha = new Bolinha(raio, x, y, cores[0], cores[1], cores[2], id);
		}
    if(cores == blue){
      bolinhaArena = bolinha;
    }else {
     	if(cores == red){
       	inimigosAereos.push_back(bolinha);
     	}else if(cores == orange){
       	inimigosTerrestres.push_back(bolinha);
     	}
  	}

  }//EndFor

}

GLfloat* getColor(std::string cor){
  if(cor.compare("red") == 0){
    return red;
  }

  if(cor.compare("green") == 0){
    return green;
  }

  if(cor.compare("blue") == 0){
    return blue;
  }

  if(cor.compare("orange") == 0){
    return orange;
  }

  std::cout << "Cor " << cor << " é inválida\n";
  exit(1);
}

/* Para colisao com inimigoAereo -> 1
 * Para colisao com a arena -> 2 */
bool colide(Bolinha *c, int flag){
  float dist = sqrt(pow((c->getX() - aviaoJogador->getX()), 2) + pow((c->getY() - aviaoJogador->getY()), 2));
  if(flag == 1)
    return (c->getRaio()+aviaoJogador->getRaio() > dist);
  if(flag == 2)
    return (c->getRaio() < dist);
}

int haveColision(){
  if(colide(bolinhaArena, 2))
    return 2;
  for(list<Bolinha*>::iterator aux = inimigosAereos.begin(); aux != inimigosAereos.end(); ++aux){
    if(colide(*aux, 1)){
      return 1;
    }
  }
  return 0;
}

bool projetilForaDaArena(Projetil* projetil){
	float dist = sqrt(pow((bolinhaArena->getX() - projetil->getX()), 2) + pow((bolinhaArena->getY() - projetil->getY()), 2));
	return (bolinhaArena->getRaio() < dist);
}

void deslocarProjeteis(GLfloat movSpeed){
	list<Projetil*>::iterator aux = projeteis.begin();
	while (aux != projeteis.end()) {
		Projetil* projAux = *aux;
		++aux;

		projAux->andar(movSpeed);
		if( projetilForaDaArena(projAux) ){
			projeteis.remove(projAux);
			free(projAux);
		}
	}
}

bool bombaForaDaArena(Bomba* bomba){
	float dist = sqrt(pow((bolinhaArena->getX() - bomba->getX()), 2) + pow((bolinhaArena->getY() - bomba->getY()), 2));
	return (bolinhaArena->getRaio() < dist);
}

void deslocarBombas(GLfloat movSpeed){
	list<Bomba*>::iterator aux = bombas.begin();
	while (aux != bombas.end()) {
		Bomba* bombAux = *aux;
		++aux;

		bombAux->andar(movSpeed);
		if( bombaForaDaArena(bombAux) ){
			bombas.remove(bombAux);
			free(bombAux);
		} else if( bombAux->getRaio() <= bombAux->getRaioInit()/2 ){
			bombas.remove(bombAux);
			free(bombAux);
		}
	}
}

void teleportar(Aviao* aviao){
	float fixX = bolinhaArena->getX();
	float fixY = bolinhaArena->getY();

	float x = aviao->getX() - fixX;
  float y = aviao->getY() - fixY;

  float alpha = atan2(y, x) * 180 / M_PI;
	float beta = aviao->getAnguloEmGrau() - 90;
  float theta = (-2 * (alpha - beta)) * M_PI / 180;

  aviao->setX( (x * cos(theta)) - (y * sin(theta)) + fixX);
  aviao->setY( (x * sin(theta)) + (y * cos(theta)) + fixY);
}

void limparProjeteis(){
	list<Projetil*>::iterator aux = projeteis.begin();
	while (aux != projeteis.end()) {
		Projetil* projAux = *aux;
		++aux;
		projeteis.remove(projAux);
		free(projAux);
	}
}

void limparBombas(){
	list<Bomba*>::iterator aux = bombas.begin();
	while (aux != bombas.end()) {
		Bomba* bombAux = *aux;
		++aux;
		bombas.remove(bombAux);
		free(bombAux);
	}
}

void restart(){
	limparProjeteis();
	limparBombas();

	aviaoJogador->reiniciar();
	decolando = false;
}
