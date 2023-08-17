#pragma once

#include "utils.h"

struct StaticBody
{
	vec2d pos;
	vec2d size;
};

struct RigidBody : public StaticBody
{
	vec2d vel;
	vec2d acc;
	float friction = .5;

	void update_pos();
};
