#pragma once

#include <vector>

#include "utils.h"
#include "terrain/Chunks.h"

class Squirrel
{
	float max_speed = 1;
	// float size = .2;
	vec2d velocity;
	unsigned int cycle_counter = 0;
	unsigned int cycle_limit = 0;

public:
	vec2d pos;

	Squirrel(vec2d pos);
	void update(Chunk* neighbors[]);
};

class Cat
{
	float max_speed = 1;
	// float size = .5;
	vec2d velocity;
	unsigned int cycle_counter = 0;
	unsigned int cycle_limit = 0;

public:
	vec2d pos;

	Cat(vec2d pos);
	void update(Chunk* neighbors[], std::vector<Squirrel> squirrels);
};
