#include "beings/People.h"

Relation::Relation(std::shared_ptr<Person> to) : to(to) {}

Person::Person(vec2d pos) : Animal() {
	this->pos = pos;
	is_male = rand()%2 == 0;
	look_dir = vec2d((rand()%20-10)*.1*max_speed, (rand()%20-10)*.1*max_speed);
}

void Person::updatePers(float brightness)
{
	if (brightness <= .3 && in_house != home)
	{
		if (in_house != nullptr && goTo(in_house->exit_door)) {
			pos = in_house->pos;
			in_house = nullptr;
		} else if (in_house == nullptr && goTo(home)) {
			home->enter(shared_from_this());
			in_house = home;
			pos = in_house->exit_door;
		}
	}
	else if (brightness >= .5 && in_house != work)
	{
		if (in_house != nullptr && goTo(in_house->exit_door)) {
			pos = in_house->pos;
			in_house = nullptr;
		} else if (in_house == nullptr && goTo(work)) {
			work->enter(shared_from_this());
			in_house = work;
			pos = in_house->exit_door;
		}
	} else wander();

	if (in_house == work) money++;
	update_pos();
}

