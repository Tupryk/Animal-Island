#pragma once

#include <vector>

#include "terrain/Chunks.h"
#include "beings/Animals.h"

class World
{
    static const unsigned int dimensions = 100;
    static const unsigned int chunk_size = 64;
	Chunk chunks[dimensions][dimensions];
	std::vector<Cat> cats;

public:
	World();
	void update();
	void draw(SDL_Renderer* renderer);
};
