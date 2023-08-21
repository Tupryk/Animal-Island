#pragma once

#include "utils.h"
#include "Physics.h"
#include "visuals/Structures.h"

class House : StaticBody
{
	vec2d exit;

public:
	HouseVisual visual;
};
