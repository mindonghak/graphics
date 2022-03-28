#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
#define GLUT_KEY_SHIFT 112

int rightbuttonpressed = 0;
int shift_pressed = 0, p_pressed = 0, r_pressed = 0;
int WinSizeX = 500;
int WinSizeY = 500;
int points[129][2] = {};
int rotate[129][2] = {};
int G[2] = {};
//float r = 0.0f, g = 0.0f, b = 0.0f; // Backgroud color = Black
float r = 250.0f / 255.0f, g = 128.0f / 255.0f, b = 114.0f / 255.0f; // Background color = Salmon

GLfloat toglx(int x) {
	int w = WinSizeX;
	return (GLfloat)((x - w / 2.0) * (2.0 / w));
}
GLfloat togly(int y) {
	int h = WinSizeY;
	return -(GLfloat)((y - h / 2.0) * (2.0 / h));
}



#define TO_RADIAN 0.01745329252f  
#define TO_DEGREE 57.295779513f
int animation_mode = 0;
unsigned int timestamp = 0;

void timer(int value) {
	timestamp = (timestamp + 1) % UINT_MAX;
	glutPostRedisplay();
	if (animation_mode)
		glutTimerFunc(10, timer, 0);
}


void makepoint(int x, int y) {
	int i;
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(toglx(x), togly(y));
	glEnd();

	if (points[0][0] > 1&&  !r_pressed) {
		for (i = 1; i < points[0][0]; i++) {
			glColor3f(0.5f, 1.0f, 1.0f);
			glBegin(GL_LINES);
			glVertex2f(toglx(points[i][0]), togly(points[i][1])); glVertex2f(toglx(points[i+1][0]), togly(points[i+1][1]));
			glEnd();
		}
	}
	
	else if (points[0][0] > 1 && r_pressed) {
		for (i = 1; i < points[0][0]; i++) {
			glColor3f(0.5f, 1.0f, 1.0f);
			glBegin(GL_LINES);
			glVertex2f(toglx(rotate[i][0]), togly(rotate[i][1])); glVertex2f(toglx(rotate[i + 1][0]), togly(rotate[i + 1][1]));
			glEnd();
		}
	}
	
}
void display(void) {
	int i, num = points[0][0];
	//fprintf(stdout, "display executed\n");
	glClearColor(r, g, b, 1.0f); 
	glClear(GL_COLOR_BUFFER_BIT);
	if (num>0&&!r_pressed) {
		for (i = 1; i <= num; i++) {
			makepoint(points[i][0], points[i][1]);
		}
	}
	if (p_pressed&&!r_pressed) {
		glColor3f(0.5f, 1.0f, 1.0f);
		glBegin(GL_LINES);
		glVertex2f(toglx(points[num][0]), togly(points[num][1])); glVertex2f(toglx(points[1][0]), togly(points[1][1]));
		glEnd();
	}



	int point_clock = (timestamp % 1442) / 2 - 360;

	
	if (r_pressed) {
		glBegin(GL_POINTS);
		glColor3f(1.0f, 1.0f, 0.5f);
		glVertex2f(toglx(G[0]), togly(G[1]));
		glEnd();
		
		for (i = 1; i <= num; i++) {
			rotate[i][0] = (points[i][0] - G[0]) * cos(-point_clock * TO_RADIAN) - (points[i][1] - G[1]) * sin(-point_clock * TO_RADIAN) + G[0];
			rotate[i][1] = (points[i][0] - G[0]) * sin(-point_clock * TO_RADIAN) + (points[i][1] - G[1]) * cos(-point_clock * TO_RADIAN) + G[1];
			makepoint(rotate[i][0], rotate[i][1]);
			glColor3f(1.0f, 1.0f, 1.0f);
			glBegin(GL_LINES);
			glVertex2f(toglx(rotate[num][0]), togly(rotate[num][1])); glVertex2f(toglx(rotate[1][0]), togly(rotate[1][1]));
			glEnd();
		}
		
	}
	

	glFlush();
}

void keyboard(unsigned char key, int x, int y) {
	int j,i;
	int num = points[0][0];
	switch (key) {
	case 'r':
		if (p_pressed) {
			if (r_pressed) {
				for (i = 1; i <= num; i++) {
					points[i][0] = rotate[i][0];
					points[i][1] = rotate[i][1];
				}
				timestamp = 0;
			}
			r_pressed = 1 - r_pressed;
			G[0] = 0;
			G[1] = 0;
			for (j = 0; j < 2; j++) {
				for (i = 1; i <= num; i++) {
					G[j] += points[i][j];
				}
				G[j] = (float)G[j] / num;
			}
			animation_mode = 1 - animation_mode;
			if (animation_mode)
				glutTimerFunc(10, timer, 0);
		}
		break;
	case 'f':
		glutLeaveMainLoop();
		break;
	case 'p':
		if (points[0][0] > 2) {
			p_pressed = 1;
			glutPostRedisplay();
		}
		else {
			fprintf(stdout, "you need at least 3 points.\n");
		}
		break;
	case 'c':
		if (r_pressed) {
			break;
		}
		points[0][0] = 0;
		p_pressed = 0;
		r_pressed = 0;
		glutPostRedisplay();
		break;
	}
}

void special(int key, int x, int y) {
	int moverate = 3;
	int num = points[0][0], i;
	if (r_pressed) {
		return;
	}
	switch (key) {
	case GLUT_KEY_SHIFT:
		shift_pressed = 1;
		//fprintf(stdout, "shift is pressed\n");
		break;
	case GLUT_KEY_LEFT:
		if (p_pressed) {
			for (i = 1; i <= num; i++) {
				points[i][0] -= moverate;
			}
		}
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:
		if (p_pressed) {
			for (i = 1; i <= num; i++) {
				points[i][0] += moverate;
			}
		}
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:
		if (p_pressed) {
			for (i = 1; i <= num; i++) {
				points[i][1] += moverate;
			}
		}
		glutPostRedisplay();
		break;
	case GLUT_KEY_UP:
		if (p_pressed) {
			for (i = 1; i <= num; i++) {
				points[i][1] -= moverate;
			}
		}
		glutPostRedisplay();
		break;
	}
}

void specialup(int key, int  x, int y) {
	switch (key) {
	case GLUT_KEY_SHIFT:
		shift_pressed = 0;
		//fprintf(stdout, "shift is not pressed\n");
		break;
	}
}

	int nowx, nowy;

void mousepress(int button, int state, int x, int y) {
	int num;
	if (r_pressed) {
		return;
	}
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
		if (shift_pressed&&!p_pressed) {

			num = ++points[0][0];
			points[num][0] = x;
			points[num][1] = y;
			glutPostRedisplay();
		}
		fprintf(stdout, "*** The left mouse button was pressed at (%d, %d).\n", x, y);
	}
	else if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP)) {
		
	}
	else if ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN)) {
		rightbuttonpressed = 1;
		nowx = x;
		nowy = y;
	}
	else if ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_UP))
		rightbuttonpressed = 0;
}

void mousemove(int x, int y) {
	int i;
	int changex = x - nowx;
	int changey = y - nowy;
	glutPostRedisplay();
	if (rightbuttonpressed&&p_pressed){
		for (i = 1; i <= points[0][0]; i++) {
			points[i][0] += changex;
			points[i][1] += changey;
		}
		nowx = x;
		nowy = y;
	}
}
	
void reshape(int width, int height) {
	int i;
	int num = points[0][0];
	fprintf(stdout, "### The new window size is %dx%d.\n", width, height);
	for (i = 1; i <= num; i++) {
		points[i][0] *= ((float)width / WinSizeX);
		points[i][1] *= ((float)height / WinSizeY);
	}
	WinSizeX = width;
	WinSizeY = height;
	glViewport(0, 0, WinSizeX, WinSizeY);
}

void close(void) {
	fprintf(stdout, "\n^^^ The control is at the close callback function now.\n\n");
}

void register_callbacks(void) {
	glutSpecialUpFunc(specialup);
	glutMouseFunc(mousepress);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMotionFunc(mousemove);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
 	glutCloseFunc(close);
}

void initialize_renderer(void) {
	register_callbacks();
}

void initialize_glew(void) {
	GLenum error;

	glewExperimental = TRUE;
	error = glewInit();
	if (error != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(error));
		exit(-1);
	}
	fprintf(stdout, "*********************************************************\n");
	fprintf(stdout, " - GLEW version supported: %s\n", glewGetString(GLEW_VERSION));
	fprintf(stdout, " - OpenGL renderer: %s\n", glGetString(GL_RENDERER));
	fprintf(stdout, " - OpenGL version supported: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "*********************************************************\n\n");
}

void greetings(char *program_name, char messages[][256], int n_message_lines) {
	fprintf(stdout, "**************************************************************\n\n");
	fprintf(stdout, "  PROGRAM NAME: %s\n\n", program_name);
	fprintf(stdout, "    This program was coded for CSE4170 students\n");
	fprintf(stdout, "      of Dept. of Comp. Sci. & Eng., Sogang University.\n\n");

	for (int i = 0; i < n_message_lines; i++)
		fprintf(stdout, "%s\n", messages[i]);
	fprintf(stdout, "\n**************************************************************\n\n");

	initialize_glew();
}

#define N_MESSAGE_LINES 4
void main(int argc, char *argv[]) {
	char program_name[64] = "Sogang CSE4170 SimplefreeGLUTcode";
	char messages[N_MESSAGE_LINES][256] = {
		"    - Keys used: 'c', 'r', 'f', 'p'",
		"    - Special keys used: SHIFT, LEFT, RIGHT, UP, DOWN",
		"    - Mouse used: L-click, R-click and move",
		"    - Other operations: window size change"
	};

	glutInit(&argc, argv);
	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); // <-- Be sure to use this profile for this example code!
 //	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutInitDisplayMode(GLUT_RGBA);

	glutInitWindowSize(WinSizeX, WinSizeY);
	glutInitWindowPosition(500, 200);
	glutCreateWindow(program_name);

	greetings(program_name, messages, N_MESSAGE_LINES);
	initialize_renderer();

   // glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_EXIT); // default
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	
	glutMainLoop();
	fprintf(stdout, "^^^ The control is at the end of main function now.\n\n");
}