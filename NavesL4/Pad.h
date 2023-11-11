#pragma once

#include "Actor.h"

class Pad : public Actor
{
public:
	Pad(float x, float y, Game* game);
	float getOrientationX(float clickX);
};
