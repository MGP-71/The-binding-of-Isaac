#pragma once
#include "Enemy.h"
#include "ProjectileEnemy.h"

class Horf : public Enemy
{
public:
	Horf(float x, float y, Game* game);
	void update(Player* p) override;
	ProjectileEnemy* shoot();
	int shootCadence = 60;
	int shootTime = 0;
};

