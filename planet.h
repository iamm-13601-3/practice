#ifndef PLANET_H_INCLUDED
#define PLANET_H_INCLUDED
#pragma once

#include "animation.h"
#include "vec_math.h"

class planet : public object 
{
private:
	
public:
	vec coord, trans;
	viewport_t view;
	double radius, mass;
	AUX_RGBImageRec* photo_image;
	void draw(vector<object*> stack);
	planet(double radius, double mass, vec coord);
};

#endif
