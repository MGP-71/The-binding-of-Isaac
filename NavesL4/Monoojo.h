#pragma once
#include "Enemy.h"
#include "Player.h"
#include "ProjectileEnemy.h"
class Monoojo : public Enemy
{
public:
	Monoojo(float x, float y, Game* game);
	void update(Player* p) override;
	ProjectileEnemy* shoot(Player* p);
	int shootCadence = 160;
	int shootTime = 0;
};

