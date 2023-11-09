#include "ProjectileEnemy.h"

ProjectileEnemy::ProjectileEnemy(int direction, float x, float y, Game* game) :
	Actor("res/enemigos/disparo_enemigo.png", x, y, 18, 15, game) {
	if (direction == 1)
		vx = 9;
	else
		vx = -9;
	vy = -1; // La gravedad inicial es 1
}

void ProjectileEnemy::update() {
	vy = vy - 1; // La gravedad suma 1 en cada actualización restamos para anularla 
}

