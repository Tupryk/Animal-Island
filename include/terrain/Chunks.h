#pragma once

#include "utils.h"

enum ChunkTypes { LAND, SEA };

struct Chunk
{
	ChunkTypes type;

	Chunk(ChunkTypes type = ChunkTypes::SEA);
};

struct Terrain
{
	static const unsigned int dimensions = 64;
	Chunk chunks[dimensions][dimensions];

	Terrain();
};
