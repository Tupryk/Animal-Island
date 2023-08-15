#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

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

	Limb(vec2d* base, unsigned int seg_count, float total_len);
	void update(vec2d follows);
};

class AnimalVisual
{
	vec2d pos;
	float speed = 5;
	static const unsigned int limb_count = 4;
	vec2d body_joints[limb_count];
	Limb limbs[limb_count];
	vec2d leg_target[limb_count];
	bool target_reseting[limb_count];
	float target_limit = 50;

	Limb head;
	vec2d head_origin;
	float leg_width = 15;

public:
	AnimalVisual();
	void update();
	void draw(SDL_Renderer* renderer);
};
