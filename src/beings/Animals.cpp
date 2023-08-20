#include "beings/Animals.h"

AnimalState Animal::update_basic()
{
	// Hunger
	if (hunger > max_hunger) hunger = max_hunger;
	if (hunger <= 0) health--;
	else {
		if (hunger > max_hunger*.5) health++;
		hunger--;
	}

	// Lust
	lust++;
	if (lust > lust_threshold) lust = lust_threshold;

	vec2d tmp = vel + acc;
	vel = tmp - tmp*friction;
	pos = pos + vel;

	// Age & Health
	age++;
	if (age >= max_age) health = 0;
	if (health > max_health) health = max_health;
	if (health <= 0) return AnimalState::DEAD;

	// Pregnancy
	if (pregnant >= 0) pregnant++;
	if (pregnant >= pregnancy_time) {
		pregnant = -1;
		return AnimalState::HAD_CHILD;
	}

	return AnimalState::DEFAULT;
}

void Animal::wander()
{
	// Random walking
	cycle_counter++;
	if (cycle_counter >= cycle_limit) {
		cycle_counter = 0;
		cycle_limit = 64+rand()%256;
		float scale = max_speed*.05;
		acc = vec2d(
			(rand()%20-10)*scale,
			(rand()%20-10)*scale);
	}
}

std::shared_ptr<Animal> Animal::build_child() { return nullptr; }

AnimalState Animal::update(Chunk* neighbors[], std::vector<std::shared_ptr<Animal>> animals, std::vector<std::shared_ptr<Tree>> plants) {
    return update_basic();
}

Cat::Cat(vec2d pos) : Animal() {
	this->pos = pos;
	max_speed = 1.5;
	is_male = rand()%2 == 0;
	look_dir = vec2d((rand()%20-10)*.1*max_speed, (rand()%20-10)*.1*max_speed);
}

AnimalState Cat::update(Chunk* neighbors[], std::vector<std::shared_ptr<Animal>> animals, std::vector<std::shared_ptr<Tree>> plants)
{
	if (health <= 0) return AnimalState::DEAD;

	// Prepare candidates for prey
	std::vector<std::shared_ptr<Animal>> candidates;
	if (hunger < max_hunger*.75 && !animals.empty())
	{
		for (auto animal : animals)
			if (std::dynamic_pointer_cast<Squirrel>(animal))
				if (animal->getHealth() > 0)
					candidates.push_back(animal);
	}
	if (!candidates.empty()) {
		// Chase prey
		std::shared_ptr<Animal> target_prey = getClosest(candidates);
		vec2d squirrel_direction(target_prey->pos.x - pos.x, target_prey->pos.y - pos.y);
		acc = squirrel_direction.norm() * max_speed;

		if (squirrel_direction.get_length() < reach) {
			acc = vec2d(0, 0);
			target_prey->hurt(strength*2);
			if (target_prey->getHealth() <= 0) hunger += target_prey->size*1000;
		}
		cycle_counter = 0;
	}
	else wander();
	return update_basic();
}

std::shared_ptr<Animal> Animal::getClosest(std::vector<std::shared_ptr<Animal>> animals)
{
	// Should be optimized
	if (animals.empty()) return nullptr;
	std::shared_ptr<Animal> closest = animals[0];
	for (int i = 1; i < animals.size(); i++) {
		vec2d direction(closest->pos.x - pos.x, closest->pos.y - pos.y);
		vec2d new_direction(animals[i]->pos.x - pos.x, animals[i]->pos.y - pos.y);

		if (direction.get_length() > new_direction.get_length())
			closest = animals[i];
	}
	return closest;
}

Squirrel::Squirrel(vec2d pos) : Animal() {
	this->pos = pos;
	is_male = rand()%2 == 0;
	look_dir = vec2d((rand()%20-10)*.1*max_speed, (rand()%20-10)*.1*max_speed);;
}

AnimalState Squirrel::update(Chunk* neighbors[], std::vector<std::shared_ptr<Animal>> animals, std::vector<std::shared_ptr<Tree>> plants)
{
	if (health <= 0) return AnimalState::DEAD;

	std::vector<std::shared_ptr<Animal>> threads;
	std::vector<std::shared_ptr<Animal>> candidates;
	if (!animals.empty() && this->pregnant < 0)
	{
		for (auto animal : animals)
			if (std::shared_ptr<Squirrel> squirrel = std::dynamic_pointer_cast<Squirrel>(animal)) {
				if (squirrel->pregnant < 0 && this->is_male != squirrel->is_male && squirrel->getHealth() > 0 && squirrel->age > .01*max_age)
					candidates.push_back(animal);
			}
			else if (std::shared_ptr<Cat> cat = std::dynamic_pointer_cast<Cat>(animal)) {
				if ((cat->pos-this->pos).get_length() < 128)
					threads.push_back(animal);	
			}
	}
	if (threads.size() > 0) {
		vec2d sum(0, 0);
		for (auto cat : threads)
			sum = sum + (this->pos-cat->pos);
		acc = sum.norm() * max_speed;
	}
	else if (!candidates.empty() && lust >= lust_threshold) {
		std::shared_ptr<Squirrel> closest_squirrel = std::dynamic_pointer_cast<Squirrel>(getClosest(candidates));
		vec2d squirrel_direction(closest_squirrel->pos.x - pos.x, closest_squirrel->pos.y - pos.y);
		acc = squirrel_direction.norm() * max_speed;

		if (squirrel_direction.get_length() < reach && is_male) {
			acc = vec2d(0, 0);
			closest_squirrel->give_pregnancy();
			lust = 0;
		}
		cycle_counter = 0;
	}
	else wander();
	return update_basic();
}

void Squirrel::give_pregnancy() { pregnant = 0; }
std::shared_ptr<Animal> Squirrel::build_child() { return std::make_shared<Squirrel>(pos); }

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
