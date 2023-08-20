#include "beings/Plants.h"

Tree::Tree(vec2d pos, TreeState state) : visual(), pos(pos) {
	this->state = state;
	health = max_health*.5+(rand()%static_cast<int>(max_health*.5));
}

void Tree::update()
{
	if (health <= 0) return;
	cycle += 1;
	if (cycle >= cycle_limit) {
		cycle = 0;
		if (state == TreeState::BIG) { drop_seeds(); health--; }
		else if (state == TreeState::SMALL) state = TreeState::BIG;
		else if (state == TreeState::SEED) {
			if (rand()%2) state = TreeState::SMALL;
			else health = 0;
		}
	}
}

std::vector<Tree> Tree::drop_seeds()
{
	std::vector<Tree> children;
	unsigned int seed_count = rand()%3 + 1;
	for (int i = 0; i < seed_count; i++) {
		vec2d child_pos(pos.x + rand()%10, pos.y + rand()%10);
		children.push_back(Tree(child_pos, TreeState::SEED));
	}
	return children;
}

int Tree::getHealth() { return health; }

bool Tree::adopt(std::shared_ptr<Animal> animal) {
	if (max_capacity >= animals.size()) return false;
	animals.push_back(animal);
	return true;
}

unsigned int Tree::getAnimalPopulation() { return animals.size(); }
