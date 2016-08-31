#ifndef COORD_AXIS_H_INCLUDED
#define COORD_AXIS_H_INCLUDED
#pragma once

#include "animation.h"

class axis : public object //Пример объекта система координат
{
public:
	double radius;
	void draw(vector<object*> stack)
	{
#if USE_SHADERS
		setShaders(shaders, "shaders/default_vert_shader.glsl", "shaders/default_frag_shader.glsl");
		glUseProgram(shaders);
#endif
		glBegin(GL_LINES);

		glColor3d(1, 0, 0);
		glVertex3d(0, 0, 0);
		glVertex4d(1, 0, 0, 0);

		glColor3d(0, 1, 0);
		glVertex3d(0, 0, 0);
		glVertex4d(0, 1, 0, 0);

		glColor3d(0, 0, 1);
		glVertex3d(0, 0, 0);
		glVertex4d(0, 0, 1, 0);

		glEnd();
	}
	axis()
	{
	}
};

#endif