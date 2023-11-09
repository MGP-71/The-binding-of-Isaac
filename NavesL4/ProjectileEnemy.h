#pragma once

#include "Actor.h"


class ProjectileEnemy : public Actor
{
public:
	ProjectileEnemy(int directionx, int directiony, float x, float y, Game* game);
	void update();
};

