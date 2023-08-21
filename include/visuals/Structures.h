#pragma once

#include <SDL2/SDL.h>

#include "utils.h"
#include "Physics.h"

class HouseVisual : StaticBody
{
	unsigned int floor_color[3] = {125, 78, 78};
	unsigned int walls_color[3] = {200, 128, 128};
public:
	void draw_inside(SDL_Renderer* renderer, vec2d origin);
};
