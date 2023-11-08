#include "Monoojo.h"

Monoojo::Monoojo(float x, float y, Game* game)
	: Enemy("res/enemigos/monoojo.png", x, y, game) {

	state = game->stateMoving;

	aDying = new Animation("res/enemigos/monoojo_morir.png", width, height,
		138, 40, 6, 4, false, game);
	aMoving = new Animation("res/enemigos/monoojo_movimiento.png", width, height,
		124, 41, 6, 4, true, game);
	animation = aMoving;

	vx = enemySpeed;


}
