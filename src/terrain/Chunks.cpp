#include "terrain/Chunks.h"

Chunk::Chunk(ChunkTypes type, vec2d coor) : type(type), coor(coor)
{
	if (type == ChunkTypes::VALLEY) {
		unsigned int trees_count = rand()%3;

		for (int i = 0; i < trees_count; i++)
		{
			vec2d tpos = vec2d(rand()%size, rand()%size) + coor;
			std::shared_ptr<Tree> tree = std::make_shared<Tree>(tpos);
			trees.push_back(tree);
		}
	}
}

void Chunk::update()
{
	for (int i = 0; i < trees.size(); i++) {
		if (trees[i]->getHealth() <= 0) trees.erase(trees.begin() + i);
		else trees[i]->update();
	}
}
