#pragma once

#include "utils.h"
#include "beings/Animals.h"
#include "terrain/Structures.h"

struct Relation;

class Person : public Animal
{
	std::vector<Relation> relations;
	float money = 0;

public:
	std::shared_ptr<House> in_house = nullptr;
	std::shared_ptr<House> home = nullptr;
	std::shared_ptr<House> work = nullptr;
	Person(vec2d pos);
	void updatePers(float brightness);
};

struct Relation
{
	std::shared_ptr<Person> to;
	float knowledge = 0; // How well do you know this person
	float attraction = 0;
	float friendship = 0;
	float love_interest = 0;
	float hate = 0;

	Relation(std::shared_ptr<Person> to);
};
