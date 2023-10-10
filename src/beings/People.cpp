#include "beings/People.h"

Relation::Relation(std::shared_ptr<Person> to) : to(to) {}

Person::Person(vec2d pos) : Animal() {
	this->pos = pos;
	is_male = rand()%2 == 0;
	look_dir = vec2d((rand()%20-10)*.1*max_speed, (rand()%20-10)*.1*max_speed);
	see_distance = 5;
}

AnimalState Person::update(Chunk* neighbors[], std::vector<std::shared_ptr<Animal>> animals, std::vector<std::shared_ptr<Tree>> plants, float brightness)
{
	// Sleep manager
	if (sleeping) {
		if (brightness > .3) {
			sleeping = false;
			day++;
			conversation_tokens = 5*social*.01;
			talked_to.clear();
			talkative = (rand()%100 < social);
		}
		else return AnimalState::DEFAULT;
	} else if (brightness < .3 && in_house == home) {
		sleeping = true;
		return AnimalState::DEFAULT;
	}

	// Busy
	if (brightness <= .3 || (day%7 < 5 && brightness >= .8)) {
		if (talking) process_inbox(true);
		// Go Home (sleep)
		else if (brightness <= .3 && home && in_house != home)
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
		else if (day%7 < 5 && brightness >= .8 && work && in_house != work)
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
	} else if (talking)
		process_inbox();
	else if (in_house != nullptr) {
		if (goTo(in_house->exit_door)) {
			pos = in_house->pos;
			in_house = nullptr;
		}
	} else if (talkative && conversation_tokens > 0) {
		std::vector<std::shared_ptr<StaticBody>> candidates;
		for (auto animal : animals)
			if (std::dynamic_pointer_cast<Person>(animal) && std::find(talked_to.begin(), talked_to.end(), animal) == talked_to.end())
				candidates.push_back(animal);

		if (!candidates.empty()) {
			// Initiate conversation
			std::shared_ptr<Person> target = std::dynamic_pointer_cast<Person>(getClosest(candidates));
			if (goTo(target)) {
				talking = target;
				send_message(target, "hello");
				talked_to.push_back(target);
			}
		}
	} else wander();

	if (in_house == work) money++;
	update_pos();

	return AnimalState::DEFAULT;
}

void Person::send_message(std::shared_ptr<Person> reciever, std::string message) {
	Message mess = { std::dynamic_pointer_cast<Person>(shared_from_this()), message };
	reciever->put_inbox(mess);
}

void Person::send_message()
{
	char to_be_sent = '';
	if (token_buffer <= 0) {
		to_be_sent = send_queue[0];
    	send_queue.erase(0, 1);
    	token_buffer = token_period;
	} else
		 token_buffer--;
	taking->put_inbox(to_be_sent);
}

void Person::put_inbox(Message mess) { inbox.push_back(mess); }
void Person::put_inbox(char token) { inbox.push_back(token); }

void Person::process_inbox(bool end_all);
{

}

void Person::process_inbox(bool end_all)
{
	if (inbox.empty()) { talking = nullptr; return; }
	for (int i = inbox.size()-1; i >= 0; i--) {
		// std::cout << shared_from_this() << " recieved: " << inbox[i].text << std::endl;
		if (talking) {
			if (inbox[i].text == "bye" || rand()%100 >= social || end_all) {
				send_message(talking, "bye");
				// std::cout << shared_from_this() << " responded: " << "bye" << std::endl;
				talking = nullptr;
				conversation_tokens--;
				talkative = (rand()%100 < social);
			} else {
				talking->send_message(talking, "blah");
				// std::cout << shared_from_this() << " responded: " << "blah" << std::endl;
			}
		} else if (inbox[i].text == "hello") {
			talking = inbox[i].sender;
			send_message(talking, "hello");
			// std::cout << shared_from_this() << " responded: " << "hello" << std::endl;
		}
		inbox.erase(inbox.begin() + i);

		// Update relation to sender
	}
}
