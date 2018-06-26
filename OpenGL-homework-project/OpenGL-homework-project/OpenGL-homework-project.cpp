#include <GL/glut.h>
#include <al/alut.h>
#include <al/al.h>
#include <iostream>
#include <time.h>
using namespace std;

#define max_bullet 5

struct Bullet {
	GLdouble x, y, z, x_velocity, y_velocity, z_velocity;
};
Bullet player_bullet[max_bullet];
Bullet teapothead_bullet[max_bullet];

struct Teapothead {
	double x, y, z, red, green, blue, x_velocity, y_velocity, z_velocity;
};
Teapothead teapothead[100];

int input_number_of_teapotheads = 0, number_of_teapotheads = 0, number_of_player_bullets = 0, number_of_teapothead_bullets = 0;
int window_width = 1280, window_height = 720, room_length = 1000;
int center_x = window_width / 2, center_y = window_height / 2;
double player_x = room_length / 3, player_y = 0, player_z = room_length / 3, player_y_velocity = 0;
double rotation = 4, look_up_down = 0;
double mouse_last_x = window_width / 2, mouse_last_y = window_height / 2;
double t = 0.005, flying = 0;
bool reset_Mouse = false, injured = false, dead = false;

ALuint source1;

void createTeapothead(void) {
	number_of_teapotheads = input_number_of_teapotheads;
	for (int i = 0; i < number_of_teapotheads; i++) {
		teapothead[i].y = 0;
		if (i % 2) {
			teapothead[i].x = rand() % (room_length - 30) - (room_length - 30) / 2;
			teapothead[i].z = 30 - room_length / 2;
		}
		else {
			teapothead[i].x = 30 - room_length / 2;
			teapothead[i].z = rand() % (room_length - 30) - (room_length - 30) / 2;
		}
		teapothead[i].red = (double)rand() / RAND_MAX;
		teapothead[i].green = (double)rand() / RAND_MAX;
		teapothead[i].blue = (double)rand() / RAND_MAX;
		teapothead[i].x_velocity = rand() % 100 - 50;
		teapothead[i].y_velocity = rand() % 100 - 50;
		teapothead[i].z_velocity = rand() % 100 - 50;
	}
}

void playerShoot(Bullet & bullet) {
	bullet.x = player_x + cos(rotation) * 5;
	bullet.y = player_y + look_up_down * 5;
	bullet.z = player_z + sin(rotation) * 5;
	bullet.x_velocity = cos(rotation) * 1200;
	bullet.y_velocity = look_up_down * 1200;
	bullet.z_velocity = sin(rotation) * 1200;
}

void teapotheadShoot(Teapothead & teapothead, Bullet & bullet) {
	double x_distance = player_x - teapothead.x;
	double y_distance = player_y - teapothead.y;
	double z_distance = player_z - teapothead.z;
	double distance = fabs(x_distance) + fabs(y_distance) + fabs(z_distance);
	bullet.x = teapothead.x;
	bullet.y = teapothead.y;
	bullet.z = teapothead.z;
	bullet.x_velocity = 500 * x_distance / distance;
	bullet.y_velocity = 500 * y_distance / distance;
	bullet.z_velocity = 500 * z_distance / distance;
}

bool edgeCollision(Bullet bullet) {
	int boundary = room_length / 2;
	return (bullet.x < -boundary || bullet.x > boundary
		|| bullet.y < -boundary || bullet.y > boundary
		|| bullet.z < -boundary || bullet.z > boundary);
}

void bulletCollision(void) {
	for (int i = 0; i < number_of_teapothead_bullets; i++) {
		for (int j = 0; j < number_of_player_bullets; j++) {
			double x_distance = (teapothead_bullet[i].x - player_bullet[j].x) * (teapothead_bullet[i].x - player_bullet[j].x);
			double y_distance = (teapothead_bullet[i].y - player_bullet[j].y) * (teapothead_bullet[i].y - player_bullet[j].y);
			double z_distance = (teapothead_bullet[i].z - player_bullet[j].z) * (teapothead_bullet[i].z - player_bullet[j].z);
			if (x_distance + y_distance + z_distance <= 4) {
				number_of_teapothead_bullets--;
				teapothead_bullet[i] = teapothead_bullet[number_of_teapothead_bullets];
				number_of_player_bullets--;
				player_bullet[j] = player_bullet[number_of_player_bullets];
				break;
			}
		}
	}
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_COLOR_MATERIAL);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(player_x, player_y, player_z, player_x + cos(rotation), player_y + look_up_down, player_z + sin(rotation), 0, 1, 0);

	glPushMatrix();//wall
	glColor3f(0.5, 0.5, 0);
	glTranslated(room_length, 0, 0);
	glutSolidCube(room_length);
	glTranslated(-room_length * 2, 0, 0);
	glutSolidCube(room_length);
	glPopMatrix();

	glPushMatrix();//floor
	glTranslated(0, -(room_length / 2 + 50), 0);
	glColor3f(0.5, 0.5, 0.5);
	glutSolidCube(room_length);
	glPopMatrix();

	glPushMatrix();//wall
	glColor3f(0.5, 0, 0.5);
	glTranslated(0, 0, room_length);
	glutSolidCube(room_length);
	glTranslated(-0 * 2, 0, -room_length * 2);
	glutSolidCube(room_length);
	glPopMatrix();

	glPushMatrix();//health bar
	glColor3f(0, 1, 0);
	glTranslated(player_x + cos(rotation) * 10, player_y + look_up_down * 10 - 2, player_z + sin(rotation) * 10);
	glRotated(rotation * 57.29577951, 0, -1, 0);
	glScaled(0.01, 0.1, 10);
	glutSolidCube(1.5);
	if (injured) {
		glTranslated(0, 0, 0.7);
		glColor3f(1, 0, 0);
		glutSolidCube(1.51);
	}
	glPopMatrix();

	if (dead) {
		glPushMatrix();
		glTranslated(player_x, player_y, player_z);
		glColor3f(0, 0, 0);
		glutSolidCube(10);
		glPopMatrix();
	}

	for (int i = 0; i < number_of_teapotheads; i++) {
		glPushMatrix();
		glTranslated(teapothead[i].x, teapothead[i].y, teapothead[i].z);
		glColor3f(teapothead[i].red, teapothead[i].green, teapothead[i].blue);
		glutSolidTeapot(10);
		glTranslated(0, -50, 0);
		glRotatef(270, 1, 0, 0);
		glutSolidCone(10, 50, 3, 1);
		glPopMatrix();

		teapothead[i].x += teapothead[i].x_velocity * t;
		teapothead[i].y += teapothead[i].y_velocity * t * flying;
		teapothead[i].z += teapothead[i].z_velocity * t;

		if (number_of_teapothead_bullets < max_bullet && rand() % (number_of_teapotheads * 10) == i) {
			teapotheadShoot(teapothead[i], teapothead_bullet[number_of_teapothead_bullets]);
			number_of_teapothead_bullets++;
		}

		if (teapothead[i].x < -room_length / 2 + 20 || teapothead[i].x > room_length / 2 - 20) {
			teapothead[i].x_velocity = -teapothead[i].x_velocity;
		}
		if (teapothead[i].y < 0 || teapothead[i].y > room_length / 2) {
			teapothead[i].y_velocity = -teapothead[i].y_velocity;
		}
		if (teapothead[i].z < -room_length / 2 + 20 || teapothead[i].z > room_length / 2 - 20) {
			teapothead[i].z_velocity = -teapothead[i].z_velocity;
		}
	}

	bulletCollision();

	for (int i = 0; i < number_of_player_bullets; i++) {
		glPushMatrix();
		glTranslated(player_bullet[i].x, player_bullet[i].y, player_bullet[i].z);
		glColor3f(1, 1, 1);
		glutSolidSphere(1, 20, 20);
		glPopMatrix();

		player_bullet[i].x += player_bullet[i].x_velocity * t;
		player_bullet[i].y += player_bullet[i].y_velocity * t;
		player_bullet[i].z += player_bullet[i].z_velocity * t;

		for (int j = 0; j < number_of_teapotheads; j++) {
			if (player_bullet[i].x < teapothead[j].x + 10 && player_bullet[i].x > teapothead[j].x - 10
				&& player_bullet[i].y < teapothead[j].y + 10 && player_bullet[i].y > teapothead[j].y - 50
				&& player_bullet[i].z < teapothead[j].z + 10 && player_bullet[i].z > teapothead[j].z - 10) {
				number_of_player_bullets--;
				player_bullet[i] = player_bullet[number_of_player_bullets];
				number_of_teapotheads--;
				teapothead[j] = teapothead[number_of_teapotheads];
				if (number_of_teapotheads == 0) {
					createTeapothead();
					flying = 1;
				}
				break;
			}
		}

		if (edgeCollision(player_bullet[i])) {
			number_of_player_bullets--;
			player_bullet[i] = player_bullet[number_of_player_bullets];
			break;
		}
	}

	bulletCollision();

	for (int i = 0; i < number_of_teapothead_bullets; i++) {
		glPushMatrix();
		glTranslated(teapothead_bullet[i].x, teapothead_bullet[i].y, teapothead_bullet[i].z);
		glColor3f(0, 0, 0);
		glutSolidSphere(1, 20, 20);
		glPopMatrix();

		teapothead_bullet[i].x += teapothead_bullet[i].x_velocity * t;
		teapothead_bullet[i].y += teapothead_bullet[i].y_velocity * t;
		teapothead_bullet[i].z += teapothead_bullet[i].z_velocity * t;

		if (player_x - 5 < teapothead_bullet[i].x && player_x + 5 > teapothead_bullet[i].x
			&& player_y - 5 < teapothead_bullet[i].y && player_y + 5 > teapothead_bullet[i].y
			&& player_z - 5 < teapothead_bullet[i].z && player_z + 5 > teapothead_bullet[i].z) {
			number_of_teapothead_bullets--;
			teapothead_bullet[i] = teapothead_bullet[number_of_teapothead_bullets];
			if (injured) {
				dead = true;
			}
			else {
				injured = true;
			}
			break;
		}

		if (edgeCollision(teapothead_bullet[i])) {
			number_of_teapothead_bullets--;
			teapothead_bullet[i] = teapothead_bullet[number_of_teapothead_bullets];
			break;
		}
	}

	bulletCollision();

	player_y += player_y_velocity * t;
	if (player_y > 0) {
		player_y_velocity -= 980 * t;
	}
	else {
		player_y = 0;
	}

	if (number_of_player_bullets >= max_bullet) {
		glutSetCursor(GLUT_CURSOR_WAIT);
	}
	else {
		glutSetCursor(GLUT_CURSOR_CROSSHAIR);
	}
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

void keyboard(unsigned char key, int x, int y) {
	double cos_rotation = cos(rotation) * 2;
	double sin_rotation = sin(rotation) * 2;
	int boundary = room_length / 2;
	switch (key) {
	case 'w':
		player_x += cos_rotation;
		player_z += sin_rotation;
		if (player_x - 5 < -boundary || player_x + 5 > boundary) {
			player_x -= cos_rotation;
		}
		if (player_z - 5 < -boundary || player_z + 5 > boundary) {
			player_z -= sin_rotation;
		}
		break;
	case 'a':
		player_x += sin_rotation;
		player_z -= cos_rotation;
		if (player_x - 5 < -boundary || player_x + 5 > boundary) {
			player_x -= sin_rotation;
		}
		if (player_z - 5 < -boundary || player_z + 5 > boundary) {
			player_z += cos_rotation;
		}
		break;
	case 's':
		player_x -= cos_rotation;
		player_z -= sin_rotation;
		if (player_x - 5 < -boundary || player_x + 5 > boundary) {
			player_x += cos_rotation;
		}
		if (player_z - 5 < -boundary || player_z + 5 > boundary) {
			player_z += sin_rotation;
		}
		break;
	case 'd':
		player_x -= sin_rotation;
		player_z += cos_rotation;
		if (player_x - 5 < -boundary || player_x + 5 > boundary) {
			player_x += sin_rotation;
		}
		if (player_z - 5 < -boundary || player_z + 5 > boundary) {
			player_z -= cos_rotation;
		}
		break;
	case ' ':
		if (player_y == 0) {
			player_y_velocity = 500;
		}
		break;
	}
	glutPostRedisplay();
}

void mouseButton(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && number_of_player_bullets < max_bullet) {
		playerShoot(player_bullet[number_of_player_bullets]);
		number_of_player_bullets++;
		alSourcePlay(source1);
	}
}

void mouseMove(int x, int y) {
	if (reset_Mouse == false) {
		double distance_x = x - mouse_last_x;
		double distance_y = y - mouse_last_y;
		mouse_last_x = x;
		mouse_last_y = y;
		rotation += distance_x * 0.001;
		look_up_down -= distance_y * 0.001;

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
	cout << "number of teapotheads: ";
	cin >> input_number_of_teapotheads;
	createTeapothead();
	glutInit(&argc, argv);
	alutInit(&argc, argv);
	ALuint buffer1 = alutCreateBufferFromFile("gunshot.wav");
	alGenSources(1, &source1);
	alSourcei(source1, AL_BUFFER, buffer1);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("Project #5");
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

	glutMainLoop();
	return 0;
}