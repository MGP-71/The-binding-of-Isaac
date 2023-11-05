#include "Projectile.h"

Projectile::Projectile(float x, float y, Game* game) :
	Actor("res/disparo_jugador.png", x, y, 18, 15, game) {
	vx = 9;
}

void Projectile::update() {
	x = x + vx;
	y = y + vy;
}