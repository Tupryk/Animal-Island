#include "terrain/Chunks.h"

Chunk::Chunk(ChunkTypes type) : type(type)
{
	if (type == ChunkTypes::VALLEY) {
		unsigned int trees_count = rand()%5;

		for (int i = 0; i < trees_count; i++)
		{
			float pos_x = rand()%size;
			float pos_y = rand()%size;

			trees.push_back(Tree({pos_x, pos_y}));
		}
	}
}

void Chunk::update()
{
	for (int i = 0; i < trees.size(); i++)
		trees[i].update();
}
