#include "World.h"

World::World() : terrain() {}

void World::update()
{
	for (int i = 0; i < terrain.dimensions; i++)
		for (int j = 0; j < terrain.dimensions; j++)
			terrain.chunks[i][j].update();
}

void World::draw(SDL_Renderer* renderer)
{
	displayTerrain(renderer, &terrain);
}
