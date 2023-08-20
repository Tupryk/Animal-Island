#pragma once

#include <list>
#include <vector>
#include <memory>
#include <algorithm>

#include "utils.h"
#include "terrain/Chunks.h"

enum AnimalState { DEFAULT, HAD_CHILD, DEAD };

struct Chunk;

struct Animal
{
	float max_speed = 1;
	float size = .5;

	vec2d vel;
	vec2d acc;
	vec2d pos;
	float friction = .5;

	unsigned int cycle_counter = 0;
	unsigned int cycle_limit = 0;

	float max_hunger = 5000;
	float hunger = max_hunger;

	float max_health = 100;
	float health = max_health;
	float strength = 100;

	float see_distance = 10; // How far can be seen in front (meters)
	float fov = 120; // Field of view in degrees
	vec2d look_dir; // direction being looked at

	bool is_male = true;
	int pregnant = -1;
	int pregnancy_time = 100;
	int lust = 0;
	int lust_threshold = 100;

	unsigned int age = 0;
	unsigned int max_age = 10000;

	float max_energy = 100;
	float max_energy_discout = .1; // Multiply by age and subtract result from max energy.
	float min_energy = 10;
	float energy = max_energy;

	unsigned int over_population_thresh = 64; // If there are this many animals of the same species close to the animal it dies.

	float reach = 10;

	float getHealth();
	void hurt(float damage);
	void wander();
	AnimalState update_basic();
	virtual AnimalState update(Chunk* neighbors[], std::list<std::shared_ptr<Animal>> animals);
	virtual ~Animal() = default;
};

class Squirrel : public Animal
{
public:
	bool on_tree = false;
	Squirrel(vec2d pos);
	void give_pregnancy();
	AnimalState update(Chunk* neighbors[], std::list<std::shared_ptr<Animal>> animals);
};

class Cat : public Animal
{
public:
	Cat(vec2d pos);
	AnimalState update(Chunk* neighbors[], std::list<std::shared_ptr<Animal>> animals);
};

class Shark : public Animal
{
public:
	Shark(vec2d pos);
};
