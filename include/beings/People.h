#pragma once

#include "utils.h"
#include "beings/Animals.h"
#include "terrain/Structures.h"

class Person : public Animal
{
public:
	std::shared_ptr<House> in_house = nullptr;
	Person(vec2d pos);
};
