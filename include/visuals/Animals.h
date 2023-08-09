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
	vec2d* base;

	Limb(vec2d* base, unsigned int seg_count, float seg_len);
	void update(vec2d follows);
};

class AnimalVisual
{
	vec2d pos;
	static const unsigned int limb_count = 4;
	vec2d body_joints[limb_count];
	Limb limbs[limb_count];
	float leg_target = 100;
	float speed = 7;
	vec2d visual_coors;

public:
	AnimalVisual();
	void update();
	void draw(SDL_Renderer* renderer);
};
