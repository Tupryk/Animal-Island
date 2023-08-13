#pragma once

#include <vector>
#include <algorithm>

#include <SDL2/SDL.h>

#include "terrain/Chunks.h"
#include "beings/Animals.h"

#define KEEP_STATS 0

class World
{
    static const unsigned int dimensions = 100;
    static const unsigned int chunk_size = 64;
	Chunk chunks[dimensions][dimensions];
	std::vector<Cat> cats;
	std::vector<Squirrel> squirrels;
	std::vector<Shark> sharks;

	// Statictics
	std::vector<unsigned int> squirrel_population;
	std::vector<unsigned int> cat_population;
	void update_stats();
	void render_stats(SDL_Renderer* renderer);

public:
	bool display_stats = false;

	World();
	void update();
	void draw(SDL_Renderer* renderer);
};
