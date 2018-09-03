#include "GLInterface.h"
#include "GLSLSetup.h"

#include <GL/glew.h>
#include <GL/glut.h>

#include <stdio.h>

#pragma comment(lib, "glew32.lib")

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(960, 720);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow("Project #1");

	glutDisplayFunc(Display);
	glutKeyboardFunc(KeyboardDown);
	glutSpecialFunc(SpecialKey);
	glutTimerFunc(50, Timer, 0);

	glewInit();
	if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader) {
		printf("Ready for GLSL\n");
	}
	else {
		printf("No GLSL support\n");
		exit(1);
	}

	SetGLShaderLanguage("Gouraud.vert", "Gouraud.frag");
	SetGLShaderLanguage("Phong.vert", "Phong.frag");
	glutMainLoop();
}