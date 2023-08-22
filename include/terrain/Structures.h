#pragma once

#include "utils.h"
#include "Physics.h"
#include "visuals/Structures.h"

class House : public StaticBody
{
public:
	vec2d exit;

	House(vec2d pos);
	HouseVisual visual;
};
