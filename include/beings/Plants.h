#pragma once

#include "utils.h"

enum TreeState { SEED, SMALL, BIG };

class Tree
{
	unsigned int cycle = 0;
	unsigned int cycle_limit = 256; // After this many days the tree drops nuts.

	int max_health = 100;
	int health = max_health;

	std::vector<Tree> drop_seeds();

public:
	TreeState state;
	vec2d pos;

	Tree(vec2d pos, TreeState state=TreeState::BIG);
	void update();
	int getHealth();
};
