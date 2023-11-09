#include "ProjectileEnemy.h"

ProjectileEnemy::ProjectileEnemy(int directionX, int directionY, float x, float y, Game* game) :
	Actor("res/enemigos/disparo_enemigo.png", x, y, 18, 15, game) {
	//está a su izquierda y por encima el jugador
	vx = 5;
	vy = 5;

}

void ProjectileEnemy::update() {
	//vy = vy - 1; // La gravedad suma 1 en cada actualización restamos para anularla 
}

