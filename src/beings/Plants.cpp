#include "beings/Plants.h"

Tree::Tree(vec2d pos) : pos(pos) { state = TreeState::BIG; }

void Tree::update()
{
	cycle += 1;
	if (cycle >= cycle_limit) {
		cycle = 0;
		if (state == TreeState::BIG) drop_seeds();
		else if (state == TreeState::SMALL) state = TreeState::BIG;
		else if (state == TreeState::SEED) state = TreeState::SMALL;
	}
}

void Tree::drop_seeds()
{
	// unsigned int seed_count = rand()%3 + 4;
}
