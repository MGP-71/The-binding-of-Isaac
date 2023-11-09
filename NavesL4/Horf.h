#pragma once
#include "Enemy.h"
#include "ProjectileEnemy.h"
#include "Player.h"

class Horf : public Enemy
{
public:
	Horf(float x, float y, Game* game);
	void update(Player* p) override;
	ProjectileEnemy* shoot(Player* p);
	int shootCadence = 120;
	int shootTime = 0;
};

