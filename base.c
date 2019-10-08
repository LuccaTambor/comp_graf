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
    camera_position_y = 20,
    camera_position_z = 25,
    look_position_x = 0,
    look_position_y = 0,
    look_position_z = 0;
int
    axis_x = 0,
    axis_y = 1,
    axis_z = 0,
	flag_y = 0; //Flag para movimento do torus;

int cam_flag = 0;

float
	torus_y = 0; //Posição do torus para translação

float cam_angle = 0; //Angulo da camera

void ReshapeFunction(int Width, int Height); //Redimensiona a janela
void RenderFunction(void);
void KeyboardFunction(unsigned char key, int x, int y);
void TimerFunction(int Value);
void INIT(int argc, char **argv);
void movingTorus();
void verticalMove();

void rotateCam();

//Desenho Braço
void mechanicalArm();
void drawSnowMan();



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
    //glColor3ub(100, 0, 0);
    glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
		glVertex3f(-100.0f, 0.0f, -100.0f);
		glVertex3f(-100.0f, 0.0f,  100.0f);
		glVertex3f( 100.0f, 0.0f,  100.0f);
		glVertex3f( 100.0f, 0.0f, -100.0f);
	glEnd();

    verticalMove();
    int i, j;
    for(i = -3; i < 3; i++) {
		for(int j=-3; j < 3; j++) {
			glPushMatrix();
			glTranslatef(i * 5.0, 0, j * 5.0);
			movingTorus();
            //drawSnowMan();
			glPopMatrix();
		}
    }

    //Camera
    rotateCam();
    
  

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

void movingTorus() {
    //cOLOCAR A FUNÇÃO MOVE AQUI PARA UMA ROSQUINHAS
    glColor3ub(41, 3, 3);
    glTranslatef(0,torus_y,0);
  	glutSolidTorus(0.275, 0.85, 10, 15);
    glutPostRedisplay();
}

void verticalMove() {
	/*	torus_y = 0, //Posição do torus para translação
	flag_y = 0; //Flag para movimento do torus;*/
  
    //fprintf(stdout, "%f : %d\n", torus_y, flag_y);
	if(flag_y == 0) {
		torus_y += 0.1;
    }
  	if(flag_y == 1) {
		torus_y -= 0.1;
    }
  	if(torus_y >= 8) {
      flag_y = 1;
    }
  	if(torus_y <= 0) {
		flag_y = 0;
    }
}

void rotateCam() {
    camera_position_x = cos(cam_angle) * 50;
    camera_position_z = sin(cam_angle) * 50;
    if(cam_flag == 0) {
		cam_angle += 0.02;
    }
  	if(cam_flag == 1) {
		cam_angle -= 0.02;
    }
  	if(cam_angle >= 360) {
        cam_flag = 1;
    }
  	if(cam_angle <= 0) {
        cam_flag = 0;
    }
}

void drawSnowMan() {

	glColor3f(1.0f, 1.0f, 1.0f);

// Draw Body
	glTranslatef(0.0f ,0.75f, 0.0f);
	glutSolidSphere(0.75f,20,20);

// Draw Head
	glTranslatef(0.0f, 1.0f, 0.0f);
	glutSolidSphere(0.25f,20,20);

// Draw Eyes
	glPushMatrix();
	glColor3f(0.0f,0.0f,0.0f);
	glTranslatef(0.05f, 0.10f, 0.18f);
	glutSolidSphere(0.05f,10,10);
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glutSolidSphere(0.05f,10,10);
	glPopMatrix();

// Draw Nose
	glColor3f(1.0f, 0.5f , 0.5f);
	glutSolidCone(0.08f,0.5f,10,2);
}