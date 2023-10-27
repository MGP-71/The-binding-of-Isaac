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
	Animation* aIdleRight;
	Animation* animation; // Referencia a la animación mostrada

	Audio* audioShoot;
	int shootCadence = 30;
	int shootTime = 0;
};

