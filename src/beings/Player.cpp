#include "beings/Player.h"

Player::Player(vec2d pos) { this->pos = vec2d(pos); }

void Player::interact(std::shared_ptr<StaticBody> obj)
{

}

void Player::enterHouse(std::shared_ptr<House> house) {
	in_house = house;
	pos = house->exit_door;
}

void Player::exitHouse() {
	if (in_house) {
		pos = in_house->pos;
		in_house = nullptr;
	}
}
