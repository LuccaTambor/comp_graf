#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <math.h>
#include <string.h>

#define WINDOW_TITLE "Janela"
int 
    WIDTH = 600,
    HEIGHT = 600,
    WINDOW_HANDLE = 0;

unsigned int FrameCount = 0;

float
  camera_position_x = 0,
  camera_position_y = 0,
  camera_position_z = 25,
  look_position_x = 0,
  look_position_y = 0,
  look_position_z = 0;
int
  axis_x = 0,
  axis_y = 1,
  axis_z = 0;

float
	torus_y = 0; //Posição do torus para translação

void ReshapeFunction(int Width, int Height); //Redimensiona a janela
void RenderFunction(void);
void KeyboardFunction(unsigned char key, int x, int y);
void TimerFunction(int Value);
void INIT(int argc, char **argv);



int main(int argc, char **argv) {
    INIT(argc, argv);
    glutMainLoop();
    exit(EXIT_SUCCESS);
}

void ReshapeFunction(int Width, int Height) {
    WIDTH = Width;
    HEIGHT = Height;
    //Atualizando o viewport
    glViewport(0, 0, WIDTH, HEIGHT); //glViewport(GLint x,GLint y,GLsizei width,GLsizei height);
}

void RenderFunction(void) {
    ++FrameCount; //Atualiza contador de frames
    //Define Cor de fundo Inicial da janela rgba [0, 1]
    glClearColor(0.0f, 0.96f, 0.8f, 0.0f);

  	//Listagem de enables
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH); 
    glEnable(GL_SMOOTH);
    glEnable(GL_BLEND);
  
  	//Matriz de projeção
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
    gluPerspective(45,1,1,150); //Aqui escolhemos o modo de visão do observador (Como o observador ve a cena) 
    /*gluPerspective(angulo de abertura da camera, aspectratio, near, range de visão) gluOrtho3d()*/
  
  	//Matriz modelview
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camera_position_x,camera_position_y,camera_position_z,look_position_x,look_position_y,look_position_z,axis_x,axis_y,axis_z); //Onde o observador ve a cena
    //Limpando os buffers 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //*Definindo as matrizes*//
  
  	//Desenho
	glColor3ub(100, 0, 0);
	glutWireTeapot(3);


    glutSwapBuffers(); //Alternando entre os dois buffers
    glutPostRedisplay();//Marca 
}

void KeyboardFunction(unsigned char key, int x, int y) {
    switch(key) {
		case 27:
            exit(0);
        	break;
		case 'j':
			camera_position_y--;
			look_position_y--;
			break;
		case 'k':
			camera_position_y++;
			look_position_y++;
			break;
    }
    fprintf(stdout, "(Flag Key) -> [%c]\n", key);
}

void KeyboardSpecials(int key, int x, int y) { //Conferir dps Não funciona
    if (key == GLUT_ACTIVE_CTRL) {
      fprintf(stdout, "INFO: OpenGL Version: %s\n",glGetString(GL_VERSION));
    }

	switch(key) {
		case GLUT_KEY_LEFT:
			camera_position_x--;
			break;
		case GLUT_KEY_RIGHT:
			camera_position_x++;
			break;

		case GLUT_KEY_UP:
			camera_position_z--;
			break;
		case GLUT_KEY_DOWN:
			camera_position_z++;
			break;	
	}
  	fprintf(stdout, "Camera x: %f\n", camera_position_x);
}

void TimerFunction(int Value) {
  if (0 != Value) {
    char *TempString = (char *)
    malloc(512 + strlen(WINDOW_TITLE));

    sprintf(TempString, "%s: %d Frames Per Second @ %d x %d", WINDOW_TITLE, FrameCount * 4, WIDTH, HEIGHT);
    glutSetWindowTitle(TempString);
    free(TempString);
  }
  
  FrameCount = 0;
  glutTimerFunc(250, TimerFunction, 1);
}

void INIT(int argc, char **argv) {
    glutInit(&argc, argv);
    //Definindo Buffers de video
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); //Buffer de profundidade/ ? / ? 
    //Definindo Janela
    glutInitWindowSize(WIDTH, HEIGHT);
    //Criando a janela
    WINDOW_HANDLE = glutCreateWindow(WINDOW_TITLE);
    if(WINDOW_HANDLE < 1) {
        fprintf(stderr, "ERROR: Could not create a new rendering window.\n");  
        exit(EXIT_FAILURE);
    }
    //Funções de Callback
    glutReshapeFunc(ReshapeFunction);
    glutDisplayFunc(RenderFunction); //Toda a parte visual
    glutKeyboardFunc(KeyboardFunction);
  	glutSpecialFunc(KeyboardSpecials);  
  	glutTimerFunc(0, TimerFunction, 0);
  	
  	
    //Configuração final
    fprintf(stdout, "INFO: OpenGL Version: %s\n",glGetString(GL_VERSION));
}