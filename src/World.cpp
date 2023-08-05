#include "World.h"

World::World() : terrain() {}

void World::update()
{

}

void World::draw(SDL_Renderer* renderer)
{
	displayTerrain(renderer, &terrain);
}

