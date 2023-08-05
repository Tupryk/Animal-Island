#include "beings/Plants.h"

void Tree::update()
{
	cycle += 1;
	if (cycle >= cycle_limit) {
		cycle = 0;
		if (state == TreeStates::BIG) drop_seeds();
		else if (state == TreeStates::SMALL) state = TreeState::BIG;
		else if (state == TreeStates::SEED) state = TreeState::SMALL;
	}
}

void Tree::drop_seeds()
{
	unsigned int seed_count = rand()%3 + 4;
	std::cout << "Dropped " << seed_count << " seeds." << std::endl;
}
