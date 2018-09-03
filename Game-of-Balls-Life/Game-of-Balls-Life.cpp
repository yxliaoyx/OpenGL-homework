#include <GL/glut.h>
#include <iostream>
#include <time.h>
using namespace std;

#define max_number 100
#define t 0.01

enum Color { red, green, blue };
int window_width = 1280, window_height = 720, box_half_length = 20;
int number_of_boxes = 10;
int number_of_balls[3];
int enable_boxAI = 0;
double food_sources_count = 0;

struct Actor {
	GLdouble x, y, x_velocity = 0, y_velocity = 0, radius;
	Color color;
	int target = 0;
	bool stop = false;
	double hungry_count = 0;
};

Actor box[max_number];
Actor red_ball[max_number];
Actor green_ball[max_number];
Actor blue_ball[max_number];

void createBox(Actor & box) {
	box.x = rand() % (window_width - 150) - (window_width - 150) / 2;
	box.y = rand() % (window_height - 150) - (window_height - 150) / 2;
	box.x_velocity = rand() % 200 - 100;
	box.y_velocity = rand() % 200 - 100;
}

void createBall(Actor & ball, Color color) {
	int random_number = rand() % 4;
	switch (random_number) {
	case 0:
		ball.x = rand() % window_width - window_width / 2;
		ball.y = 30 - window_height / 2;
		break;
	case 1:
		ball.x = rand() % window_width - window_width / 2;
		ball.y = window_height / 2 - 30;
		break;
	case 2:
		ball.x = 30 - window_width / 2;
		ball.y = rand() % window_height - window_height / 2;
		break;
	default:
		ball.x = window_width / 2 - 30;
		ball.y = rand() % window_height - window_height / 2;
		break;
	}
	ball.color = color;
	ball.radius = 20;
	if (color == blue) {
		ball.radius = 10;
	}
}

void drawBox(Actor box) {
	glPushMatrix();
	glTranslated(box.x, box.y, 0);
	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
	glVertex3f(box_half_length, box_half_length, 0.0);
	glVertex3f(box_half_length, -box_half_length, 0.0);
	glVertex3f(-box_half_length, -box_half_length, 0.0);
	glVertex3f(-box_half_length, box_half_length, 0.0);
	glEnd();
	glPopMatrix();
}

void drawBall(Actor ball) {
	glPushMatrix();
	glTranslated(ball.x, ball.y, 0);
	if (ball.color == red) {
		glColor3f(1, 0, 0);
	}
	if (ball.color == green) {
		glColor3f(0, 1, 0);
	}
	if (ball.color == blue) {
		glColor3f(0, 0, 1);
	}
	glBegin(GL_POLYGON);
	for (int i = 0; i < 30; ++i) {
		glVertex2f(ball.radius * cos(i * 2 * 3.14 / 30), ball.radius * sin(i * 2 * 3.14 / 30));
	}
	glEnd();
	glPopMatrix();
}

void init(void) {
	srand(time(NULL));
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	cout << "number_of_boxes: ";
	cin >> number_of_boxes;
	cout << "number_of_balls[red]: ";
	cin >> number_of_balls[red];
	cout << "number_of_balls[green]: ";
	cin >> number_of_balls[green];
	cout << "number_of_balls[blue]: ";
	cin >> number_of_balls[blue];
	cout << "enable boxAI? ";
	cin >> enable_boxAI;

	for (int i = 0; i < number_of_boxes; i++) {
		createBox(box[i]);
	}
	for (int i = 0; i < number_of_balls[red]; i++) {
		createBall(red_ball[i], red);
	}
	for (int i = 0; i < number_of_balls[green]; i++) {
		createBall(green_ball[i], green);
	}
	for (int i = 0; i < number_of_balls[blue]; i++) {
		createBall(blue_ball[i], blue);
	}
}

void changeTarget(Actor & ball) {
	int number_of_enemies;
	if (ball.color == red) {
		number_of_enemies = number_of_balls[green];
	}
	else if (ball.color == green) {
		number_of_enemies = number_of_balls[red];
	}
	if (number_of_enemies) {
		ball.stop = false;
		ball.target += 1;
		if (ball.target >= number_of_enemies) {
			ball.target = 0;
		}
	}
}

void eliminate(Actor & ball) {
	number_of_balls[ball.color] -= 1;
	if (ball.color == red) {
		ball = red_ball[number_of_balls[red]];
	}
	else if (ball.color == green) {
		ball = green_ball[number_of_balls[green]];
	}
	else if (ball.color == blue) {
		ball = blue_ball[number_of_balls[blue]];
	}
}

void predation(Actor & predator, Actor & prey) {
	predator.radius = sqrt(predator.radius * predator.radius + prey.radius * prey.radius);
	eliminate(prey);
	predator.hungry_count = 0;
}

void split(Actor & ball) {
	if (ball.radius < 15) {
		eliminate(ball);
	}
	else {
		ball.hungry_count = 0;
		ball.radius = sqrt(ball.radius * ball.radius / 2);
		if (ball.color == red && number_of_balls[red] < max_number) {
			red_ball[number_of_balls[red]] = ball;
			number_of_balls[red] += 1;
		}
		else if (ball.color == green && number_of_balls[green] < max_number) {
			green_ball[number_of_balls[green]] = ball;
			number_of_balls[green] += 1;
		}
		changeTarget(ball);
	}
}

void goAhead(Actor & ball) {
	ball.x += ball.x_velocity * t;
	ball.y += ball.y_velocity * t;
}

void goBack(Actor & ball) {
	ball.x -= ball.x_velocity * t;
	ball.y -= ball.y_velocity * t;
	ball.x_velocity = 0;
	ball.y_velocity = 0;
}

void redGreenCollision(void) {
	for (int i = 0; i < number_of_balls[red]; i++) {
		for (int j = 0; j < number_of_balls[green]; j++) {
			double Manhattan_distance = abs(red_ball[i].x - green_ball[j].x) + abs(red_ball[i].y - green_ball[j].y);
			if (Manhattan_distance < red_ball[i].radius + green_ball[j].radius) {
				changeTarget(red_ball[i]);
				changeTarget(green_ball[j]);
				if (red_ball[i].radius > green_ball[j].radius) {
					predation(red_ball[i], green_ball[j]);
				}
				else if (red_ball[i].radius < green_ball[j].radius) {
					predation(green_ball[j], red_ball[i]);
				}
			}
		}
	}
}

void foodSources(void) {
	food_sources_count += t;
	if (food_sources_count > 10) {
		food_sources_count = 0;
		for (int i = 0; i < 5 && number_of_balls[blue] < max_number; i++, number_of_balls[blue]++) {
			createBall(blue_ball[number_of_balls[blue]], blue);
			blue_ball[number_of_balls[blue]].x = rand() % window_width - window_width / 2;
			blue_ball[number_of_balls[blue]].y = rand() % window_height - window_height / 2;
		}
	}
}

void sceneCollision(Actor & ball) {
	double ball_left = ball.x - ball.radius;
	double ball_right = ball.x + ball.radius;
	double ball_bottom = ball.y - ball.radius;
	double ball_top = ball.y + ball.radius;

	if (ball_left <= -window_width / 2 ||
		ball_right >= window_width / 2 ||
		ball_bottom <= -window_height / 2 ||
		ball_top >= window_height / 2) {
		goBack(ball);
		ball.stop = true;
	}

	for (int i = 0; i < number_of_boxes; i++) {
		if (ball_left <= box[i].x + box_half_length &&
			ball_right >= box[i].x - box_half_length &&
			ball_bottom <= box[i].y + box_half_length &&
			ball_top >= box[i].y - box_half_length) {
			goBack(ball);
			ball.stop = true;
		}
	}

	for (int i = 0; i < number_of_balls[blue]; i++) {
		double Manhattan_distance = abs(ball.x - blue_ball[i].x) + abs(ball.y - blue_ball[i].y);
		if (Manhattan_distance < ball.radius + blue_ball[i].radius) {
			predation(ball, blue_ball[i]);
		}
	}
}

void boxAI(Actor & box) {
	goAhead(box);
	if (box.x - box_half_length <= -window_width / 2 ||
		box.x + box_half_length >= window_width / 2) {
		box.x_velocity = -box.x_velocity;
	}
	if (box.y - box_half_length <= -window_height / 2 ||
		box.y + box_half_length >= window_height / 2) {
		box.y_velocity = -box.y_velocity;
	}
}

void ballAI(Actor & ball) {
	if (ball.stop) {
		changeTarget(ball);
	}
	else {
		Actor target;
		if (ball.color == red) {
			target = green_ball[ball.target];
		}
		else if (ball.color == green) {
			target = red_ball[ball.target];
		}
		double x_distance = ball.x - target.x;
		double y_distance = ball.y - target.y;
		double Manhattan_distance = abs(x_distance) + abs(y_distance);
		if (Manhattan_distance) {
			ball.x_velocity = 200 * x_distance / Manhattan_distance;
			ball.y_velocity = 200 * y_distance / Manhattan_distance;
			if (ball.radius > target.radius) {
				ball.x_velocity = -ball.x_velocity;
				ball.y_velocity = -ball.y_velocity;
			}
		}
		goAhead(ball);
	}
	if (ball.hungry_count > 5) {
		split(ball);
	}
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_COLOR_MATERIAL);

	for (int i = 0; i < number_of_boxes; i++) {
		drawBox(box[i]);
	}
	for (int i = 0; i < number_of_balls[red]; i++) {
		drawBall(red_ball[i]);
	}
	for (int i = 0; i < number_of_balls[green]; i++) {
		drawBall(green_ball[i]);
	}
	for (int i = 0; i < number_of_balls[blue]; i++) {
		drawBall(blue_ball[i]);
	}

	if (enable_boxAI) {
		for (int i = 0; i < number_of_boxes; i++) {
			boxAI(box[i]);
		}
	}

	redGreenCollision();
	foodSources();

	for (int i = 0; i < number_of_balls[red]; i++) {
		red_ball[i].hungry_count += t;
		ballAI(red_ball[i]);
		sceneCollision(red_ball[i]);
	}
	for (int i = 0; i < number_of_balls[green]; i++) {
		green_ball[i].hungry_count += t;
		ballAI(green_ball[i]);
		sceneCollision(green_ball[i]);
	}

	glFlush();
}

void Timer(int c) {
	glutPostRedisplay();
	glutTimerFunc(10, Timer, 0);
}

void reshape(int window_width, int window_height) {
	glViewport(0, 0, window_width, window_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-window_width / 2, window_width / 2, -window_height / 2, window_height / 2);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("Project #2");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(10, Timer, 0);
	glutMainLoop();
	return 0;
}
