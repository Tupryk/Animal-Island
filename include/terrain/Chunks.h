#pragma once

#include <vector>

#include "utils.h"
#include "beings/Plants.h"
#include "beings/Animals.h"

enum ChunkTypes { GRASS, VALLEY, SEA, SAND };

struct Chunk
{
	ChunkTypes type;
	unsigned int size = 64;
	Chunk* neighbours[8]; // 1: top left, 2: top center, ... 4: center left, 5: center right, ... 8: bottom right.

	std::vector<Tree> trees;
	std::vector<Cat*> cats;

	Chunk(ChunkTypes type = ChunkTypes::SEA);
	void update();
};
