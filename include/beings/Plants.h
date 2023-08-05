#pragma once

enum TreeStates { SEED, SMALL, BIG };

class Tree
{
	unsigned int cycle = 0;
	unsigned int cycle_limit = 256; // After this many timesteps the tree drops nuts.
	unsigned int age = 0;
	unsigned int state_transition = 16; // Cycles until growth.
	TreeStates state;

	void drop_seeds();

public:
	void update();
};
