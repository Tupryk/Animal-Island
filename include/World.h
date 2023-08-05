#pragma once

#include "terrain/Chunks.h"
#include "visuals/Terrain.h"

class World
{
    Terrain terrain;

public:
	World();
	void update();
	void draw(SDL_Renderer* renderer);
};
