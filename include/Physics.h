#pragma once

#include "utils.h"

struct StaticBody : public std::enable_shared_from_this<StaticBody>
{
	vec2d pos;
	vec2d size;
	
	virtual ~StaticBody() = default;
	std::shared_ptr<StaticBody> getClosest(std::vector<std::shared_ptr<StaticBody>> bodies);
};

struct RigidBody : public StaticBody
{
	vec2d vel;
	vec2d acc;
	float friction = .5;

	void update_pos();
	virtual ~RigidBody() = default;
};
