#ifndef PLANET_H_INCLUDED
#define PLANET_H_INCLUDED 1
#pragma once

#include "animation.h"
#include "vec_math.h"


void LoadTexImage(char *FileName);

class planet : public object 
{
private:
	
public:
	vec coord, trans;
	viewport_t view;
	double radius, mass;
	static planet instance;
	planet();
	void draw(vector<object*> stack);
	planet(double radius, double mass, vec coord);
};

#endif
