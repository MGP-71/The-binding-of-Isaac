#pragma once
#include "Character.h"
#include "Actor.h"
#include "Projectile.h" 
#include "Audio.h"
#include "Animation.h" // incluir animacion 

class Player : public Actor
{
public:
	Player(float x, float y, Game* game, Character* cha);
	virtual Projectile* shoot();
	virtual void update();
	void moveX(float axis);
	void moveY(float axis);
	void draw() override; // Va a sobrescribir
	void loseLife();
	void setAnimations();
	Animation* animation; // Referencia a la animación mostrada
	int orientation;
	Audio* audioShoot;
	Audio* audioGolpeado;
	Audio* audioMuerte;

	int shootTime = 0;
	int invulnerableTime = 0;

	int state;

	Character* character;
};

