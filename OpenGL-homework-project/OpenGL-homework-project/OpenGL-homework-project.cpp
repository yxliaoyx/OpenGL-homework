#include <GL/glut.h>
#include <iostream>
#include <time.h>
using namespace std;

#define t 0.01
#define max_bullet 5

struct Bullet {
	GLdouble x, y, z, vx, vy, vz;
};
Bullet player_bullet[max_bullet];
Bullet enemy_bullet[max_bullet];

int player_bullet_count = 0, enemy_bullet_count = 0;
int window_width = 1280, window_height = 720, room_length = 1000;
int center_x = window_width / 2, center_y = window_height / 2;
double player_x = 0, player_y = 0, player_z = 0, player_y_velocity = 0;
double rotation = 0, look_up_down = 0;
double mouse_last_x = window_width / 2, mouse_last_y = window_height / 2;
double reloading = 0;
bool reset_Mouse = false, cursor = true;

void createBullet(Bullet & bullet) {
	bullet.x = player_x + cos(rotation) * 10;
	bullet.y = player_y + look_up_down * 10;
	bullet.z = player_z + sin(rotation) * 10;
	bullet.vx = cos(rotation) * 100;
	bullet.vy = look_up_down * 100;
	bullet.vz = sin(rotation) * 100;
}

void player_shoot(Bullet & bullet) {
	bullet.vx = cos(rotation) * 1500;
	bullet.vy = look_up_down * 1500;
	bullet.vz = sin(rotation) * 1500;
	bullet.x = player_x + cos(rotation) * 10;
	bullet.y = player_y + look_up_down * 10;
	bullet.z = player_z + sin(rotation) * 10;
}

//void DrawBullet(Bullet bullet) {
//	glPushMatrix();
//	glTranslated(bullet.x, bullet.y, bullet.z);
//	glColor3f(0, 0, 1);
//	glutSolidSphere(ball.r, 10, 10);
//	glPopMatrix();
//}

bool edgeCollision(Bullet & bullet) {
	int boundary = room_length / 2;
	if (bullet.x < -boundary || bullet.x > boundary
		|| bullet.y < -boundary || bullet.y > boundary
		|| bullet.z < -boundary || bullet.z > boundary) {
		return true;
	}
	else {
		return false;
	}
}

//void Collision(int i) {
//	for (int j = 0; j < n; j++) {
//		if (i != j) {
//			double dx = ball[i].x - ball[j].x;
//			double dy = ball[i].y - ball[j].y;
//			double dz = ball[i].z - ball[j].z;
//			double d = sqrt(dx * dx + dy * dy + dz * dz);
//
//			if (d <= ball[i].r + ball[j].r) {
//				double v1n = (ball[i].vx * dx + ball[i].vy * dy + ball[i].vz * dz) / d;//法線方向的速度
//				double v1ix = v1n * dx / d;//法線方向的速度的x分量
//				double v1iy = v1n * dy / d;//法線方向的速度的y分量
//				double v1iz = v1n * dz / d;//法線方向的速度的z分量
//
//				double v2n = (ball[j].vx * dx + ball[j].vy * dy + ball[j].vz * dz) / d;
//				double v2ix = v2n * dx / d;
//				double v2iy = v2n * dy / d;
//				double v2iz = v2n * dz / d;
//
//				double m1 = pow(ball[i].r, 3);
//				double m2 = pow(ball[j].r, 3);
//
//				double v1fx = (m1 * v1ix + m2 * v2ix + CrBB * m2 * (v2ix - v1ix)) / (m1 + m2);
//				double v1fy = (m1 * v1iy + m2 * v2iy + CrBB * m2 * (v2iy - v1iy)) / (m1 + m2);
//				double v1fz = (m1 * v1iz + m2 * v2iz + CrBB * m2 * (v2iz - v1iz)) / (m1 + m2);
//
//				double v2fx = (m1 * v1ix + m2 * v2ix + CrBB * m1 * (v1ix - v2ix)) / (m1 + m2);
//				double v2fy = (m1 * v1iy + m2 * v2iy + CrBB * m1 * (v1iy - v2iy)) / (m1 + m2);
//				double v2fz = (m1 * v1iz + m2 * v2iz + CrBB * m1 * (v1iz - v2iz)) / (m1 + m2);
//
//				ball[i].vx = v1fx + ball[i].vx - v1ix;
//				ball[i].vy = v1fy + ball[i].vy - v1iy;
//				ball[i].vz = v1fz + ball[i].vz - v1iz;
//
//				ball[j].vx = v2fx + ball[j].vx - v2ix;
//				ball[j].vy = v2fy + ball[j].vy - v2iy;
//				ball[j].vz = v2fz + ball[j].vz - v2iz;
//			}
//		}
//	}
//}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_COLOR_MATERIAL);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(player_x, player_y, player_z, player_x + cos(rotation), player_y + look_up_down, player_z + sin(rotation), 0, 1, 0);
	//gluLookAt(0, 5000, 0, 0, 0, 0, 1, 0, 0);

	glPushMatrix();
	glColor3f(0, 1, 0);
	glTranslated(player_x, player_y, player_z);
	glutWireSphere(100, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0.5, 0);
	glTranslated(room_length, 0, 0);
	glutSolidCube(room_length);
	glTranslated(-room_length * 2, 0, 0);
	glutSolidCube(room_length);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, -(room_length / 2 + 50), 0);
	glColor3f(0.5, 0.5, 0.5);
	glutSolidCube(room_length);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.5, 0, 0.5);
	glTranslated(0, 0, room_length);
	glutSolidCube(room_length);
	glTranslated(-0 * 2, 0, -room_length * 2);
	glutSolidCube(1000);
	glPopMatrix();

	for (int i = 0; i < player_bullet_count; i++) {
		if (edgeCollision(player_bullet[i])) {
			player_bullet_count--;
			player_bullet[i] = player_bullet[player_bullet_count];
		}
		glPushMatrix();
		glTranslated(player_bullet[i].x, player_bullet[i].y, player_bullet[i].z);
		glColor3f(1, 1, 1);
		glutSolidSphere(1, 20, 20);
		glPopMatrix();

		player_bullet[i].x += player_bullet[i].vx * t;
		player_bullet[i].y += player_bullet[i].vy * t;
		player_bullet[i].z += player_bullet[i].vz * t;
	}

	player_y += player_y_velocity * t;
	if (player_y > 0) {
		player_y_velocity -= 9.8 * t;
	}
	else {
		player_y = 0;
	}

	reloading -= t;
	if (reloading > 0) {
		glutSetCursor(GLUT_CURSOR_WAIT);
	}
	else {
		reloading = 0;
		if (cursor) {
			glutSetCursor(GLUT_CURSOR_CROSSHAIR);
		}
		else {
			glutSetCursor(GLUT_CURSOR_NONE);
		}
	}

	//for (int i = 0; i < n; i++) {
	//	DrawBall(bullet[i]);
	//	//Collision(i);
	//	bullet[i].x += bullet[i].vx * t;
	//	bullet[i].y += bullet[i].vy * t;
	//	bullet[i].z += bullet[i].vz * t;
	//}

	glFlush();
}

void reshape(int width, int height) {
	window_width = width;
	window_height = height;
	mouse_last_x = center_x = window_width / 2;
	mouse_last_y = center_y = window_height / 2;
	glViewport(0, 0, window_width, window_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, (GLfloat)window_width / window_height, 1, 10000);
}

void specialKey(int key, int x, int y) {
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
	case ' ':
		if (player_y == 0) {
			player_y_velocity = 10;
		}
		break;
	case 'q':
		cursor = !cursor;
		break;
	}
	glutPostRedisplay();
}

void Timer(int c) {
	glutPostRedisplay();
	glutTimerFunc(10, Timer, 0);
}

void mouseButton(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && reloading == 0) {
		player_shoot(player_bullet[player_bullet_count]);
		player_bullet_count++;
		if (player_bullet_count >= max_bullet) {
			reloading = 2;
		}
	}
}

void mouseMove(int x, int y) {
	if (reset_Mouse == false) {
		double mouseDiffX = x - mouse_last_x;
		double mouseDiffY = y - mouse_last_y;

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
	glutWarpPointer(center_x, center_y);
	srand((unsigned)time(NULL));
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseButton);
	glutPassiveMotionFunc(mouseMove);
	glutTimerFunc(10, Timer, 0);

	glutMainLoop();
	return 0;
}