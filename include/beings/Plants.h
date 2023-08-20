#pragma once

#include <vector>

#include "utils.h"
#include "visuals/Plants.h"
#include "beings/Animals.h"
#include "Physics.h"

enum TreeState { SEED, SMALL, BIG };

struct Animal;

class Tree : public StaticBody
{
	unsigned int cycle = 0;
	unsigned int cycle_limit = 256; // After this many days the tree drops nuts.

	int max_health = 100;
	int health = max_health;

	unsigned int max_capacity = 5; // How many animals can hop on (maybe make it relative to size and weight of animals in the future)
	unsigned int load = 0;

	std::vector<Tree> drop_seeds();

public:
	TreeVisual visual;
	TreeState state;

	Tree(vec2d pos, TreeState state=TreeState::BIG);
	void update();
	int getHealth();
	void adopt(float size);
	void drop(float size);
	bool fits(float size);
	unsigned int getAnimalPopulation();
};
