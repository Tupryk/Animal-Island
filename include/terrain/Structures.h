#pragma once

#include "utils.h"
#include "Physics.h"
#include "visuals/Structures.h"

class House : public StaticBody
{
	std::vector<std::shared_ptr<StaticBody>> bodies_inside;

public:
	vec2d inner_size;
	vec2d exit_door;

	House(vec2d pos);
	void enter(std::shared_ptr<StaticBody> body);
	void exit(std::shared_ptr<StaticBody> body);
	void draw_inside(SDL_Renderer* renderer, vec2d origin);
};
