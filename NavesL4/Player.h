#pragma once

#include "Actor.h"
#include "Projectile.h" 
#include "Audio.h"
#include "Animation.h" // incluir animacion 

class Player : public Actor
{
public:
	Player(float x, float y, Game* game);
	Projectile* shoot();
	void update();
	void moveX(float axis);
	void moveY(float axis);
	void draw() override; // Va a sobrescribir
	Animation* animation; // Referencia a la animación mostrada
	int orientation;
	Audio* audioShoot;
	int shootCadence = 30;
	int shootTime = 0;

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

