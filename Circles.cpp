#include <iostream>
#include <GL/glut.h>
#include <cmath>
#include "Circles.h"

#define M_PI 3.14159265358979323846

using namespace std;

void Circles::setPixel(int x, int y)
{
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	glVertex2f(x, y);
	glEnd();
}

void Circles::ParametricEquation(int cx, int cy, int r)
{
	float step = 1.0 / 2;
	for (float t = 0; t < 360; t += step)
	{
		float rad = t / 180 * M_PI;

		int x = cx + r * cos(rad);
		int y = cy + r * sin(rad);

		Circles::setPixel(x, y);
	}
}