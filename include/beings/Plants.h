#pragma once

#include <vector>

#include "utils.h"
#include "visuals/Plants.h"
#include "beings/Animals.h"

enum TreeState { SEED, SMALL, BIG };

struct Animal;

class Tree
{
	unsigned int cycle = 0;
	unsigned int cycle_limit = 256; // After this many days the tree drops nuts.

	int max_health = 100;
	int health = max_health;

	unsigned int max_capacity = 5; // How many animals can hop on (maybe make it relative to size and weight of animals in the future)
	std::vector<std::shared_ptr<Animal>> animals;

	std::vector<Tree> drop_seeds();

public:
	TreeVisual visual;
	TreeState state;
	vec2d pos;

	Tree(vec2d pos, TreeState state=TreeState::BIG);
	void update();
	int getHealth();
	bool adopt(std::shared_ptr<Animal> animal);
	unsigned int getAnimalPopulation();
};
