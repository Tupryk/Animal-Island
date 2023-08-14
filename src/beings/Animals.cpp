#include "beings/Animals.h"

void Animal::updade_vel()
{
	vec2d tmp = vel + acc;
	vel = tmp - tmp*friction;
}

AnimalState Animal::update(Chunk* neighbors[], std::vector<Animal*> animals) {
    return AnimalState::DEFAULT;
}

Cat::Cat(vec2d pos) : Animal() {
	this->pos = pos;
	is_male = rand()%2 == 0;
	look_dir = vec2d(1, 0);
}

AnimalState Cat::update(Chunk* neighbors[], std::vector<Animal*> animals)
{
	if (health <= 0) return AnimalState::DEAD;

	// Update hunger
	if (hunger <= 0) health--;
	else {
		if (hunger > max_hunger*.5) health++;
		hunger--;
	}
	if (hunger > max_hunger) hunger = max_hunger;

	// Update hunger
	if (hunger <= 0) health--;
	else {
		if (hunger > max_hunger*.5) health++;
		hunger--;
	}
	if (hunger > max_hunger) hunger = max_hunger;
	/*
	if (squirrels.size() > 0 && hunger < max_hunger*.75) {
		// Chase prey
		Squirrel* closest_squirrel;
		unsigned int start_index = 0;
		while(start_index < squirrels.size()) {
			if (squirrels[start_index]->getHealth() > 0 && !squirrels[start_index]->on_tree) {
				closest_squirrel = squirrels[start_index];
				break;
			}
			start_index++;
		}
		for (int i = start_index+1; i < squirrels.size(); i++) {
			vec2d squirrel_direction(closest_squirrel->pos.x - pos.x, closest_squirrel->pos.y - pos.y);
			vec2d squirrel_direction_new(squirrels[i]->pos.x - pos.x, squirrels[i]->pos.y - pos.y);

			if (squirrel_direction.get_length() > squirrel_direction_new.get_length() && squirrels[i]->getHealth() > 0 && !squirrels[i]->on_tree)
				closest_squirrel = squirrels[i];
		}
		vec2d squirrel_direction(closest_squirrel->pos.x - pos.x, closest_squirrel->pos.y - pos.y);
		acc = squirrel_direction.norm() * max_speed;

		if (squirrel_direction.get_length() < 10) {
			closest_squirrel->hurt(strength);
			if (closest_squirrel->getHealth() <= 0) hunger += closest_squirrel->size*1000;
		}
		cycle_counter = 0;
	}
	else {*/
		// Update random walking
		cycle_counter++;
		if (cycle_counter >= cycle_limit) {
			cycle_counter = 0;
			cycle_limit = 64+rand()%256;
			acc = vec2d((rand()%20-10)*.1*max_speed, (rand()%20-10)*.1*max_speed);
		}
	//}
	
	// Don't jump into water
	if (acc.x > 0 && (
		neighbors[4]->type == ChunkTypes::SAND ||
		neighbors[2]->type == ChunkTypes::SAND ||
		neighbors[7]->type == ChunkTypes::SAND)) acc.x *= -1;
	if (acc.x < 0 && (
		neighbors[0]->type == ChunkTypes::SAND ||
		neighbors[3]->type == ChunkTypes::SAND ||
		neighbors[5]->type == ChunkTypes::SAND)) acc.x *= -1;
	if (acc.y > 0 && (
		neighbors[5]->type == ChunkTypes::SAND ||
		neighbors[6]->type == ChunkTypes::SAND ||
		neighbors[7]->type == ChunkTypes::SAND)) acc.y *= -1;
	if (acc.y < 0 && (
		neighbors[0]->type == ChunkTypes::SAND ||
		neighbors[1]->type == ChunkTypes::SAND ||
		neighbors[2]->type == ChunkTypes::SAND)) acc.y *= -1;
	pos = pos + vel + ( acc * .5 );

	if (health > max_health) health = max_health;

	return AnimalState::DEFAULT;
}

Squirrel::Squirrel(vec2d pos) : Animal() {
	this->pos = pos;
	is_male = rand()%2 == 0;
	look_dir = vec2d(1, 0);
}

AnimalState Squirrel::update(Chunk* neighbors[], std::vector<Animal*> animals)
{
	if (health <= 0) return AnimalState::DEAD;

	horny++;
	if (horny > horny_threshold) horny = horny_threshold;

	std::vector<Squirrel*> candidates;
	if (animals.size() > 0 && this->pregnant < 0)
	{
		for (auto anim : animals) {
			if (Squirrel* squirrel = dynamic_cast<Squirrel*>(anim)) {
				if (squirrel->pregnant < 0 && this->is_male != squirrel->is_male && squirrel->getHealth() > 0)
					candidates.push_back(squirrel);
			}
		}
	}

	if (candidates.size() > 0 && horny >= horny_threshold) {
		Squirrel* closest_squirrel = candidates[0];
		for (int i = 1; i < candidates.size(); i++) {
			vec2d squirrel_direction(closest_squirrel->pos.x - pos.x, closest_squirrel->pos.y - pos.y);
			vec2d squirrel_direction_new(candidates[i]->pos.x - pos.x, candidates[i]->pos.y - pos.y);

			if (squirrel_direction.get_length() > squirrel_direction_new.get_length())
				closest_squirrel = candidates[i];
		}
		vec2d squirrel_direction(closest_squirrel->pos.x - pos.x, closest_squirrel->pos.y - pos.y);
		acc = squirrel_direction.norm() * max_speed;

		if (squirrel_direction.get_length() < reach && is_male) {
			closest_squirrel->give_pregnancy();
			horny = 0;
		}
		cycle_counter = 0;
	}
	else {
		cycle_counter++;
		if (cycle_counter >= cycle_limit) {
			cycle_counter = 0;
			cycle_limit = 64+rand()%256;
			acc = vec2d((rand()%20-10)*.1*max_speed, (rand()%20-10)*.1*max_speed);
		}
	}
	
	if (acc.x > 0 && (
		neighbors[4]->type == ChunkTypes::SAND ||
		neighbors[2]->type == ChunkTypes::SAND ||
		neighbors[7]->type == ChunkTypes::SAND)) acc.x *= -1;
	if (acc.x < 0 && (
		neighbors[0]->type == ChunkTypes::SAND ||
		neighbors[3]->type == ChunkTypes::SAND ||
		neighbors[5]->type == ChunkTypes::SAND)) acc.x *= -1;
	if (acc.y > 0 && (
		neighbors[5]->type == ChunkTypes::SAND ||
		neighbors[6]->type == ChunkTypes::SAND ||
		neighbors[7]->type == ChunkTypes::SAND)) acc.y *= -1;
	if (acc.y < 0 && (
		neighbors[0]->type == ChunkTypes::SAND ||
		neighbors[1]->type == ChunkTypes::SAND ||
		neighbors[2]->type == ChunkTypes::SAND)) acc.y *= -1;
	pos = pos + vel + ( acc * .5 );

	if (pregnant >= 0) pregnant++;
	if (pregnant >= pregnancy_time) {
		pregnant = -1;
		return AnimalState::HAD_CHILD;
	}
	return AnimalState::DEFAULT;
}

void Squirrel::give_pregnancy() { pregnant = 0; }

float Animal::getHealth() { return health; }
void Animal::hurt(float damage) { health -= damage; }

/*
Shark::Shark(vec2d pos) : Animal() {
	this->pos = pos;
	is_male = rand()%2 == 0;
	friction = 0;
}

AnimalState Shark::update(Chunk* neighbors[])
{
	cycle_counter++;
	if (cycle_counter >= cycle_limit) {
		cycle_counter = 0;
		cycle_limit = 64+rand()%256;
		acc = vec2d((rand()%20-10)*.1*max_speed, (rand()%20-10)*.1*max_speed);
	}
	pos = pos + vel + ( acc * .5 );
	return AnimalState::DEFAULT;
}
*/
