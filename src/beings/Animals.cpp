#include "beings/Animals.h"

Cat::Cat(vec2d pos) : velocity({0, 0}), pos(pos) {}

void Cat::update()
{
	velocity = vec2d((rand()%3-1)*max_speed, (rand()%3-1)*max_speed);
	pos = pos + velocity;
}
