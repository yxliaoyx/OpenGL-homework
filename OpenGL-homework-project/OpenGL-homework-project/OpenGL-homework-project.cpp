#include <iostream>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cstdlib>
#include <cmath>

using namespace std;

int window_width = 1280, window_height = 720;

GLfloat myPositionX = 0;
GLfloat myPositionY = 10;
GLfloat myPositionZ = 0;

GLfloat myRotation = 0;

GLfloat myLookUpDown = 0;

GLfloat mouseLastX;
GLfloat mouseLastY;

int centerX;
int centerY;

bool reset_Mouse = false;

void reshape(int window_width, int window_height) {
	glViewport(0, 0, window_width, window_height);
	gluPerspective(45, (GLfloat)window_width / (GLfloat)window_height, 1, 1000);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotatef(myLookUpDown, 1, 0, 0);
	glRotatef(myRotation, 0, 1, 0);
	glTranslatef(myPositionX, myPositionY, myPositionZ);

	//glBindTexture(GL_TEXTURE_2D, texture);
	//cube.draw();
	/*glPushMatrix();
	glTranslated(0, 0, 0);
	glColor3f(0, 1, 0);
	glutWireSphere(100, 10, 10);
	glPopMatrix();*/
	glBegin(GL_QUADS);
	glVertex3f(-100, 0, 100);
	glVertex3f(-100, 0, -100);
	glVertex3f(100, 0, -100);
	glVertex3f(100, 0, 100);
	glEnd();

	//Floor
	//glBindTexture(GL_TEXTURE_2D, texture);
	/*glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(Floor.vertexs[0].x, Floor.vertexs[0].y, Floor.vertexs[0].z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(Floor.vertexs[1].x, Floor.vertexs[1].y, Floor.vertexs[1].z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(Floor.vertexs[2].x, Floor.vertexs[2].y, Floor.vertexs[2].z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(Floor.vertexs[3].x, Floor.vertexs[3].y, Floor.vertexs[3].z);
	glEnd();*/

	glutSwapBuffers();
}

void keyBoardFunc(unsigned char key, int x, int y) {
	GLfloat rad = myRotation / 57.29577951;
	switch (key) {
	case 'w':
		myPositionX += sin(rad);
		myPositionZ += cos(rad);
		break;

	case 'a':
		myPositionX += cos(rad);
		myPositionZ -= sin(rad);
		break;

	case 's':
		myPositionX -= sin(rad);
		myPositionZ -= cos(rad);
		break;

	case 'd':
		myPositionX -= cos(rad);
		myPositionZ += sin(rad);
		break;
	}
	glutPostRedisplay();
}

void mouseMovementFunc(int x, int y) {
	if (reset_Mouse == false) {
		int mouseDiffX = x - mouseLastX;
		int mouseDiffY = y - mouseLastY;

		mouseLastX = x;
		mouseLastY = y;

		myRotation += mouseDiffX;
		if (myRotation > 360) {
			myRotation -= 360;
		}
		else if (myRotation < 0) {
			myRotation += 360;
		}
		myLookUpDown += mouseDiffY;
		if (myLookUpDown > 90) {
			myLookUpDown = 90;
		}
		else if (myLookUpDown < -90) {
			myLookUpDown = -90;
		}

		if ((int)mouseLastX != centerX && (int)mouseLastY != centerY) {
			reset_Mouse = true;
		}
	}
	else if (reset_Mouse == true) {
		glutWarpPointer(centerX, centerY);
		mouseLastX = centerX;
		mouseLastY = centerY;

		reset_Mouse = false;
	}
	glutPostRedisplay();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("Project #5");

	glMatrixMode(GL_PROJECTION);

	glEnable(GL_DEPTH_TEST);

	/*Floor.vertexs[0].x = 0.0f;
	Floor.vertexs[0].y = -2.0f;
	Floor.vertexs[0].z = 10.0f;
	Floor.vertexs[1].x = -10.0f;
	Floor.vertexs[1].y = -2.0f;
	Floor.vertexs[1].z = 0.0f;
	Floor.vertexs[2].x = 0.0f;
	Floor.vertexs[2].y = -2.0f;
	Floor.vertexs[2].z = -10.0f;
	Floor.vertexs[3].x = 10.0f;
	Floor.vertexs[3].y = -2.0f;
	Floor.vertexs[3].z = 0.0f;*/

	mouseLastX = centerX = window_width / 2;
	mouseLastY = centerY = window_height / 2;

	glutSetCursor(GLUT_CURSOR_CROSSHAIR);

	glutWarpPointer(mouseLastX, mouseLastY);

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyBoardFunc);
	glutPassiveMotionFunc(mouseMovementFunc);

	glutMainLoop();

	return 0;
}