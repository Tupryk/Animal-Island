#include "Physics.h"

std::shared_ptr<StaticBody> StaticBody::getClosest(std::vector<std::shared_ptr<StaticBody>> bodies)
{
	if (bodies.empty()) return nullptr;
	auto closestIt = std::min_element(bodies.begin(), bodies.end(), [&](const std::shared_ptr<StaticBody>& a, const std::shared_ptr<StaticBody>& b) {
	    return (a->pos - pos).get_length_squared() < (b->pos - pos).get_length_squared();
	});
	std::shared_ptr<StaticBody> closest = *closestIt;
	return closest;
}

void RigidBody::update_pos()
{
	vec2d tmp = vel + acc;
	vel = tmp - tmp*friction;
	pos = pos + vel;
}
