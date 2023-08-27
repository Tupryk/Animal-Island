#pragma once

#include "utils.h"
#include "Physics.h"
#include "visuals/Structures.h"

class House : public StaticBody
{
	unsigned int color[3];

public:
	vec2d inner_size;
	vec2d exit_door;
	std::vector<std::shared_ptr<StaticBody>> bodies_inside;

	House(vec2d pos, unsigned int color[3]);
	void enter(std::shared_ptr<StaticBody> body);
	void exit(std::shared_ptr<StaticBody> body);
	void clear_bodies();
	void draw_inside(SDL_Renderer* renderer, vec2d origin);
	void draw_outside(SDL_Renderer* renderer, vec2d origin);
};
