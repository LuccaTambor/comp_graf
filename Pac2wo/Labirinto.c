#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <math.h>
#include <string.h>

#define WINDOW_TITLE "Janela"
float ang = 0;
struct window {
    int 
        WIDTH,
        HEIGHT,
        WINDOW_HANDLE;
    unsigned int 
        FrameCount;
} window = {600, 600, 0, 0};


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
} camera = {0, 5, 10, 0, 0, 0, 0, 1, 0};

int click = 0;

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

void DESENHA(int click) {
	if(click == 0) {
		glColor3ub(100, 0, 0);
		glutWireCube(5);
		glutPostRedisplay();
	}
	else if(click == 1) {
		glColor3ub(100, 0, 0);
    	glutSolidCube(5);
    	glutPostRedisplay();
	}
}

void crack() {
	int i;
	for(i = 0; i < 30; i+=5) {
		glPushMatrix();	
		glTranslatef(i, 0, 0);
		DESENHA(click);
		glPopMatrix();
	}
}

int m[16][16] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1,
};
int cx = 0, cz = 0;
void cubo() {
	glColor3f(100, 0, 100);
	glutSolidCube(1);
}
void cubo2() {
	glColor3f(20, 30, 100);
	glutSolidCube(1);
}

//Cabeçalho
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
    window.WIDTH = Width;
    window.HEIGHT = Height;
    //Atualizando o viewport
    glViewport(0, 0, window.WIDTH, window.HEIGHT); //glViewport(GLint x,GLint y,GLsizei width,GLsizei height);
}



void RenderFunction(void) {
    ++window.FrameCount; //Atualiza contador de frames
    //Define Cor de fundo Inicial da janela rgba [0, 1]
    //glClearColor(0.0f, 0.96f, 0.8f, 0.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  	//Listagem de enables
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH); 
    glEnable(GL_SMOOTH);
    glEnable(GL_BLEND);
  
  	//Matriz de projeção
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
    //gluPerspective(45,1,1,150); //Aqui escolhemos o modo de visão do observador (Como o observador ve a cena) 
    /*gluPerspective(angulo de abertura da camera, aspectratio, near, range de visão) gluOrtho3d()*/
  
  	//Matriz modelview
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	glOrtho( -20, 20, -20, 20, -20, 20); //Em perspectiva os eixos x e z se alteram
    gluLookAt(camera.position_x, camera.position_y, camera.position_z, camera.look_x, camera.look_y, camera.look_z, camera.axis_x, camera.axis_y, camera.axis_z); //Onde o observador ve a cena
    //Limpando os buffers 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  	//Desenho

	//glutWireTeapot(3);

    //crack();
	int i, j;
    for(i = 0; i < 16; i++) {
		for(int j = 0; j < 16; j++) {
			if(m[i][j] == 1) {
				glPushMatrix();
				glTranslatef(i+1, j+1, 0);
				cubo();
				glPopMatrix();
			}
            //drawSnowMan();
		}
    }
	glPushMatrix();
	glColor3f(20, 30, 30);
	glTranslatef(cx, cz, 0);
	glutSolidCube(1);
	glPopMatrix();



    //[Fim dos desenhos]
    glutSwapBuffers(); //Alternando entre os dois buffers
    glutPostRedisplay();//Marca 
}

void KeyboardFunction(unsigned char key, int x, int y) {
    switch(key) {
		case 27:
            exit(0);
        	break;
		case 'a':
			camera.position_x--;
            camera.look_x--;
			break;
		case 'd':
			camera.position_x++;
            camera.look_x++;
			break;

		case 'w':
			camera.position_z--;
            camera.look_z--;
			break;
		case 's':
			camera.position_z++;
            camera.look_z++;
			break;	

        //Girando em torno do proprio eixo
        case 'q':
			fprintf(stdout, "Click[%i]\n", click);
			click = 0;
			//camera.look_x += cos(ang);
			//camera.look_z += sin(ang);
			//ang += 0.1;
            break;
		case 'e':
			click = 1;
			//camera.look_x -= cos(ang);
			//camera.look_z -= sin(ang);
			//ang -= 0.1;

	}
  	fprintf(stdout, "[X, Y, Z]: [%.2f, %.2f, %.2f]\n", camera.position_x, camera.position_y, camera.position_z);
}

void KeyboardSpecials(int key, int x, int y) { //Conferir dps Não funciona
    if (key == GLUT_ACTIVE_CTRL) {
      fprintf(stdout, "INFO: OpenGL Version: %s\n",glGetString(GL_VERSION));
    }
	if(key == GLUT_KEY_UP) {
		cz++;
	}
	if(key == GLUT_KEY_DOWN) {
		cz--;
	}
	if(key == GLUT_KEY_LEFT) {
		cx--;
	}
	if(key == GLUT_KEY_RIGHT) {
		cx++;
	}
	fprintf(stdout, "cx: %i, cz:%i\n", cx, cz);
}

void TimerFunction(int Value) {
  if (0 != Value) {
    char *TempString = (char *)
    malloc(512 + strlen(WINDOW_TITLE));

    sprintf(TempString, "%s: %d Frames Per Second @ %d x %d", WINDOW_TITLE, window.FrameCount * 4, window.WIDTH, window.HEIGHT);
    glutSetWindowTitle(TempString);
    free(TempString);
  }
  
  window.FrameCount = 0;
  glutTimerFunc(250, TimerFunction, 1);
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