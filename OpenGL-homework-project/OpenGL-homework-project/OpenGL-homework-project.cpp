#include <GL/glut.h>
#include <iostream>
#include <time.h>
using namespace std;

#define G -981//硉
#define CrBB 0.95//瞴籔瞴確玒计
#define CrWB 0.9//鲤籔瞴確玒计
#define t 0.001//–盎丁
#define Max 21//程瞴计

int window_width = 1280, window_height = 720;
int center_x = window_width / 2;
int center_y = window_height / 2;
GLfloat player_x = 0;
GLfloat player_y = 0;
GLfloat player_z = 0;
GLfloat rotation = 0;
GLfloat look_up_down = 0;
GLfloat mouse_last_x = window_width / 2;
GLfloat mouse_last_y = window_height / 2;
bool reset_Mouse = false;

struct Ball {
	GLdouble x, y, z, vx, vy, vz, r;
};
Ball ball[Max];

int n = 0;

void RandBall(int n) {
	ball[n].x = rand() % 900 - 450;
	ball[n].y = rand() % 450;
	ball[n].z = rand() % 900 - 450;
	ball[n].vx = rand() % 4000 - 2000;
	ball[n].vy = rand() % 4000 - 2000;
	ball[n].vz = rand() % 4000 - 2000;
}

void init(void) {
	mouse_last_x = 640;
	srand(time(NULL));
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void DrawBall(Ball ball) {
	glPushMatrix();
	glTranslated(ball.x, ball.y, ball.z);
	if (ball.r < 40) {
		glColor3f(0, 0, 1);
	}
	else {
		glColor3f(1, 0, 0);
	}
	glutSolidSphere(ball.r, 20, 20);
	glPopMatrix();
}

void Collision(int i) {
	for (int j = 0; j < n; j++) {
		if (i != j) {
			double dx = ball[i].x - ball[j].x;
			double dy = ball[i].y - ball[j].y;
			double dz = ball[i].z - ball[j].z;
			double d = sqrt(dx * dx + dy * dy + dz * dz);

			if (d <= ball[i].r + ball[j].r) {
				double v1n = (ball[i].vx * dx + ball[i].vy * dy + ball[i].vz * dz) / d;//猭絬よ硉
				double v1ix = v1n * dx / d;//猭絬よ硉xだ秖
				double v1iy = v1n * dy / d;//猭絬よ硉yだ秖
				double v1iz = v1n * dz / d;//猭絬よ硉zだ秖

				double v2n = (ball[j].vx * dx + ball[j].vy * dy + ball[j].vz * dz) / d;
				double v2ix = v2n * dx / d;
				double v2iy = v2n * dy / d;
				double v2iz = v2n * dz / d;

				double m1 = pow(ball[i].r, 3);
				double m2 = pow(ball[j].r, 3);

				double v1fx = (m1 * v1ix + m2 * v2ix + CrBB * m2 * (v2ix - v1ix)) / (m1 + m2);
				double v1fy = (m1 * v1iy + m2 * v2iy + CrBB * m2 * (v2iy - v1iy)) / (m1 + m2);
				double v1fz = (m1 * v1iz + m2 * v2iz + CrBB * m2 * (v2iz - v1iz)) / (m1 + m2);

				double v2fx = (m1 * v1ix + m2 * v2ix + CrBB * m1 * (v1ix - v2ix)) / (m1 + m2);
				double v2fy = (m1 * v1iy + m2 * v2iy + CrBB * m1 * (v1iy - v2iy)) / (m1 + m2);
				double v2fz = (m1 * v1iz + m2 * v2iz + CrBB * m1 * (v1iz - v2iz)) / (m1 + m2);

				ball[i].vx = v1fx + ball[i].vx - v1ix;
				ball[i].vy = v1fy + ball[i].vy - v1iy;
				ball[i].vz = v1fz + ball[i].vz - v1iz;

				ball[j].vx = v2fx + ball[j].vx - v2ix;
				ball[j].vy = v2fy + ball[j].vy - v2iy;
				ball[j].vz = v2fz + ball[j].vz - v2iz;
			}
		}
	}
	bool floor = false;
	if (ball[i].x - ball[i].r <= -500 || ball[i].x + ball[i].r >= 500) {
		ball[i].vx = -ball[i].vx * CrWB;
	}
	if (ball[i].y - ball[i].r <= -500 || ball[i].y + ball[i].r >= 500) {
		ball[i].vy = -ball[i].vy * CrWB;
		if (ball[i].y - ball[i].r <= -500) {
			floor = true;
		}
	}
	if (ball[i].z - ball[i].r <= -500 || ball[i].z + ball[i].r >= 500) {
		ball[i].vz = -ball[i].vz * CrWB;
	}
	if (!floor) {
		ball[i].vy += G * t;
	}
}

void Drag(int i) {
	double ax = ball[i].vx;
	double ay = ball[i].vy;
	double az = ball[i].vz;
	ball[i].vx -= ax * t;
	ball[i].vy -= ay * t;
	ball[i].vz -= az * t;
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_COLOR_MATERIAL);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(player_x, player_y, player_z, player_x + cos(rotation), player_y + look_up_down, player_z + sin(rotation), 0, 1, 0);
	glPushMatrix();
	glTranslated(0, 0, 0);
	glColor3f(0, 1, 0);
	glutWireSphere(100, 30, 30);
	glPopMatrix();

	for (int i = 0; i < n; i++) {
		DrawBall(ball[i]);
		Collision(i);
		Drag(i);
		ball[i].x += ball[i].vx * t;
		ball[i].y += ball[i].vy * t;
		ball[i].z += ball[i].vz * t;
	}
	glFlush();
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, (GLfloat)w / h, 1, 5000);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
		player_x += cos(rotation);
		player_z += sin(rotation);
		break;

	case 'a':
		player_x += sin(rotation);
		player_z -= cos(rotation);
		break;

	case 's':
		player_x -= cos(rotation);
		player_z -= sin(rotation);
		break;

	case 'd':
		player_x -= sin(rotation);
		player_z += cos(rotation);
		break;
	}
	glutPostRedisplay();
}

void Timer(int c) {
	glutPostRedisplay();
	glutTimerFunc(1, Timer, 0);
}

void mouseMovementFunc(int x, int y) {
	if (reset_Mouse == false) {
		int mouseDiffX = x - mouse_last_x;
		int mouseDiffY = y - mouse_last_y;
		
		mouse_last_x = x;
		mouse_last_y = y;

		rotation += mouseDiffX * 0.01;

		look_up_down -= mouseDiffY * 0.01;
		if (look_up_down > 1) {
			look_up_down = 1;
		}
		else if (look_up_down < -1) {
			look_up_down = -1;
		}
		
		if (mouse_last_x != center_x || mouse_last_y != center_y) {
			reset_Mouse = true;
		}
	}
	else if (reset_Mouse == true) {
		glutWarpPointer(center_x, center_y);
		mouse_last_x = center_x;
		mouse_last_y = center_y;

		reset_Mouse = false;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("Project #2");
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);
	glutWarpPointer(center_x, center_y);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutPassiveMotionFunc(mouseMovementFunc);
	glutTimerFunc(1, Timer, 0);
	glutMainLoop();
	return 0;
}