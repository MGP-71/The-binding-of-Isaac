#include "Horf.h"

Horf::Horf(float x, float y, Game* game)
	: Enemy("res/enemigos/horf.png", x, y, game) {

	state = game->stateMoving;

	aDying = new Animation("res/enemigos/horf_dying.png", width, height,
		122, 36, 6, 4, false, game);
	aMoving = new Animation("res/enemigos/horf.png", width, height,
		68, 36, 6, 2, true, game);
	animation = aMoving;

	vx = 0;
	vy = 0;

	enemySpeed = 0;


}


