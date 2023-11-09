#pragma once

#include "Actor.h"


class ProjectileEnemy : public Actor
{
public:
	ProjectileEnemy(int direction, float x, float y, Game* game);
	void update();
};

