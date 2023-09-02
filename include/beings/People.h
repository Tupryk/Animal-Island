#pragma once

#include <string>

#include "utils.h"
#include "beings/Animals.h"

struct Relation;
struct Message;

class Person : public Animal
{
	float money = 0;
	unsigned int day = 0; // 0 to 6. If this is 5 or 6, person will not go to work.
	bool sleeping = true;

	// Speech
	bool talkative = true; // Personality trait
	unsigned int token_buffer = 0; // Counter for checking if token (character in message string can be sent).
	unsigned int token_period = 60; // Ticks for each character sent.
	unsigned int social = 50; // From 0% to 100%, how likely is it that person will start or end a conversation.
	unsigned int conversation_tokens = 5*social*.01; // How many times per day can a person start a conversation. (max 5)
	std::string send_queue;


	std::vector<Message> inbox;
	std::vector<Relation> relations;
	std::shared_ptr<Person> talking = nullptr;
	std::vector<std::shared_ptr<Animal>> talked_to;

public:
	std::shared_ptr<House> home = nullptr;
	std::shared_ptr<House> work = nullptr;

	Person(vec2d pos);
	void put_inbox(Message mess);
	void put_inbox(char token);
	void process_inbox(bool end_all=false);
	void send_message(std::shared_ptr<Person> to, std::string message);
	void send_message();
	AnimalState update(Chunk* neighbors[], std::vector<std::shared_ptr<Animal>> animals, std::vector<std::shared_ptr<Tree>> plants, float brightness);
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
