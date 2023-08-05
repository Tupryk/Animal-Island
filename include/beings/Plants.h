#pragma once

#include "utils.h"

enum TreeState { SEED, SMALL, BIG };

class Tree
{
	unsigned int cycle = 0;
	unsigned int cycle_limit = 256; // After this many days the tree drops nuts.

	void drop_seeds();

public:
	TreeState state;
	vec2d pos;

	Tree(vec2d pos);
	void update();
};
