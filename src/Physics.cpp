#include "Physics.h"

void RigidBody::update_pos()
{
	vec2d tmp = vel + acc;
	vel = tmp - tmp*friction;
	pos = pos + vel;
}
