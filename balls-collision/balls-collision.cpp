#include <GL/glut.h>
#include <iostream>
#include <time.h>
using namespace std;

#define G -981//重力加速度
#define CrBB 0.95//球與球的恢復係數
#define CrWB 0.9//牆與球的恢復係數
#define Cd 0.2//空氣阻力
#define t 0.001//每偵時間
#define Max 21//最大球數

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
	/*n = 1;
	ball[0].x = -400;
	ball[0].y = -400;
	ball[0].z = 0;
	ball[0].vx = 1000;
	ball[0].vy = 1000;
	ball[0].vz = 0;
	ball[0].r = 30;*/

	/*ball[1].x = 0;
	ball[1].y = -30;
	ball[1].z = 0;
	ball[1].vx = 0;
	ball[1].vy = 0;
	ball[1].vz = 0;
	ball[1].r = 30;

	ball[2].x = 30;
	ball[2].y = -60;
	ball[2].z = 0;
	ball[2].vx = 0;
	ball[2].vy = 0;
	ball[2].vz = 0;
	ball[2].r = 30;*/
	
	srand(time(NULL));
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void DrawBall(Ball ball) {
	glPushMatrix();
	glTranslated(ball.x, ball.y, ball.z);
	if (ball.r < 40){
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
				double v1n = (ball[i].vx * dx + ball[i].vy * dy + ball[i].vz * dz) / d;//法線方向的速度
				double v1ix = v1n * dx / d;//法線方向的速度的x分量
				double v1iy = v1n * dy / d;//法線方向的速度的y分量
				double v1iz = v1n * dz / d;//法線方向的速度的z分量

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
	double ax = ball[i].vx * Cd;
	double ay = ball[i].vy * Cd;
	double az = ball[i].vz * Cd;
	ball[i].vx -= ax * t;
	ball[i].vy -= ay * t;
	ball[i].vz -= az * t;
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_COLOR_MATERIAL);
	glPushMatrix();
	glTranslated(0, 0, 0);
	glColor3f(0, 1, 0);
	glutWireCube(1000);
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
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, 1, 1, 5000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 3000, 0, 0, 0, 0, 1, 0);
}

void SpecialKey(GLint key, GLint x, GLint y) {
	switch (key) {
	case GLUT_KEY_UP:
		glRotatef(5.0, 1, 0, 0);
		break;
	case GLUT_KEY_LEFT:
		glRotatef(5.0, 0, 1, 0);
		break;
	case GLUT_KEY_DOWN:
		glRotatef(-5.0, 1, 0, 0);
		break;
	case GLUT_KEY_RIGHT:
		glRotatef(-5.0, 0, 1, 0);
		break;
	}
}

void keyboard(unsigned char key, int x, int y) {
	if (n < Max) {
		switch (key) {
		case 'b':
			RandBall(n);
			ball[n].r = 45;
			n++;
			break;
		case 's':
			RandBall(n);
			ball[n].r = 30;
			n++;
			break;
		default:
			break;
		}
	}
}

void Timer(int c) {
	glutPostRedisplay();
	glutTimerFunc(1, Timer, 0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1280, 1280);
	glutCreateWindow("Project #2");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(SpecialKey);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(1, Timer, 0);
	glutMainLoop();
	return 0;
}
