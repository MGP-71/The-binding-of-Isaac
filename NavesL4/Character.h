#pragma once
#include "Animation.h"
#include "Game.h"

class Character
{
public:
	Character(Game* game);
	Game* game;
	int shootCadence;
	float playerSpeed;
	int lifes;

	Animation* aIdleRight;
	Animation* aIdleUp;
	Animation* aIdleDown;
	Animation* aIdleLeft;
	Animation* aRunningRight;
	Animation* aRunningLeft;
	Animation* aRunningUp;
	Animation* aRunningDown;

	Animation* aShootingRight;
	Animation* aShootingLeft;
	Animation* aShootingUp;
	Animation* aShootingDown;

	int width = 47;
	int height = 46;

	bool invencibilidad = false;
	bool rompeRocas = false;
};