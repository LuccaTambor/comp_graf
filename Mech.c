#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <math.h>
#include <string.h>

#define WINDOW_TITLE "Braco Mecanico"

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
        look_z;
    int 
        axis_x,
        axis_y,
        axis_z;
} camera = { 0, 7, 33, 0, 0, 0, 0, 1, 0 };

struct mech_arm {
	float
		angle_1_y, //2 angulos por esfera [y, z]
		angle_1_z,
		angle_2_y,
		angle_2_z,
		angle_3_y,
		angle_3_z,
		deltaS, //Velocidade de rotação
		paddle_top,
		paddle_down;
} mech_arm = { -30, 10, 0, 10, 0, -6, 3.5, -4, 4}; 

//Cabeçalho
void INIT(int argc, char **argv);
void RenderFunction(void);
void ReshapeFunction(int Width, int Height); //Redimensiona a janela
void KeyboardFunction(unsigned char key, int x, int y);
void KeyboardSpecials(int key, int x, int y);
void TimerFunction(int Value);
void outputMessage(int x, int y, char *string);
void mechArm(); //Braço mecânico
void flatPlane();


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

	//Render
	//Mensagem
	glColor3ub(0, 0, 0);
	outputMessage(-8, 10, "Esc: Sair");
	outputMessage(-8, 8, "Camera: Setas");
	outputMessage(-8, 6, "Arm: A1[q,w, a,s] ; A2[r,t, f,g] ; A3[u,i, j,k]");
	outputMessage(-8, 4, "Garra: [o,p]");
	//Chão 

	flatPlane();

	//Braço mecânico
	glTranslatef(-9, 0, 0);
	mechArm();
    //[Fim dos objetos renderizados]
    glutSwapBuffers(); //Alternando entre os dois buffers
    glutPostRedisplay();
}

void KeyboardFunction(unsigned char key, int x, int y) {
    switch(key) {
		case 27:
            exit(0);
        	break;
		//Movimento do braço [q,w, a,s] ; [r,t, f,g] ; [o,p]
		case 'q':
			mech_arm.angle_1_y += mech_arm.deltaS;
			break;		
		case 'w':
			mech_arm.angle_1_y -= mech_arm.deltaS;
			break;
		//Rotação em z
		case 'a':
			mech_arm.angle_1_z += mech_arm.deltaS;
			break;		
		case 's':
			mech_arm.angle_1_z -= mech_arm.deltaS;
			break;
		//Parte 2
		case 'r':
			mech_arm.angle_2_y += mech_arm.deltaS;
			break;		
		case 't':
			mech_arm.angle_2_y -= mech_arm.deltaS;
			break;
		case 'f':
			mech_arm.angle_2_z += mech_arm.deltaS;
			break;		
		case 'g':
			mech_arm.angle_2_z -= mech_arm.deltaS;
			break;
		//Parte 3  Pa vai de [-4, 4] inicialmente e vai de [-7, 7] max e [-1, 1] min
		case 'u':
			mech_arm.angle_3_y += mech_arm.deltaS;
			break;		
		case 'i':
			mech_arm.angle_3_y -= mech_arm.deltaS;
			break;
		case 'j':
			mech_arm.angle_3_z += mech_arm.deltaS;
			break;
		case 'k':
			mech_arm.angle_3_z -= mech_arm.deltaS;
			break;	
		//Translado da pá
		case 'o':
			if(mech_arm.paddle_top >= -7.0 && mech_arm.paddle_down <= 7) {
				mech_arm.paddle_top -= 0.5;
				mech_arm.paddle_down += 0.5;
			}
			break;
		case 'p':
			if(mech_arm.paddle_top <= -1.0 && mech_arm.paddle_down >= 1) {
				mech_arm.paddle_top += 0.5;
				mech_arm.paddle_down -= 0.5;
			}
			break;

	}
	fprintf(stdout, "Paddle T: %f, Paddle D: %f\n", mech_arm.paddle_top, mech_arm.paddle_down);
}

void KeyboardSpecials(int key, int x, int y) {
	switch(key) {
		case GLUT_KEY_LEFT:
			camera.position_x--;
            camera.look_x--;
			break;
		case GLUT_KEY_RIGHT:
			camera.position_x++;
            camera.look_x++;
			break;
		case GLUT_KEY_UP:
			camera.position_z--;
            camera.look_z--;
			break;
		case GLUT_KEY_DOWN:
			camera.position_z++;
            camera.look_z++;
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
	glutSpecialFunc(KeyboardSpecials);
  	glutTimerFunc(0, TimerFunction, 0);
    //Configuração final
    fprintf(stdout, "INFO: OpenGL Version: %s\n",glGetString(GL_VERSION));
}

//Escreve na tela
void outputMessage(int x, int y, char *string) {
	glPushMatrix(); //Matém a mensagem em uma posição estática no ambiente (Não testada em visão ortogonal)
	int len, i;
	glRasterPos2f(x, y); //Especifica uma posição de varredura para operações em pixel
	len = (int) strlen(string);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]); //Rederiza um caracter em bitmap
	}
	glPopMatrix();
}

//Braço mecânico
void mechArm() {
	//Esfera 1
	glColor3ub(100, 0, 0);
	glutSolidSphere(1,20,20);
	//Rtação de todo o braço
	
	glRotatef(mech_arm.angle_1_y, 0, 1.0, 0);
	glRotatef(mech_arm.angle_1_z, 0, 0, 1.0);


	glPushMatrix();//Salva estado
	//Bloco 1
	glScaled(3,1,0.5);
	glTranslatef(1.2,0,0);
	glColor3ub(50,50,50);
	glutSolidCube(2);

	glPopMatrix();//Volta ao estado anterior

	//Esfera 2
	glColor3ub(0, 100, 0);
	glTranslatef(7,0,0);
	glutSolidSphere(1,20,20);
	//Rotação de duas partições 
	glRotatef(mech_arm.angle_2_y, 0.0, 1.0, 0);
	glRotatef(mech_arm.angle_2_z, 0.0, 0, 1.0);
	
	glPushMatrix();
	//Bloco 2
	glScaled(3,1,0.5);
	glTranslatef(1.2,0,0);
	glColor3ub(50,50,50);
	glutSolidCube(2);
	glPopMatrix();

	//Esfera 3
	glColor3ub(0, 0, 100);
	glTranslatef(7,0,0);
	glutSolidSphere(1,20,20);
	//Rotação da última partição
	glRotatef(mech_arm.angle_3_y, 0.0, 1.0, 0.0);
	glRotatef(mech_arm.angle_3_z, 0.0, 0.0, 1.0);
	
	glPushMatrix();
	//Pá 1
	glScaled(1.5, 0.1, 0.5);
	glTranslatef(1.2, mech_arm.paddle_top, 0);
	glColor3ub(100, 10, 100);
	glutSolidCube(2);
	glPopMatrix();
	//Pá 2
	glPushMatrix();
	glScaled(1.5, 0.1, 0.5);
	glTranslatef(1.2, mech_arm.paddle_down, 0);
	glColor3ub(100, 10, 100);
	glutSolidCube(2);
	glPopMatrix();

	glutPostRedisplay();
}

void flatPlane() {
	glColor3ub(40, 90, 90);
	glBegin(GL_QUADS);
		glVertex3f(-20.0f, -1.0f, -20.0f);
		glVertex3f(-20.0f, -1.0f,  20.0f);
		glVertex3f( 20.0f, -1.0f,  20.0f);
		glVertex3f( 20.0f, -1.0f, -20.0f);
	glEnd();
}