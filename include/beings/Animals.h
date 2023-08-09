#pragma once

#include <vector>

#include "utils.h"
#include "terrain/Chunks.h"

struct Animal
{
	float max_speed = 1;
	float size = .5;

	vec2d velocity;
	vec2d pos;

	unsigned int cycle_counter = 0;
	unsigned int cycle_limit = 0;

	float max_hunger = 1000;
	float hunger = max_hunger;

	float max_health = 100;
	float health = max_health;
	float strength = 50;

	float getHealth();
	void hurt(float damage);
};

class Squirrel : public Animal
{
public:
	Squirrel(vec2d pos);
	void update(Chunk* neighbors[]);
};

class Cat : public Animal
{
public:
	Cat(vec2d pos);
	void update(Chunk* neighbors[], std::vector<Squirrel*> squirrels);
};
