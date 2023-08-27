#pragma once

#include <string>

#include "utils.h"
#include "beings/Animals.h"

struct Relation;
struct Message;

class Person : public Animal
{
	std::vector<Relation> relations;
	std::vector<Message> inbox;
	float money = 0;
	unsigned int social = 99; // From 0% to 100%, how likely is it that person will start or end a conversation.
	unsigned int day = 0; // 0 to 6. If this is 5 or 6, person will not go to work.
	std::shared_ptr<Person> talking = nullptr;
	bool sleeping = true;

public:
	std::shared_ptr<House> home = nullptr;
	std::shared_ptr<House> work = nullptr;
	Person(vec2d pos);
	void send_message(std::shared_ptr<Person> to, std::string message);
	void process_inbox(bool end_all=false);
	AnimalState update(Chunk* neighbors[], std::vector<std::shared_ptr<Animal>> animals, std::vector<std::shared_ptr<Tree>> plants, float brightness);
	void put_inbox(Message mess);
};

struct Message
{
	std::shared_ptr<Person> sender;
	std::string text;
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
