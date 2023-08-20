#pragma once

#include <list>
#include <mutex>
#include <vector>
#include <memory>
#include <thread>
#include <algorithm>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "terrain/Chunks.h"
#include "beings/Animals.h"
#include "beings/Player.h"
#include "visuals/Chunks.h"

#define KEEP_STATS 0
#define DEBUG 0

class World
{
    static const unsigned int dimensions = 100;
    static const unsigned int chunk_size = 64;
	Chunk chunks[dimensions][dimensions];

	unsigned int day_cycle = 0;
	unsigned int day_duration = 3600;
	unsigned int year_cycle = 0;
	unsigned int year_duration = 365;
	float sun_angle = 0;
	float brightness = 0;

	// Shared ptr for different animal types
	std::vector<std::shared_ptr<Animal>> animals;
	std::mutex updated_animals_mutex;

	// Statictics
	std::vector<unsigned int> squirrel_population;
	std::vector<unsigned int> cat_population;

	WaterVisual water_visual;

	void update_stats();
	void update_time();
	void draw_mini_map(SDL_Renderer* renderer);
	void draw_world(SDL_Renderer* renderer);
	void render_stats(SDL_Renderer* renderer);
	AnimalState update_animal(const std::shared_ptr<Animal>& animal);
	std::shared_ptr<Animal> createAnimalCopy(const std::shared_ptr<Animal>& animal);
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
