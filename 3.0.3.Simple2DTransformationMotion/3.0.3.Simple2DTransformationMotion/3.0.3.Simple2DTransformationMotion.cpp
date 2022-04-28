#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>

#include "Shaders/LoadShaders.h"
GLuint h_ShaderProgram; // handle to shader program
GLint loc_ModelViewProjectionMatrix, loc_primitive_color; // indices of uniform variables

// include glm/*.hpp only if necessary
//#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, ortho, etc.
glm::mat4 ModelViewProjectionMatrix;
glm::mat4 ViewMatrix, ProjectionMatrix, ViewProjectionMatrix;

#define TO_RADIAN 0.01745329252f  
#define TO_DEGREE 57.295779513f
#define BUFFER_OFFSET(offset) ((GLvoid *) (offset))

#define LOC_VERTEX 0

int win_width = 0, win_height = 0; 
float centerx = 0.0f, centery = 0.0f; //mouse left clicked position

// 2D 물체 정의 부분은 objects.h 파일로 분리
// 새로운 물체 추가 시 prepare_scene() 함수에서 해당 물체에 대한 prepare_***() 함수를 수행함.
// (필수는 아니나 올바른 코딩을 위하여) cleanup() 함수에서 해당 resource를 free 시킴.
#include "objects.h"

int leftbuttonpressed = 0;
int animation_mode = 1;
unsigned int timestamp = 0;

void timer(int value) {
	timestamp = (timestamp + 1) % UINT_MAX;
	glutPostRedisplay();
	if (animation_mode)
		glutTimerFunc(10, timer, 0);
}

void display(void) {
	
	glm::mat4 ModelMatrix;
	
	glClear(GL_COLOR_BUFFER_BIT);
	
	ModelMatrix = glm::mat4(1.0f);
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_axes();
    draw_house(); // in MC
	
	int airplane_clock = (timestamp % 1442) / 2 - 360;
	GLfloat click_radius = 100.0f;
	if (leftbuttonpressed) {
		click_radius -= timestamp % 10;
	}
	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(400.0f, 300.0f, 0.0f));
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(click_radius * cosf(airplane_clock * TO_RADIAN), click_radius * sinf(airplane_clock * TO_RADIAN), 0.0f));
	ModelMatrix = glm::rotate(ModelMatrix, (airplane_clock + 180) * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_airplane();

	int airplane_shadow1 = (airplane_clock - 40);
	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3( 400.0f,  300.0f, 0.0f));
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(click_radius * cosf(airplane_shadow1 * TO_RADIAN), click_radius * sinf(airplane_shadow1 * TO_RADIAN), 0.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.6f, 0.6f, 1.0f));
	ModelMatrix = glm::rotate(ModelMatrix, (airplane_shadow1 + 180) * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_airplane();

	int airplane_shadow2 = (airplane_clock - 60);
	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3( 400.0f,  300.0f, 0.0f));
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(click_radius * cosf(airplane_shadow2 * TO_RADIAN), click_radius * sinf(airplane_shadow2 * TO_RADIAN), 0.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.4f, 0.4f, 1.0f));
	ModelMatrix = glm::rotate(ModelMatrix, (airplane_shadow2 + 180) * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_airplane();

	int sword_cake_timing = 150;
	int sword_clock = timestamp % (200 + sword_cake_timing);
	if (sword_clock > 200) {
		sword_clock = 0;
	}
	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, sword_clock * sword_clock/40, 0.0f));
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(400.0f, -400.0f, 0.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3((200 - sword_clock)/20.0f, (200 - sword_clock) / 20.0f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_sword();

	int cake_clock = timestamp % (200 + sword_cake_timing);
	if (cake_clock <= sword_cake_timing) {
		cake_clock = 0;
	}
	else {
		cake_clock -= sword_cake_timing;
	}

	int cake_rotate = cake_clock * 90 / 200.0;
	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(400.0f, 100.0f, 0.0f));
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-cake_clock, -cake_clock * cake_clock / 200 + 100.0f, 0.0f));
	ModelMatrix = glm::rotate(ModelMatrix, (cake_rotate)*TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(3.5f, 3.5f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_cake1();

	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(400.0f, 100.0f, 0.0f));
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(cake_clock, -cake_clock * cake_clock / 200 + 100.0f, 0.0f));
	ModelMatrix = glm::rotate(ModelMatrix, (-cake_rotate) * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(3.5f, 3.5f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_cake2();

	int road_clock[8],i;
	//road_clock[0] = timestamp % 800 - 400;
	for (i = 0; i < 8; i++) {
		road_clock[i] = (timestamp + 100 * i) % 800 - 400;
		ModelMatrix = glm::mat4(1.0f);
		if (leftbuttonpressed) {
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(road_clock[i],0.0f, 0.0f));
			ModelMatrix = glm::rotate(ModelMatrix, (road_clock[i]) * TO_RADIAN, glm::vec3(-1.0f, 0.0f, 0.0f));
			ModelMatrix = glm::scale(ModelMatrix, glm::vec3(-road_clock[i] / 400.0f + 1.0f, -road_clock[i] / 200.0f + 1.0f, 1.0f));
		}
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-100.0f, road_clock[i], 0.0f));
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 400.0f, 0.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(-road_clock[i] / 400.0f + 1.0f, -road_clock[i] / 400.0f + 1.0f, 1.0f));
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -400.0f, 0.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_road();
		if (i % 3 == 0) {
			ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 1.0f));
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-150.0f, 0.0f, 0.0f));
			ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
			glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
			draw_house();
		}
		else if (i % 4 == 0) {
			ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.5f, 1.5f, 1.0f));
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-150.0f, 0.0f, 0.0f));
			ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
			glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
			draw_house();
		}
		else if (i % 5 == 0) {
			ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 1.0f));
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-150.0f, 0.0f, 0.0f));
			ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
			glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
			draw_house();
		}
		else {
			ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 1.0f));
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(150.0f, 0.0f, 0.0f));
			ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
			glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
			draw_house();
		}
	}

	int car_clock = timestamp % 800;

	ModelMatrix = glm::mat4(1.0f);

	if (leftbuttonpressed) {
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(car_clock, 0.0f, 0.0f));
		ModelMatrix = glm::rotate(ModelMatrix, (car_clock) * TO_RADIAN, glm::vec3(-1.0f, 0.0f, 0.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(-car_clock / 400.0f + 1.0f, -car_clock / 400.0f + 1.0f, 1.0f));
	}

	if (car_clock > 200 && car_clock < 360) {
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-160.0f + (car_clock-200), pow(6.0f - car_clock / 60.0f, 3), 0.0f));
	}
	else if (car_clock >= 360) {
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, pow(6.0f - car_clock / 60.0f, 3), 0.0f));
	}
	else {
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-160.0f, pow(6.0f - car_clock/60.0f,3), 0.0f));
	}
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.5f + car_clock/800.0f, 1.5f + car_clock/800.0f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_car();

















	glFlush();	
}   

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'a':
		animation_mode = 1 - animation_mode;
		if (animation_mode)
			glutTimerFunc(10, timer, 0);
		break;
	case 27: // ESC key
		glutLeaveMainLoop(); // Incur destuction callback for cleanups.
		break;
	}
}

void mouse(int button, int state, int x, int y) {
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
		leftbuttonpressed = 1;
	else if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP))
		leftbuttonpressed = 0;
}

void motion(int x, int y) {
	if (leftbuttonpressed) {
		centerx =  x - win_width/2.0f, centery = (win_height - y) - win_height/2.0f;
		glutPostRedisplay();
	}
} 
	
void reshape(int width, int height) {
	win_width = width, win_height = height;
	
  	glViewport(0, 0, win_width, win_height);
	ProjectionMatrix = glm::ortho(-win_width / 2.0, win_width / 2.0, 
		-win_height / 2.0, win_height / 2.0, -1000.0, 1000.0);
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

	update_axes();

	glutPostRedisplay();
}

void cleanup(void) {
	glDeleteVertexArrays(1, &VAO_axes);
	glDeleteBuffers(1, &VBO_axes);

	glDeleteVertexArrays(1, &VAO_airplane);
	glDeleteBuffers(1, &VBO_airplane);

	glDeleteVertexArrays(1, &VAO_house);
	glDeleteBuffers(1, &VBO_house);
}

void register_callbacks(void) {
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutReshapeFunc(reshape);
	glutTimerFunc(10, timer, 0); // animation_mode = 1
	glutCloseFunc(cleanup);
}

void prepare_shader_program(void) {
	ShaderInfo shader_info[3] = {
		{ GL_VERTEX_SHADER, "Shaders/simple.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/simple.frag" },
		{ GL_NONE, NULL }
	};

	h_ShaderProgram = LoadShaders(shader_info);
	glUseProgram(h_ShaderProgram);

	loc_ModelViewProjectionMatrix = glGetUniformLocation(h_ShaderProgram, "u_ModelViewProjectionMatrix");
	loc_primitive_color = glGetUniformLocation(h_ShaderProgram, "u_primitive_color");
}

void initialize_OpenGL(void) {
	glEnable(GL_MULTISAMPLE); 
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glClearColor(184 / 255.0f, 248 / 255.0f, 250 / 255.0f, 1.0f);
	ViewMatrix = glm::mat4(1.0f);
}

void prepare_scene(void) {
	prepare_axes();
	prepare_airplane();
	prepare_house();
	prepare_sword();
	prepare_cake();
	prepare_cake1();
	prepare_cake2();
	prepare_road();
	prepare_car();
}

void initialize_renderer(void) {
	register_callbacks();
	prepare_shader_program(); 
	initialize_OpenGL();
	prepare_scene();
}

void initialize_glew(void) {
	GLenum error;

	glewExperimental = GL_TRUE;

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

#define N_MESSAGE_LINES 2
int main(int argc, char *argv[]) {
	char program_name[64] = "Sogang CSE4170 Simple2DTransformationMotion_GLSL_3.0.3";
	char messages[N_MESSAGE_LINES][256] = {
		"    - Keys used: 'a', 'ESC'"
		"    - Mouse used: L-click and move"
	};

	glutInit (&argc, argv);
 	glutInitDisplayMode(GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowSize (1200, 800);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(program_name);

	greetings(program_name, messages, N_MESSAGE_LINES);
	initialize_renderer();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutMainLoop();
}


