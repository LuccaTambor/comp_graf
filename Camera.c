#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <math.h>
#include <string.h>

#define WINDOW_TITLE "Camera"

struct window {
    int 
        WIDTH,
        HEIGHT,
        WINDOW_HANDLE;
    unsigned int 
        FRAME_COUNTER;
} window = { 600, 600, 0, 0 };

struct camera {
    float
        position_x,
        position_y,
        position_z,
        look_x,
        look_y,
        look_z,
		angle;
    int 
        axis_x,
        axis_y,
        axis_z;
} camera = { 0, 8, 20, 0, 0, 0, 0, 0, 1, 0 };

struct vertical_movement {
	float
		sy;
	int 
		flag_y;
} vMovement = { 1, 0 };

//Cabeçalho
void INIT(int argc, char **argv);
void RenderFunction(void);
void ReshapeFunction(int Width, int Height); //Redimensiona a janela
void KeyboardFunction(unsigned char key, int x, int y);
void KeyboardSpecials(int key, int x, int y);
void TimerFunction(int Value);
void sphere();
void verticalMovement();
void flatPlane();
void movingCamera();

int main(int argc, char **argv) {
    INIT(argc, argv);
    glutMainLoop();
    exit(EXIT_SUCCESS);
}

void ReshapeFunction(int Width, int Height) {
    window.WIDTH = Width;
    window.HEIGHT = Height;
    //Atualizando o viewport
    glViewport(0, 0, window.WIDTH, window.HEIGHT); //glViewport(GLint x,GLint y,GLsizei width,GLsizei height);
}

void RenderFunction(void) {
    ++window.FRAME_COUNTER; //Atualiza contador de frames
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
    /*gluPerspective(angulo de abertura da camera, aspectratio, near, range de visão)*/
  
  	//Matriz modelview
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camera.position_x, camera.position_y, camera.position_z, camera.look_x, camera.look_y, camera.look_z, camera.axis_x, camera.axis_y, camera.axis_z); //Onde o observador ve a cena
    //Limpando os buffers 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Render de objetos
	flatPlane();

	//Salvando estado da esfera em movimento para não mover o Teapot
	glPushMatrix();
	verticalMovement();
	sphere(); //Desenho da esfera
	glPopMatrix();

	//Desenhando um Teapot para servir de referência no movimento da câmera
	glColor3f(100, 0, 0);
	glTranslatef(7, 1, 0);
	glutWireTeapot(1);
	movingCamera();

    //[Fim dos objetos renderizados]
    glutSwapBuffers(); //Alternando entre os dois buffers
    glutPostRedisplay();
}

void KeyboardFunction(unsigned char key, int x, int y) {
    switch(key) {
		case 27:
            exit(0);
        	break;
	}
}

//Atualiza contador de frames
void TimerFunction(int value) {
  if (value != 0) {
    char *frames = (char *) malloc(512 + strlen(WINDOW_TITLE));
    sprintf(frames, "%s: %d Frames Per Second @ %d x %d", WINDOW_TITLE, window.FRAME_COUNTER * 4, window.WIDTH, window.HEIGHT);
    glutSetWindowTitle(frames);
    free(frames);
  }
  window.FRAME_COUNTER = 0;
  glutTimerFunc(250, TimerFunction, 1); //msecs
}

void INIT(int argc, char **argv) {
    glutInit(&argc, argv);
    //Definindo Buffers de video
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); //Buffer de profundidade/ ? / ? 
    //Definindo Janela
    glutInitWindowSize(window.WIDTH, window.HEIGHT);
    //Criando a janela
    window.WINDOW_HANDLE = glutCreateWindow(WINDOW_TITLE);
    if(window.WINDOW_HANDLE < 1) {
        fprintf(stderr, "ERROR: Could not create a new rendering window.\n");  
        exit(EXIT_FAILURE);
    }
    //Funções de Callback
    glutReshapeFunc(ReshapeFunction);
    glutDisplayFunc(RenderFunction); //Toda a parte visual
    glutKeyboardFunc(KeyboardFunction);
  	glutTimerFunc(0, TimerFunction, 0);
    //Configuração final
    fprintf(stdout, "INFO: OpenGL Version: %s\n",glGetString(GL_VERSION));
}

void sphere() {
    glColor3ub(0, 0, 100);
    glTranslatef(0, vMovement.sy, 0);
  	glutWireSphere(1, 20, 20);
    glutPostRedisplay();
}

void verticalMovement() {
	if(vMovement.flag_y == 0) {
		vMovement.sy += 0.1;
    }
  	if(vMovement.flag_y == 1) {
		vMovement.sy -= 0.1;
    }
  	if(vMovement.sy >= 8) {
      vMovement.flag_y = 1;
    }
  	if(vMovement.sy <= 1) {
		vMovement.flag_y = 0;
    }
}

void flatPlane() {
	glColor3ub(40, 90, 90);
	glBegin(GL_QUADS);
		glVertex3f(-20.0f, 0.0f, -20.0f);
		glVertex3f(-20.0f, 0.0f,  20.0f);
		glVertex3f( 20.0f, 0.0f,  20.0f);
		glVertex3f( 20.0f, 0.0f, -20.0f);
	glEnd();
}

void movingCamera() {
	fprintf(stdout, "\n[CA: %.2f]\n[%.2f | %.2f]\n", camera.angle, cos(camera.angle), sin(camera.angle));
	//Função de movimento circular em um plano x,z
	camera.position_x = cos(camera.angle) * 25; //Raio de distância do ponto observador (25)
	camera.position_z = sin(camera.angle) * 25;
	if(camera.angle >= 364.99) { //A variavel completa os graus de um circulo trigonométrico
		camera.angle = 0;
	}
	else {
		camera.angle += 0.01;
	}
}