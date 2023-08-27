#include "beings/People.h"

Relation::Relation(std::shared_ptr<Person> to) : to(to) {}

Person::Person(vec2d pos) : Animal() {
	this->pos = pos;
	is_male = rand()%2 == 0;
	look_dir = vec2d((rand()%20-10)*.1*max_speed, (rand()%20-10)*.1*max_speed);
}

AnimalState Person::update(Chunk* neighbors[], std::vector<std::shared_ptr<Animal>> animals, std::vector<std::shared_ptr<Tree>> plants, float brightness)
{	
	// Go Home (sleep)
	if (brightness <= .3 && in_house != home)
	{
		if (in_house != nullptr && goTo(in_house->exit_door)) {
			pos = in_house->pos;
			in_house = nullptr;
		} else if (in_house == nullptr && goTo(home)) {
			in_house = home;
			pos = in_house->exit_door;
		}
	}
	// Go to Work
	else if (brightness >= .5 && in_house != work)
	{
		if (in_house != nullptr && goTo(in_house->exit_door)) {
			pos = in_house->pos;
			in_house = nullptr;
		} else if (in_house == nullptr && goTo(work)) {
			in_house = work;
			pos = in_house->exit_door;
		}
	}
	// Free time
	else wander();

	if (in_house == work) money++;
	update_pos();

	return AnimalState::DEFAULT;
}

