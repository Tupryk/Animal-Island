#include "beings/Animals.h"

Cat::Cat(vec2d pos) : Animal() {
	velocity = {0, 0};
	this->pos = pos;
}

void Cat::update(Chunk* neighbors[], std::vector<Squirrel*> squirrels)
{
	if (health <= 0) return;

	// Update hunger
	if (hunger <= 0) health--;
	else {
		if (hunger > max_hunger*.5) health++;
		hunger--;
	}
	if (hunger > max_hunger) hunger = max_hunger;

	if (squirrels.size() > 0) { // && hunger < hunger*.75) {
		// Chase prey
		Squirrel* closest_squirrel;
		unsigned int start_index = 0;
		while(start_index < squirrels.size()) {
			if (squirrels[start_index]->getHealth() > 0) {
				closest_squirrel = squirrels[start_index];
				break;
			}
			start_index++;
		}
		for (int i = start_index+1; i < squirrels.size(); i++) {
			vec2d squirrel_direction(closest_squirrel->pos.x - pos.x, closest_squirrel->pos.y - pos.y);
			vec2d squirrel_direction_new(squirrels[i]->pos.x - pos.x, squirrels[i]->pos.y - pos.y);

			if (squirrel_direction.get_length() > squirrel_direction_new.get_length() && squirrels[i]->getHealth() > 0)
				closest_squirrel = squirrels[i];
		}
		vec2d squirrel_direction(closest_squirrel->pos.x - pos.x, closest_squirrel->pos.y - pos.y);
		velocity = squirrel_direction.norm() * max_speed;

		if (squirrel_direction.get_length() < 10) {
			closest_squirrel->hurt(strength);
			if (closest_squirrel->getHealth() <= 0) hunger += closest_squirrel->size;
		}
		cycle_counter = 0;
	}
	else {
		// Update random walking
		cycle_counter++;
		if (cycle_counter >= cycle_limit) {
			cycle_counter = 0;
			cycle_limit = 64+rand()%256;
			velocity = vec2d((rand()%20-10)*.1*max_speed, (rand()%20-10)*.1*max_speed);
		}
	}

	// Don't jump into water
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

	if (health > max_health) health = max_health;
}

Squirrel::Squirrel(vec2d pos) : Animal() {
	velocity = {0, 0};
	this->pos = pos;
}

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

float Animal::getHealth() { return health; }
void Animal::hurt(float damage) { health -= damage; }
