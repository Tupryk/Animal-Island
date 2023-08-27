#pragma once

#include "utils.h"
#include "Physics.h"
#include "visuals/Structures.h"

class House : public StaticBody
{
public:
	vec2d inner_size;
	vec2d exit_door;
	std::vector<std::shared_ptr<StaticBody>> bodies_inside;

	House(vec2d pos);
	void enter(std::shared_ptr<StaticBody> body);
	void exit(std::shared_ptr<StaticBody> body);
	void clear_bodies();
	void draw_inside(SDL_Renderer* renderer, vec2d origin);
};
