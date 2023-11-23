#pragma once

#include "Actor.h"
#include "Animation.h" 
#include "Player.h" 

class Enemy : public Actor
{
public:
	Enemy(string filename, float x, float y, Game* game);
	void draw() override; // Va a sobrescribir
	virtual void update(Player* p);
	Animation* aMoving;
	Animation* animation; // Referencia a la animaci�n mostrada
	void impacted(); // Recibe impacto y pone animaci�n de morir
	int state;
	Animation* aDying;
	float vxIntelligence;
	float enemySpeed = 1.5f;
	int invulnerableTime = 0;

	int vidas;

};
