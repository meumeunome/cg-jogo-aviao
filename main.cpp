#include <GL/glut.h>
#include <list>
#include <string>
#include <iostream>
#include "tinyxml/tinyxml.h"
#include <math.h>
#include <time.h>
#include "linha.h"
#include "aviao.h"

void *fonte = GLUT_BITMAP_TIMES_ROMAN_24;

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
list<Aviao*> inimigosAereos;
list<Bolinha*> inimigosTerrestres;
list<Aviao*> inimigosAereosInicial;
list<Bolinha*> inimigosTerrestresInicial;

//Lista de projeteis
list<Projetil*> projeteis;
list<Projetil*> projeteisInimigos;

//Lista de bombas
list<Bomba*> bombas;

//Variaveis auxiliares
std::string pathToArena = "";
GLfloat playerSpeed;
GLfloat playerTiroSpeed;
GLfloat enemySpeed;
GLfloat enemyTiroFreq;
GLfloat enemyTiroSpeed;
GLfloat decolagemOldTime_Fixer;
GLfloat flyingOldTime_Fixer;
GLfloat shootingTime;
GLfloat turnTime;
int numTerrestres;
int destruidos = 0;
bool decolando = false;
int oldMouseX = -1;
bool win = false;

//Flags dos botoes
int flags[256];

//Funcoes auxiliares
void readConfig(char* path);
void readArena();
GLfloat* getColor(std::string cor);
int haveColision();
bool projetilForaDaArena(Projetil* projetil);
void deslocarProjeteis(GLfloat movSpeed);
void deslocarProjeteisInimigos(GLfloat movSpeed);
void deslocarBombas(GLfloat movSpeed);
void teleportar(Aviao* aviao);
void restart();
bool colide(Bolinha* aviao, Bolinha *c, int flag);
float randomFloat(float a, float b);
void cloneInitialList();
void projeteisChecaColisao();
void projeteisInimigosChecaColisao();
void exibeGameOver(GLfloat x, GLfloat y);
void exibeVitoria(GLfloat x, GLfloat y);
void exibePontuacao(GLfloat x, GLfloat y);

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
			projeteis.push_front(aviaoJogador->atirar(playerTiroSpeed));
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

	/* Desenha os projeteis em jogo */
	for(list<Projetil*>::iterator aux = projeteis.begin(); aux != projeteis.end(); ++aux){
    (*aux)->desenha();
  }
	for(list<Projetil*>::iterator aux = projeteisInimigos.begin(); aux != projeteisInimigos.end(); ++aux){
    (*aux)->desenha();
  }

	/* Desenha as bombas em jogo */
	for(list<Bomba*>::iterator aux = bombas.begin(); aux != bombas.end(); ++aux){
    (*aux)->desenha();
  }

  /* Desenha o jogador */
  aviaoJogador->desenha();

	/* Desenha os inimigos aereos */
  for(list<Aviao*>::iterator aux = inimigosAereos.begin(); aux != inimigosAereos.end(); ++aux){
    (*aux)->desenha();
  }

	if(aviaoJogador->isDead())
		exibeGameOver(bolinhaArena->getX() - bolinhaArena->getRaio()/2, bolinhaArena->getY());

	if(win)
		exibeVitoria(bolinhaArena->getX() - bolinhaArena->getRaio()/2, bolinhaArena->getY());

	exibePontuacao(bolinhaArena->getX(), bolinhaArena->getY()-bolinhaArena->getRaio());

  glutSwapBuffers();
}

void idle(){
	GLfloat currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	GLfloat difference = currentTime - flyingOldTime_Fixer;
	flyingOldTime_Fixer = currentTime;

	if(flags['r'] == 1 && ( aviaoJogador->isVoando() || decolando )){
		restart();
		return;
	}
	if(aviaoJogador->isDead() || win)
		return;

	//------------ Andar ----------------------
	GLfloat playerMovSpeed = playerSpeed * difference;
	GLfloat enemyMovSpeed = enemySpeed * difference;
	GLfloat angSpeed = (90 * M_PI)/180 * difference;
	GLfloat accel = 100 * difference;

	/*Anda com os avioes inimigos*/
	bool willShoot = false;
	bool willTurn = false;
	if( currentTime - shootingTime >= 1/enemyTiroFreq ){
		willShoot = true;
		shootingTime = glutGet(GLUT_ELAPSED_TIME)/1000.0;
	}
	if(currentTime - turnTime >= 2){
		willTurn = true;
		turnTime = glutGet(GLUT_ELAPSED_TIME)/1000.0;
	}
	for(list<Aviao*>::iterator aux = inimigosAereos.begin(); aux != inimigosAereos.end(); ++aux){
		(*aux)->andar(enemyMovSpeed);

		if(willShoot){
			projeteisInimigos.push_front((*aux)->atirar(enemyTiroSpeed));
		}

		if(willTurn){
			(*aux)->setTurning(randomFloat(-M_PI/2, M_PI/2));
		}

		if( (*aux)->getTurning() < 0 ){
			if( (*aux)->getAngulo() > (*aux)->getInitAngulo() + (*aux)->getTurning() )
				(*aux)->incrementAngulo(-angSpeed);
			else
				(*aux)->setTurning(0.0);
		}
		if( (*aux)->getTurning() > 0 ){
			if( (*aux)->getAngulo() < (*aux)->getInitAngulo() + (*aux)->getTurning() )
				(*aux)->incrementAngulo(angSpeed);
			else
				(*aux)->setTurning(0.0);
		}

		if(colide(*aux, bolinhaArena, 2)){
			(*aux)->andar(-enemyMovSpeed);
			teleportar(*aux);
		}
	}
	/*------Anda com os avioes inimigos------*/
	deslocarProjeteisInimigos(enemyMovSpeed);

	if(aviaoJogador->isVoando()){
		if(flags['a'] == 1)
			aviaoJogador->incrementAngulo(-angSpeed);
		if(flags['d'] == 1)
			aviaoJogador->incrementAngulo(angSpeed);
		if(flags['+'] == 1)
			aviaoJogador->incrementSpeed(accel);
		if(flags['-'] == 1)
			aviaoJogador->incrementSpeed(-accel);

		deslocarProjeteis(playerMovSpeed);

		deslocarBombas(playerMovSpeed);
		if(inimigosTerrestres.empty())
			win = true;

		aviaoJogador->andar(playerMovSpeed);

		projeteisChecaColisao();
		projeteisInimigosChecaColisao();

		switch(haveColision()){
			case 1:
				aviaoJogador->setDead(true);
				break;
			case 2:
				aviaoJogador->andar(-playerMovSpeed);
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
		srand(time(NULL));

    readConfig(argv[1]);
    readArena();

    glutInit(&argc, argv);
  	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
  	glutInitWindowSize (bolinhaArena->getRaio()*2, bolinhaArena->getRaio()*2);
  	glutInitWindowPosition (50, 50);
  	glutCreateWindow ("Arena");

		shootingTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
		turnTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;

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
	TiXmlElement *inimigo = NULL;

  //----- Resgata valores-----------------------------------
  //Captura do arquivo xml
  arquivoDaArena = aplicacao->FirstChildElement( "arquivoDaArena" );
  jogador = aplicacao->FirstChildElement( "jogador" );
	inimigo = aplicacao->FirstChildElement( "inimigo" );

  //Valores para se obter a arena
  nomeArena = arquivoDaArena->FirstChildElement( "nome" );
  tipoArena = arquivoDaArena->FirstChildElement( "tipo" );
  caminhoArena = arquivoDaArena->FirstChildElement( "caminho" );

  //Valores do jogador
  playerSpeed = atof(jogador->Attribute( "vel" ));
	playerTiroSpeed = atof(jogador->Attribute( "velTiro" ));

	//Valores dos inimigos
	enemySpeed = atof(inimigo->Attribute( "vel" ));
	enemyTiroFreq = atof(inimigo->Attribute( "freqTiro" ));
	enemyTiroSpeed = atof(inimigo->Attribute( "velTiro" ));

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
	Aviao *inimigo;
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
			if(cores == red){
				GLfloat angulo = randomFloat(0, 2*M_PI);
    		inimigo = new Aviao(raio, x, y, cores[0], cores[1], cores[2], id, angulo);
			}else
				bolinha = new Bolinha(raio, x, y, cores[0], cores[1], cores[2], id);
		}
    if(cores == blue){
      bolinhaArena = bolinha;
    }else {
     	if(cores == red){
				inimigosAereosInicial.push_back(inimigo);
     	}else if(cores == orange){
				inimigosTerrestresInicial.push_back(bolinha);
     	}
  	}

  }//EndFor

	for(list<Aviao*>::iterator aux = inimigosAereosInicial.begin(); aux != inimigosAereosInicial.end(); ++aux){
    (*aux)->initializeSpeed(linhaDecolagem);
  }

	cloneInitialList();
	numTerrestres = inimigosTerrestres.size();
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

/* Para colisao com aviao -> 1
 * Para colisao com a arena -> 2 */
bool colide(Bolinha* aviao, Bolinha *c, int flag){
  float dist = sqrt(pow((c->getX() - aviao->getX()), 2) + pow((c->getY() - aviao->getY()), 2));
  if(flag == 1)
    return (c->getRaio()+aviao->getRaio() > dist);
  if(flag == 2)
    return (c->getRaio() < dist);
}

int haveColision(){
  if(colide(aviaoJogador, bolinhaArena, 2))
    return 2;
  for(list<Aviao*>::iterator aux = inimigosAereos.begin(); aux != inimigosAereos.end(); ++aux){
    if(colide(aviaoJogador, *aux, 1)){
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

void deslocarProjeteisInimigos(GLfloat movSpeed){
	list<Projetil*>::iterator aux = projeteisInimigos.begin();
	while (aux != projeteisInimigos.end()) {
		Projetil* projAux = *aux;
		++aux;

		projAux->andar(movSpeed);
		if( projetilForaDaArena(projAux) ){
			projeteisInimigos.remove(projAux);
			free(projAux);
		}
	}
}

bool bombaForaDaArena(Bomba* bomba){
	float dist = sqrt(pow((bolinhaArena->getX() - bomba->getX()), 2) + pow((bolinhaArena->getY() - bomba->getY()), 2));
	return (bolinhaArena->getRaio() < dist);
}

void checaColisaoBomba(Bomba* bomba){
	list<Bolinha*>::iterator aux = inimigosTerrestres.begin();
	while (aux != inimigosTerrestres.end()) {
		Bolinha* bolinhaAux = *aux;
		++aux;
		if(colide(bomba, bolinhaAux, 1)){
			inimigosTerrestres.remove(bolinhaAux);
			free(bolinhaAux);
			destruidos++;
		}
	}
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
			checaColisaoBomba(bombAux);
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
	projeteis.clear();
	projeteisInimigos.clear();
}

void limparBombas(){
	bombas.clear();
}

void cloneInitialList(){
	for(list<Aviao*>::iterator aux = inimigosAereosInicial.begin(); aux != inimigosAereosInicial.end(); ++aux){
		GLfloat angulo = randomFloat(0, 2*M_PI);
		GLfloat raio = (*aux)->getRaio();
		GLfloat x = (*aux)->getX();
		GLfloat y = (*aux)->getY();
		GLfloat r = (*aux)->getR();
		GLfloat g = (*aux)->getG();
		GLfloat b = (*aux)->getB();
		GLint id = (*aux)->getId();
		GLfloat speed = (*aux)->getSpeed();

		Aviao *inimigo = new Aviao(raio, x, y, r, g, b, id, angulo);
		inimigo->setSpeed(speed);

		inimigosAereos.push_front(inimigo);
  }

	for(list<Bolinha*>::iterator aux = inimigosTerrestresInicial.begin(); aux != inimigosTerrestresInicial.end(); ++aux){
		GLfloat raio = (*aux)->getRaio();
		GLfloat x = (*aux)->getX();
		GLfloat y = (*aux)->getY();
		GLfloat r = (*aux)->getR();
		GLfloat g = (*aux)->getG();
		GLfloat b = (*aux)->getB();
		GLint id = (*aux)->getId();

		Bolinha *inimigo = new Bolinha(raio, x, y, r, g, b, id);

		inimigosTerrestres.push_front(inimigo);
  }
}

void restart(){
	limparProjeteis();
	limparBombas();

	aviaoJogador->reiniciar();

	inimigosAereos.clear();
	inimigosTerrestres.clear();
	cloneInitialList();
	shootingTime = glutGet(GLUT_ELAPSED_TIME)/1000.0;

	decolando = false;
	win = false;
	destruidos = 0;
}

float randomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

void projeteisChecaColisao(){
	list<Projetil*>::iterator projAux = projeteis.begin();
	while (projAux != projeteis.end()){
		Projetil *proj = *projAux;
		++projAux;

		list<Aviao*>::iterator aux = inimigosAereos.begin();
		while (aux != inimigosAereos.end()) {
			Aviao* aviaoAux = *aux;
			++aux;
			if(colide(proj, aviaoAux, 1)){
				inimigosAereos.remove(aviaoAux);
				free(aviaoAux);

				projeteis.remove(proj);
				free(proj);
			}
		}
	}
}

void projeteisInimigosChecaColisao(){
	for(list<Projetil*>::iterator projAux = projeteisInimigos.begin(); projAux != projeteisInimigos.end(); ++projAux){
		if(colide(*projAux, aviaoJogador, 1))
			aviaoJogador->setDead(true);
	}
}

void exibeGameOver(GLfloat x, GLfloat y){
	char *strTemporaria;
	char gameOver[80];
	glColor3f(1.0,1.0,1.0);
    sprintf(gameOver, "GAME OVER!");
    glRasterPos2f(x, y);
    strTemporaria = gameOver;
    while( *strTemporaria ){
        glutBitmapCharacter(fonte, *strTemporaria);
        strTemporaria++;
    }
	glColor3f(1.0,1.0,1.0);
	sprintf(gameOver, "Pressione R para jogar novamente!");
	glRasterPos2f(x, y + 25);
	strTemporaria = gameOver;
	while( *strTemporaria ){
		glutBitmapCharacter(fonte, *strTemporaria);
		strTemporaria++;
	}
}

void exibeVitoria(GLfloat x, GLfloat y){
	char *strTemporaria;
	char gameOver[80];
	glColor3f(1.0,1.0,1.0);
    sprintf(gameOver, "VOCE VENCEU!");
    glRasterPos2f(x, y);
    strTemporaria = gameOver;
    while( *strTemporaria ){
        glutBitmapCharacter(fonte, *strTemporaria);
        strTemporaria++;
    }
	glColor3f(1.0,1.0,1.0);
	sprintf(gameOver, "Pressione R para jogar novamente!");
	glRasterPos2f(x, y + 25);
	strTemporaria = gameOver;
	while( *strTemporaria ){
		glutBitmapCharacter(fonte, *strTemporaria);
		strTemporaria++;
	}
}

void exibePontuacao(GLfloat x, GLfloat y){
	char *strTemporaria;
	char pontuacao[50];
	glColor3f(1.0,1.0,1.0);
    sprintf(pontuacao, "%d de %d bases destruidas.", destruidos, numTerrestres);
    glRasterPos2f(x+50, y+25);
    strTemporaria = pontuacao;
    while( *strTemporaria ){
        glutBitmapCharacter(fonte, *strTemporaria);
        strTemporaria++;
    }
}
