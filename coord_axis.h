#ifndef COORD_AXIS_H_INCLUDED
#define COORD_AXIS_H_INCLUDED
#pragma once

#include "animation.h"

class axis : public object //Пример объекта система координат
{
public:
	void draw(vector<object*> stack)
	{
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