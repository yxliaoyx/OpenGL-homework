#include "GLInterface.h"
#include "GLSLSetup.h"

#include <stdlib.h>
#include <stdio.h>

#include <GL/glew.h>
#include <GL/glut.h>

float light_x = 0;
float light_y = 0;
float teapot_x = 0;
float teapot_y = 0;

float light_amb[4] = { 0.1, 0.1, 0.1, 1 };
float light_dif[4] = { 0.5, 0.5, 0.5, 1 };
float light_spe[4] = { 0.9, 0.9, 0.9, 1 };
float light_dir[4] = { light_x, light_y, 500, 1 };
float blackLight[4] = { 0 };

float material_amb[4] = { 0.5, 0.5, 0.5, 1 };
float material_dif[4] = { 0.75, 0.75, 0.75, 1 };
float material_spe[4] = { 0.9, 0.9, 0.9, 1 };
float shininess = 64.0f;

int shader_effect = 0;
bool openLight = true;

static GLubyte texels0[64][64][4];
static GLubyte texels1[64][64][4];

void makeTexImages(void) {
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			texels0[i][j][0] = (GLubyte)255;
			texels0[i][j][1] = (GLubyte)0;
			texels0[i][j][2] = (GLubyte)0;
			texels0[i][j][3] = (GLubyte)255;
		}
	}
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			texels1[i][j][0] = (GLubyte)0;
			texels1[i][j][1] = (GLubyte)0;
			texels1[i][j][2] = (GLubyte)255;
			texels1[i][j][3] = (GLubyte)255;
		}
	}
}

void texture(void) {
	GLuint texNames[2];

	makeTexImages();

	glGenTextures(2, texNames);
	glBindTexture(GL_TEXTURE_2D, texNames[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, texels0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, texNames[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, texels1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texNames[0]);
	glUniform1i(glGetUniformLocation(ReturnProgramID(shader_effect), "tex1"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texNames[1]);
	glUniform1i(glGetUniformLocation(ReturnProgramID(shader_effect), "tex2"), 1);
}

void Light() {
	glEnable(GL_LIGHTING);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
	if (openLight) {
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_dif);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_spe);
	}
	else {
		glLightfv(GL_LIGHT0, GL_DIFFUSE, blackLight);
		glLightfv(GL_LIGHT0, GL_SPECULAR, blackLight);
	}
	glLightfv(GL_LIGHT0, GL_POSITION, light_dir);

	glEnable(GL_LIGHT0);
}

void Display() {
	Light();
	glEnable(GL_DEPTH_TEST);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, 1.25, 1, 5000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 1500, 0, 0, 0, 1, 0, 0);

	glUseProgram(ReturnProgramID(shader_effect));

	glMaterialfv(GL_FRONT, GL_AMBIENT, material_amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_dif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_spe);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);

	texture();

	glBegin(GL_QUADS);
	glMultiTexCoord2f(GL_TEXTURE0, 0.0, 1.0);
	glMultiTexCoord2f(GL_TEXTURE1, 0.0, 1.0);
	glVertex3f(-300, 300, 0);

	glMultiTexCoord2f(GL_TEXTURE0, 0.0, 0.0);
	glMultiTexCoord2f(GL_TEXTURE1, 0.0, 0.0);
	glVertex3f(-300, -300, 0);

	glMultiTexCoord2f(GL_TEXTURE0, 1.0, 0.0);
	glMultiTexCoord2f(GL_TEXTURE1, 1.0, 0.0);
	glVertex3f(300, -300, 0);

	glMultiTexCoord2f(GL_TEXTURE0, 1.0, 1.0);
	glMultiTexCoord2f(GL_TEXTURE1, 1.0, 1.0);
	glVertex3f(300, 300, 0);
	glEnd();

	glPushMatrix();
	glTranslatef(teapot_x, teapot_y, 0);
	glRotatef(270, 0, 0, 1);
	glutSolidTeapot(200);
	glPopMatrix();

	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);

	glutSwapBuffers();
}

void KeyboardDown(unsigned char c, int x, int y) {
	switch (c) {
	case 'g':
		shader_effect = 0;
		break;
	case 'p':
		shader_effect = 1;
		break;
	case 'w':
		teapot_x += 10;
		break;
	case 'a':
		teapot_y += 10;
		break;
	case 's':
		teapot_x -= 10;
		break;
	case 'd':
		teapot_y -= 10;
		break;
	case 'f':
		openLight = !openLight;
	}
	glFinish();
}

void SpecialKey(GLint key, GLint x, GLint y) {
	switch (key) {
	case GLUT_KEY_UP:
		light_dir[0] += 10;
		break;
	case GLUT_KEY_LEFT:
		light_dir[1] += 10;
		break;
	case GLUT_KEY_DOWN:
		light_dir[0] -= 10;
		break;
	case GLUT_KEY_RIGHT:
		light_dir[1] -= 10;
		break;
	}
	glFinish();
}

void KeyboardUp(unsigned char c, int x, int y) {
	glFinish();
}

void Timer(int c) {
	glutPostRedisplay();
	glutTimerFunc(50, Timer, 0);
}