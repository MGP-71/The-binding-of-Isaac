#pragma once

#include "Player.h"
#include "Projectile.h" 
#include "Audio.h"
#include "Animation.h" // incluir animacion 

class Eve : public Player
{
public:
	Eve(float x, float y, Game* game);

	Projectile* shoot() override;
	void update() override;
	void moveX(float axis);
	void moveY(float axis);
	void draw() override; // Va a sobrescribir
	void loseLife();
	Animation* animation; // Referencia a la animación mostrada
	int orientation;
	Audio* audioShoot;
	int shootCadence = 30;
	int shootTime = 0;
	float playerSpeed;
	int lifes = 3;
	int invulnerableTime = 0;

	Animation* aIdleRight;
	Animation* aIdleUp;
	Animation* aIdleDown;
	Animation* aIdleLeft;
	Animation* aRunningRight;
	Animation* aRunningLeft;
	Animation* aRunningUp;
	Animation* aRunningDown;

	int state;
	Animation* aShootingRight;
	Animation* aShootingLeft;
	Animation* aShootingUp;
	Animation* aShootingDown;
};

