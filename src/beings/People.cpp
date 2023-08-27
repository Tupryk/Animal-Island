#include "beings/People.h"

Relation::Relation(std::shared_ptr<Person> to) : to(to) {}

Person::Person(vec2d pos) : Animal() {
	this->pos = pos;
	is_male = rand()%2 == 0;
	look_dir = vec2d((rand()%20-10)*.1*max_speed, (rand()%20-10)*.1*max_speed);
	see_distance = 5;
	reach = 40;
}

AnimalState Person::update(Chunk* neighbors[], std::vector<std::shared_ptr<Animal>> animals, std::vector<std::shared_ptr<Tree>> plants, float brightness)
{
	// Sleep manager
	if (sleeping) {
		if (brightness > .3) {
			sleeping = false;
			day++;
		}
		else return AnimalState::DEFAULT;
	} else if (brightness < .3 && in_house == home) {
		sleeping = true;
		return AnimalState::DEFAULT;
	}

	bool free = false; // Marker for checking if a person can continue with their free time or if they should go do something else.

	// Busy
	if (brightness <= .3 || (day%7 < 5 && brightness >= .5)) {
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
		else if (day%7 < 5 && brightness >= .5 && in_house != work)
		{
			if (in_house != nullptr && goTo(in_house->exit_door)) {
				pos = in_house->pos;
				in_house = nullptr;
			} else if (in_house == nullptr && goTo(work)) {
				in_house = work;
				pos = in_house->exit_door;
			}
		} else wander();
	// Free time
	} else free = true;

	// Free time manager
	if (free) {
		if (talking) {
			process_inbox();
		} else {
			if (in_house != nullptr && goTo(in_house->exit_door)) {
				pos = in_house->pos;
				in_house = nullptr;
			} else {
				std::vector<std::shared_ptr<StaticBody>> candidates;
				for (auto animal : animals)
					if (std::dynamic_pointer_cast<Person>(animal))
						candidates.push_back(animal);

				if (!candidates.empty()) {
					// Initiate conversation
					std::shared_ptr<Person> target = std::dynamic_pointer_cast<Person>(getClosest(candidates));
					if (goTo(target)) {
						talking = target;
						target->send_message(std::dynamic_pointer_cast<Person>(shared_from_this()), "hello");
					}
				}
				else wander();
			}
		}
	} else if (talking)
		process_inbox(true);

	if (in_house == work) money++;
	update_pos();

	return AnimalState::DEFAULT;
}

void Person::send_message(std::shared_ptr<Person> sender, std::string message) {
	Message mess = { sender, message };
	inbox.push_back(mess);
}

void Person::process_inbox(bool end_all)
{
	if (inbox.empty()) { talking = nullptr; return; }
	for (int i = inbox.size()-1; i >= 0; i--) {
		// std::cout << shared_from_this() << " recieved: " << inbox[i].text << std::endl;
		if (talking) {
			if (inbox[i].text == "bye" || rand()%100 >= social || end_all) {
				talking->send_message(std::dynamic_pointer_cast<Person>(shared_from_this()), "bye");
				// std::cout << shared_from_this() << " responded: " << "bye" << std::endl;
				talking = nullptr;
			} else {
				talking->send_message(std::dynamic_pointer_cast<Person>(shared_from_this()), "blah");
				// std::cout << shared_from_this() << " responded: " << "blah" << std::endl;
			}
		} else if (inbox[i].text == "hello") {
			talking = inbox[i].sender;
			talking->send_message(std::dynamic_pointer_cast<Person>(shared_from_this()), "hello");
			// std::cout << shared_from_this() << " responded: " << "hello" << std::endl;
		}
		inbox.erase(inbox.begin() + i);

		// Update relation to sender
	}
}
