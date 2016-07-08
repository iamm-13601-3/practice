#pragma once
#include "sphere.h"

class planet : public sphere
{
	public:
		double mass;
		planet(double radius, double mass) : sphere(radius)
		{
			this->mass = mass;
			this->type = PLANET;
		}
};