#pragma once

#include "Physics.h"

class Player : public RigidBody
{
public:
	float speed = 10;
	Player(vec2d pos);
};
