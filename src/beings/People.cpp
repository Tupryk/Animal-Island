#include "beings/People.h"

Relation::Relation(std::shared_ptr<Person> to) : to(to) {}

Person::Person(vec2d pos) : Animal() {
	this->pos = pos;
	is_male = rand()%2 == 0;
	look_dir = vec2d((rand()%20-10)*.1*max_speed, (rand()%20-10)*.1*max_speed);
}

void Person::updatePers(float brightness)
{
	if (brightness <= .3 && goTo(home)) {
		in_house = home;
	}
	else if (brightness >= .5 && goTo(work)) {
		in_house = work;
	}
	else {
		in_house = nullptr;
		wander();
	}

	if (in_house == work) money++;
	update_pos();
}

