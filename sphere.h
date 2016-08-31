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
#if USE_SHADERS
		setShaders(shaders, "shaders/default_vert_shader.glsl", "shaders/default_frag_shader.glsl");
		glUseProgram(shaders);
#endif
		glColor3d(color.x, color.y, color.z);
		glutSolidSphere(radius, 100, 100);
	}
	sphere(double radius)
	{
		this->radius = radius;
	}
};
#endif
