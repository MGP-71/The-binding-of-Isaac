#pragma once

#include "Actor.h"
#include "Animation.h" 
#include "Player.h" 

class Enemy : public Actor
{
public:
	Enemy(float x, float y, Game* game);
	void draw(float scrollX = 0, float scrollY = 0) override; // Va a sobrescribir
	void update(Player* p);
	Animation* aMoving;
	Animation* animation; // Referencia a la animación mostrada
	void impacted(); // Recibe impacto y pone animación de morir
	int state;
	Animation* aDying;
	float vxIntelligence;
	float enemySpeed = 1.5f;
};
