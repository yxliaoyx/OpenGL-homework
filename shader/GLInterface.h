#ifndef GL_INTERFACE_H
#define GL_INTERFACE_H

#include <GL/glew.h>

void Display();
void KeyboardDown(unsigned char c, int x, int y);
void KeyboardUp(unsigned char c, int x, int y);
void SpecialKey(GLint key, GLint x, GLint y);
void Timer(int c);

#endif