#pragma once

#include "Physics.h"
#include "terrain/Structures.h"

class House;

class Player : public RigidBody
{
public:
	std::shared_ptr<House> in_house = nullptr;
	float speed = 10;
	Player(vec2d pos);
	void interact(std::shared_ptr<StaticBody> obj);
	void enterHouse(std::shared_ptr<House> house);
	void exitHouse();
};
