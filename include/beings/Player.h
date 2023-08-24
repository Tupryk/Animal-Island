#pragma once

#include <memory>

#include "Physics.h"
#include "terrain/Structures.h"
#include "beings/People.h"

class House;

class Player : public RigidBody
{
	std::vector<Relation> relations;

public:
	std::shared_ptr<House> in_house = nullptr;
	float speed = 10;
	Player(vec2d pos);
	void interact(std::shared_ptr<StaticBody> obj);
	void enterHouse(std::shared_ptr<House> house);
	void exitHouse();
};
