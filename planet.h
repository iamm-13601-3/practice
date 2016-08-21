#ifndef PLANET_H_INCLUDED
#define PLANET_H_INCLUDED
#pragma once

#include "animation.h"

class planet : public object 
{
private:
	
public:
	double radius, mass;
	void draw(vector<object*> stack);
	planet(double radius, double mass);
};

#endif
