#pragma once

#include <vector>

#include "utils.h"
#include "terrain/Chunks.h"

enum AnimalState { DEFAULT, HAD_CHILD, DEAD };

struct Animal
{
	float max_speed = 1;
	float size = .5;

	vec2d vel;
	vec2d acc;
	vec2d pos;
	float friction = 1;

	unsigned int cycle_counter = 0;
	unsigned int cycle_limit = 0;

	float max_hunger = 10000;
	float hunger = max_hunger;

	float max_health = 100;
	float health = max_health;
	float strength = 50;

	float see_distance = 10; // How far can be seen in front (meters)
	float fov = 10; // Field of view in degrees
	vec2d look_dir; // direction being looked at

	bool is_male = true;
	int pregnant = -1;
	int pregnancy_time = 100;
	int horny = 0;
	int horny_threshold = 100;

	unsigned int age = 0;

	float max_energy = 100;
	float max_energy_discout = .1; // Multiply by age and subtract result from max energy.
	float min_energy = 10;
	float energy = max_energy;

	float getHealth();
	void hurt(float damage);
	void updade_vel();
	virtual AnimalState update(Chunk* neighbors[], std::vector<Animal*> animals);
	virtual ~Animal() = default;
};

class Squirrel : public Animal
{
public:
	bool on_tree = false;
	Squirrel(vec2d pos);
	void give_pregnancy();
	AnimalState update(Chunk* neighbors[], std::vector<Animal*> animals);
};

class Cat : public Animal
{
public:
	Cat(vec2d pos);
	AnimalState update(Chunk* neighbors[], std::vector<Animal*> animals);
};

class Shark : public Animal
{
public:
	Shark(vec2d pos);
};

