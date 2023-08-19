#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "utils.h"

struct Branch
{
	vec2d pose;
	std::vector<Branch> branches;
	float thickness;
	float color;
	Branch(vec2d pose, float thickness);
	void populate(unsigned int child_min, unsigned int child_max, float child_len);
};

class TreeVisual
{
	vec2d pos;
	Branch root;
	float original_size;
	float scale = 1;

	void draw_branch(Branch branch, vec2d spos, SDL_Renderer* renderer);
	float getMinX(Branch branch, vec2d spos);
	float getMaxX(Branch branch, vec2d spos);

public:
	TreeVisual(vec2d pos = {0, 0});
	void setScale(float new_size);
	void setPos(vec2d pos);
	void update();
	void draw(SDL_Renderer* renderer);
};
