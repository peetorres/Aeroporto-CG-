#include <GL/glut.h>

#include "BMPLoader.h"
#include "BMPLoader.cpp"
#include <cmath>
#include <iostream>

using namespace std;

GLUquadric *quad;
GLfloat angle = 0, fAspect;
GLfloat movx = 0, movy = 0, movz = 0;
int plane = 0;
float velocidade = 0;

void gravidade(int x){
  if(movy>0){
    if(velocidade==0)
        movy-=0.001;
  }
}

void inercia(int x){
  if(movy>0)
    movz-=0.005;
}


void atrito(int x){
  if(velocidade>0)
    velocidade-=0.0001;
  else
    velocidade=0;
}

void fisicas(){
  glutTimerFunc(100,gravidade,10);
  glutTimerFunc(100,inercia,10);
  glutTimerFunc(100,atrito,10);
}

void DefineIluminacao (void){
	GLfloat luzAmbiente[4]={0.2,0.2,0.2,1.0};
	GLfloat luzDifusa[4]={0.7,0.7,0.7,1.0};	   // "cor"
	GLfloat luzEspecular[4]={1.0, 1.0, 1.0, 1.0};// "brilho"
	GLfloat posicaoLuz[4]={0.0, 50.0, 50.0, 1.0};

	// Capacidade de brilho do material
	GLfloat especularidade[4]={1.0,1.0,1.0,1.0};
	GLint especMaterial = 60;

	// Define a refletância do material
	glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
	// Define a concentração do brilho
	glMateriali(GL_FRONT,GL_SHININESS,especMaterial);

	// Ativa o uso da luz ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	// Define os parâmetros da luz de número 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );
}

void GerenciaMouse (int button, int state, int x, int y){
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        if(angle >= 5){
          angle -= 5;
        }
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
      if(angle <= 130){
          angle += 5;
      }
    }
    glutPostRedisplay();
}

void tecla(unsigned char tecla, int x, int y){
    switch (tecla){
      case 's':
        movz += 0.1;
        break;
      case 'w':
        if(plane==1){
          if(velocidade<0.07)
            velocidade+=0.008;
          movz-=velocidade;
        }
        else{

          if(velocidade<0.10)
            velocidade+=0.006;
          else if(velocidade<0.15){
            velocidade+=0.007;
            movy += 0.09;
          }
          else if(velocidade>0.2)
            velocidade=0.15;

          movz-=velocidade;


        }
        /*if(movz <= -5 && plane == 0)
          movy += 0.1;*/
        break;
      case 'v':
        movy += 0.1;
        break;
      case 'b':
        movy -= 0.1;
        break;
      case 'a': // alçar voo
        angle += 0.1;
        movz -= 0.1;
        break;
      case 'd': // alçar voo
        angle -= 0.1;
        movz -= 0.1;
        break;
      case 'q':
        if (plane == 0)
          plane = 1;
        else if(plane == 1)
          plane = 0;
        break;
      case 'F':
        exit(0);
        break;
      case 'f':
        exit(0);
        break;
    }
}


void DesenhaBola(void){
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslatef(0.0f, -1.0f, -7.0f);

  glEnable( GL_TEXTURE_2D );
  glBindTexture(GL_TEXTURE_2D,2);

  glRotatef(angle, 0.0f, 1.0f, 0.0f);
  //glRotatef(-100, 0.0f, 1.0f, 0.0f);
  glTranslatef(movx, movy, movz);
  // Tamanho da esfera
  glScalef(0.2,0.2,0.2);

  gluQuadricTexture(quad, 2);
  //position
  glTranslatef(0,0,0);
  gluSphere(quad, 2, 20, 20);

  glPopMatrix();
}

void DesenhaAviao(void)
{
    glMatrixMode(GL_MODELVIEW);

	  //DefineIluminacao();
    glPushMatrix();
      glEnable( GL_TEXTURE_2D );
      glBindTexture(GL_TEXTURE_2D,2);
      gluQuadricTexture(quad, 2);
  	  //glColor3f(0.0f, 0.0f, 1.0f);

      glScalef(0.1,0.1,0.1);
      glTranslatef(movx, movy, movz);
      glRotatef(angle, 0.0f, 1.0f, 0.0f);

  	  //cone referente ao corpo do avião
      glPushMatrix();
  	   glutSolidCone(1.0, 9.0, 20, 4);
      glPopMatrix();

      //cone referente ao bico do avião
     	glPushMatrix();
      	glRotatef(180, 1,0,0);
        glutSolidCone(1, 3, 50, 20);
      glPopMatrix();

      //cone referente a uma asa
     	glPushMatrix();
      	glRotatef(70, 0,1,0);
        glutSolidCone(1,7, 50, 20);
      glPopMatrix();

      //cone referente a outra asa
      glPushMatrix();
      	glRotatef(-70, 0,1,0);
    	  glutSolidCone(1,7, 50, 20);
      glPopMatrix();

      //cone referente a parte da cauda de um lado
     	glPushMatrix();
    	  glTranslatef(0, 0, 7);
      	glRotatef(-70, 0,1,0);
    	  glutSolidCone(0.3, 3 , 60, 1);
      glPopMatrix();

      //cone referente a parte da cauda do outro lado
     	glPushMatrix();
    	  glTranslatef(0, 0, 7);
      	glRotatef(70, 0,1,0);
    	  glutSolidCone(0.3, 3 , 60, 1);
      glPopMatrix();

      //cone referente a parte de cima da cauda
     	glPushMatrix();
    	  glTranslatef(0, 0, 7);
      	glRotatef(-70, 1,0,0);
    	  glutSolidCone(0.3, 2 , 60, 1);
      glPopMatrix();
    glPopMatrix();
}

void DesenhaCenario(void){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(angle, movy, movz+5, movx, movy, movz,0, 1, 0);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -7.0f);
    glEnable( GL_TEXTURE_2D );
    glBindTexture(GL_TEXTURE_2D,1);

    //glRotatef(angle, 1.0f, 0.0f, 0.0f);
    //glTranslatef(0.0, movx, movz);
    glScalef(0.2,0.2,0.2);

    //gluQuadricTexture(quad, 1);
    //glEnable( GL_TEXTURE_2D );
    //glBindTexture(GL_TEXTURE_2D,1);
    //gluSphere(quad, 2, 20, 20);

    // MAR
    glEnable( GL_TEXTURE_2D );
    glBindTexture(GL_TEXTURE_2D,3);

    glBegin(GL_POLYGON);
        glTexCoord2f(0,0);glVertex3f(-10000.0, -10.0, 0.0);
        glTexCoord2f(1,0);glVertex3f(-10000.0, -9.0, -10000.0);
        glTexCoord2f(1,1);glVertex3f(10000.0, -9.0, -10000.0);
        glTexCoord2f(0,1);glVertex3f(10000.0, -10.0, 0.0);
    glEnd();

    // PISTA DE DECOLAGEM
    glEnable( GL_TEXTURE_2D );
    glBindTexture(GL_TEXTURE_2D,1);

    glBegin(GL_POLYGON);
        glTexCoord2f(0,0);glVertex3f(-20.0, -10.0, 0.0);
        glTexCoord2f(1,0);glVertex3f(-20.0, -9.0, -1000.0);
        glTexCoord2f(1,1);glVertex3f(20.0, -9.0, -1000.0);
        glTexCoord2f(0,1);glVertex3f(20.0, -10.0, 0.0);
    glEnd();

    glPopMatrix();

    /*glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
      glVertex3f(-100.0, -10.0, 0.0);
      glVertex3f(-100.0, -9.0, -1000.0);
      glVertex3f(100.0, -9.0, -1000.0);
      glVertex3f(100.0, -10.0, 0.0);
    glEnd();*/
}

void Simulator(void){
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     //cout << velocidade << endl;

     DesenhaCenario();
     DesenhaAviao();
     //DesenhaBola();

     fisicas();
     //Desenha();
     glutSwapBuffers();

}

void init(void)
{
    /*  define a cor de background da janela       */
    glClearColor (1.0, 1.0, 1.0, 0.0);

    glViewport(0, 0, 640, 640);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30, 640.0/640.0, 1.0, 200.0);

    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    quad = gluNewQuadric();

    // Textura da pista
    BMPClass bmp;
    BMPLoad("texture/pista.bmp",bmp);
    glBindTexture(GL_TEXTURE_2D, 1);  // numero 1 escolhido para identificar a textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST OU GL_LINEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,bmp.width,bmp.height,0,GL_RGB,GL_UNSIGNED_BYTE,bmp.bytes);


    // Textura da bola
    BMPClass bmp2;
    BMPLoad("texture/map.bmp",bmp2);
    glBindTexture(GL_TEXTURE_2D, 2);  // numero 1 escolhido para identificar a textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST OU GL_LINEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,bmp2.width,bmp2.height,0,GL_RGB,GL_UNSIGNED_BYTE,bmp2.bytes);

    // Textura do mar
    BMPClass bmp3;
    BMPLoad("texture/mar.bmp",bmp3);
    glBindTexture(GL_TEXTURE_2D, 3);  // numero 1 escolhido para identificar a textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST OU GL_LINEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,bmp3.width,bmp3.height,0,GL_RGB,GL_UNSIGNED_BYTE,bmp3.bytes);
}

class cameraPosition{
public:
  int x,y,z;
};


class objectPosition{
public:
  int x,y,z;
};

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640,640);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Aeroporto CG");
    init();

    glutDisplayFunc(Simulator);
    glutIdleFunc(Simulator);

    glutKeyboardFunc(tecla);
    glutMouseFunc(GerenciaMouse);

    glutMainLoop();
    return 0;
}
