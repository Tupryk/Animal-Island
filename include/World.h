#pragma once

#include <list>
#include <vector>
#include <memory>
#include <algorithm>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "terrain/Chunks.h"
#include "beings/Animals.h"
#include "beings/Player.h"

#define KEEP_STATS 0
#define DEBUG 0

class World
{
    static const unsigned int dimensions = 100;
    static const unsigned int chunk_size = 64;
	Chunk chunks[dimensions][dimensions];

	// Shared ptr for different animal types
	std::list<std::shared_ptr<Animal>> animals;

	// Statictics
	std::vector<unsigned int> squirrel_population;
	std::vector<unsigned int> cat_population;

	void update_stats();
	void draw_mini_map(SDL_Renderer* renderer);
	void draw_world(SDL_Renderer* renderer);
	void render_stats(SDL_Renderer* renderer);
	Chunk* pos2chunk(vec2d pos);
	std::vector<Chunk*> get_chunks_viewed(float fov, float distance, vec2d pos, vec2d dir);

public:
	bool display_stats = false;
	bool big_map = false;
	Player player;

	World();
	void update();
	void draw(SDL_Renderer* renderer);
};
