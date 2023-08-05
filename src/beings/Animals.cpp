#include "beings/Animals.h"

Cat::Cat(vec2d pos) : velocity({0, 0}), pos(pos) {}

void Cat::update(Chunk* neighbors[])
{
	cycle_counter++;
	if (cycle_counter >= cycle_limit) {
		cycle_counter = 0;
		cycle_limit = 64+rand()%256;
		velocity = vec2d((rand()%20-10)*.1*max_speed, (rand()%20-10)*.1*max_speed);
	}
	if (velocity.x > 0 && (
		neighbors[4]->type == ChunkTypes::SAND ||
		neighbors[2]->type == ChunkTypes::SAND ||
		neighbors[7]->type == ChunkTypes::SAND)) velocity.x *= -1;
	if (velocity.x < 0 && (
		neighbors[0]->type == ChunkTypes::SAND ||
		neighbors[3]->type == ChunkTypes::SAND ||
		neighbors[5]->type == ChunkTypes::SAND)) velocity.x *= -1;
	if (velocity.y > 0 && (
		neighbors[5]->type == ChunkTypes::SAND ||
		neighbors[6]->type == ChunkTypes::SAND ||
		neighbors[7]->type == ChunkTypes::SAND)) velocity.y *= -1;
	if (velocity.y < 0 && (
		neighbors[0]->type == ChunkTypes::SAND ||
		neighbors[1]->type == ChunkTypes::SAND ||
		neighbors[2]->type == ChunkTypes::SAND)) velocity.y *= -1;
	pos = pos + velocity;
}

Squirrel::Squirrel(vec2d pos) : velocity({0, 0}), pos(pos) {}

void Squirrel::update(Chunk* neighbors[])
{
	cycle_counter++;
	if (cycle_counter >= cycle_limit) {
		cycle_counter = 0;
		cycle_limit = 64+rand()%256;
		velocity = vec2d((rand()%20-10)*.1*max_speed, (rand()%20-10)*.1*max_speed);
	}
	if (velocity.x > 0 && (
		neighbors[4]->type == ChunkTypes::SAND ||
		neighbors[2]->type == ChunkTypes::SAND ||
		neighbors[7]->type == ChunkTypes::SAND)) velocity.x *= -1;
	if (velocity.x < 0 && (
		neighbors[0]->type == ChunkTypes::SAND ||
		neighbors[3]->type == ChunkTypes::SAND ||
		neighbors[5]->type == ChunkTypes::SAND)) velocity.x *= -1;
	if (velocity.y > 0 && (
		neighbors[5]->type == ChunkTypes::SAND ||
		neighbors[6]->type == ChunkTypes::SAND ||
		neighbors[7]->type == ChunkTypes::SAND)) velocity.y *= -1;
	if (velocity.y < 0 && (
		neighbors[0]->type == ChunkTypes::SAND ||
		neighbors[1]->type == ChunkTypes::SAND ||
		neighbors[2]->type == ChunkTypes::SAND)) velocity.y *= -1;
	pos = pos + velocity;
}
