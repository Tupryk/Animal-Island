#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "utils.h"

class WaterVisual
{
	static const unsigned int points_dim = 16;
	float grid[points_dim][points_dim];
	bool reverse[points_dim][points_dim];
	float speed = 1;
	vec2d size;
	vec2d pos;
	float sep_x;
	float sep_y;

public:
	WaterVisual();
	void update();
	void draw(SDL_Renderer* renderer);
};
