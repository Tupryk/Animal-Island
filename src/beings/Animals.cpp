#include "beings/Animals.h"

AnimalState Animal::update_basic()
{
	// Hunger
	if (hunger > max_hunger) hunger = max_hunger;
	if (hunger <= 0) { health--; hunger = 0; }
	else {
		if (hunger > max_hunger*.5)
			{ health++; if (acc.get_length() != 0) energy++; }
		hunger--;
	}

	// Movement and Energy
	if (energy <= 0) acc = vec2d(0, 0);
	update_pos();
	if (acc.get_length() != 0) energy -= acc.get_length()*.0001;

	// Age & Health
	age++;
	if (age >= max_age) health = 0;
	if (health > max_health) health = max_health;
	if (health <= 0) return AnimalState::DEAD;

	// Pregnancy
	if (pregnant >= 0) {
		pregnant++;
		if (pregnant >= pregnancy_time) {
			pregnant = -1;
			return AnimalState::HAD_CHILD;
		}
	} else {
		lust++;
		if (lust > lust_threshold) lust = lust_threshold;
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

AnimalState Animal::update(Chunk* neighbors[], std::vector<std::shared_ptr<Animal>> animals, std::vector<std::shared_ptr<Tree>> plants, float brightness) {
    return update_basic();
}

Cat::Cat(vec2d pos) : Animal() {
	this->pos = pos;
	max_speed = 1.5;
	is_male = rand()%2 == 0;
	look_dir = vec2d((rand()%20-10)*.1*max_speed, (rand()%20-10)*.1*max_speed);
}

AnimalState Cat::update(Chunk* neighbors[], std::vector<std::shared_ptr<Animal>> animals, std::vector<std::shared_ptr<Tree>> plants, float brightness)
{
	if (health <= 0) return AnimalState::DEAD;

	// Prepare candidates for prey
	std::vector<std::shared_ptr<StaticBody>> candidates;
	if (hunger < max_hunger*.75 && !animals.empty())
	{
		for (auto animal : animals)
			if (std::dynamic_pointer_cast<Squirrel>(animal))
				if (animal->getHealth() > 0)
					candidates.push_back(animal);
	}
	if (!candidates.empty()) {
		// Chase prey
		std::shared_ptr<Animal> target_prey = std::dynamic_pointer_cast<Animal>(getClosest(candidates));
		vec2d squirrel_direction(target_prey->pos.x - pos.x, target_prey->pos.y - pos.y);
		acc = squirrel_direction.norm() * max_speed;

		if (squirrel_direction.get_length() < reach) {
			acc = vec2d(0, 0);
			target_prey->hurt(strength*2);
			if (target_prey->getHealth() <= 0) hunger += target_prey->size.x*target_prey->size.y*1000;
		}
		cycle_counter = 0;
	}
	else wander();
	return update_basic();
}

std::shared_ptr<StaticBody> Animal::getClosest(std::vector<std::shared_ptr<StaticBody>> bodies)
{
	if (bodies.empty()) return nullptr;
	auto closestIt = std::min_element(bodies.begin(), bodies.end(), [&](const std::shared_ptr<StaticBody>& a, const std::shared_ptr<StaticBody>& b) {
	    return (a->pos - pos).get_length_squared() < (b->pos - pos).get_length_squared();
	});
	std::shared_ptr<StaticBody> closest = *closestIt;
	return closest;
}

Squirrel::Squirrel(vec2d pos) : Animal() {
	this->pos = pos;
	is_male = rand()%2 == 0;
	look_dir = vec2d((rand()%20-10)*.1*max_speed, (rand()%20-10)*.1*max_speed);;
}

AnimalState Squirrel::update(Chunk* neighbors[], std::vector<std::shared_ptr<Animal>> animals, std::vector<std::shared_ptr<Tree>> plants, float brightness)
{
	if (health <= 0) return AnimalState::DEAD;
	
	if (brightness <= .3) {
		// Get trees
		std::vector<std::shared_ptr<StaticBody>> trees;
		for (auto tree : plants)
			if (tree->fits(size.x*size.y))
				trees.push_back(tree);

		// Goto Nearest Tree
		if (on_tree) { energy++; acc = vec2d(0, 0); } 
		else if (!on_tree && !trees.empty()) {
			std::shared_ptr<Tree> closest_tree = std::dynamic_pointer_cast<Tree>(getClosest(trees));
			vec2d direction(closest_tree->pos.x - pos.x, closest_tree->pos.y - pos.y);
			acc = direction.norm() * max_speed;

			if (direction.get_length() < reach) {
				acc = vec2d(0, 0);
				closest_tree->adopt(size.x*size.y);
				on_tree = closest_tree;
			}
		}
		else wander();
		return update_basic();
	}
	else
	{
		if (on_tree) {
			on_tree->drop(size.x*size.y);
			on_tree = nullptr;
		}

		std::vector<std::shared_ptr<StaticBody>> threads;
		std::vector<std::shared_ptr<StaticBody>> candidates;
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
}

void Squirrel::give_pregnancy() { lust = 0; pregnant = 0; }
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
