#ifndef PLANET_H_INCLUDED
#define PLANET_H_INCLUDED 1
#pragma once

#include "animation.h"



void LoadTexImage(char *FileName);

class planet : public object 
{
private:
	
public:
	double radius, mass;
	static planet instance;
	planet();
	void draw(vector<object*> stack);
	planet(double radius, double mass);
};

#endif
