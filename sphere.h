#ifndef SPHERE_H_INCLUDED
#define SPHERE_H_INCLUDED
#pragma once

#include "animation.h"

class sphere : public object //Пример объекта сфера 
{
public:
	double radius;
	void draw(vector<object*> stack)
	{
		glColor3d(color.x, color.y, color.z);
		glutSolidSphere(radius, 10, 10);
	}
	sphere(double radius)
	{
		this->radius = radius;
	}
};

#endif
