#include "Fatty.h"
Fatty::Fatty(float x, float y, Game* game)
	: Enemy("res/enemigos/fatty.png", x, y, game) {

	state = game->stateMoving;

	aDying = new Animation("res/enemigos/fatty_morir.png", width, height,
		175, 56, 6, 4, false, game);
	aMoving = new Animation("res/enemigos/fatty_movimiento.png", width, height,
		175, 56, 6, 4, true, game);
	animation = aMoving;

	vx = enemySpeed;
	/*
	* vxIntelligence = -1;
	vx = vxIntelligence;
	*/

}
