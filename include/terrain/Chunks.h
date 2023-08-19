#pragma once

#include <vector>

#include "utils.h"
#include "beings/Plants.h"
#include "beings/Animals.h"

enum ChunkTypes { GRASS, VALLEY, SEA, SAND };

struct Animal;

struct Chunk
{
	ChunkTypes type;
	vec2d coor;
	unsigned int size = 64;
	Chunk* neighbors[9]; // 1: top left, 2: top center, ..., 4: center center, ..., 5: center right, ... 8: bottom right.

	std::vector<Tree> trees;
	std::list<std::shared_ptr<Animal>> animals;

	Chunk(ChunkTypes type = ChunkTypes::SEA, vec2d coor = {0, 0});
	void update();
};
