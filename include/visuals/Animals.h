#pragma once

#include <vector>
#include <SDL2/SDL.h>

#include "utils.h"

struct Segment
{
	vec2d origin;
	vec2d end;
	float length;
	float angle;

	Segment(float x, float y, float length, float angle);
	void follow(vec2d target);
	void set_origin(vec2d pos);
	void update();
};

struct Limb
{
	std::vector<Segment> segs;
	unsigned int seg_count;
	vec2d base;
	float leg_origin = 100;
	float speed = 5;

	Limb(unsigned int seg_count=10, float seg_len=32, vec2d base={300, 0});
	void update(vec2d follows);
};

class AnimalVisual
{
	std::vector<Limb> limbs;
	float leg_target = 100;
	float speed = 7;
	vec2d visual_coors;

public:
	AnimalVisual();
	void update();
	void draw(SDL_Renderer* renderer);
};
