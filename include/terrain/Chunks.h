#pragma once

#include <vector>

#include "utils.h"
#include "beings/Plants.h"

enum ChunkTypes { GRASS, VALLEY, SEA, SAND };

struct Chunk
{
	ChunkTypes type;
	unsigned int size = 64;
	std::vector<Tree> trees;

	Chunk(ChunkTypes type = ChunkTypes::SEA);
	void update();
};

struct Terrain
{
	static const unsigned int dimensions = 100;
	Chunk chunks[dimensions][dimensions];

	Terrain();
};
