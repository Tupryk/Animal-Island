#pragma once

#include "utils.h"

class Cat
{
	float max_speed = 1;
	vec2d velocity;

public:
	vec2d pos;

	Cat(vec2d pos);
	void update();
};
