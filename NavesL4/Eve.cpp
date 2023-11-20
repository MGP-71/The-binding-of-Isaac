#include "Eve.h"

Eve::Eve(Game* game) : Character(game) {
	aShootingRight = new Animation("res/eve_derecha_disparando.png",
		width, height, 131, 41, 6, 4, false, game);
	aShootingLeft = new Animation("res/eve_left_disparando.png",
		width, height, 124, 40, 6, 4, false, game);
	aShootingUp = new Animation("res/eve_up_disparando.png",
		width, height, 126, 40, 6, 4, false, game);
	aShootingDown = new Animation("res/eve_down_disparando.png",
		width, height, 127, 40, 6, 4, false, game);


	aIdleRight = new Animation("res/eve_derecha.png", width, height,
		132, 39, 20000, 4, true, game);
	aIdleDown = new Animation("res/eve.png", width, height,
		138, 40, 20000, 4, true, game);

	aIdleLeft = new Animation("res/eve_left.png", width, height,
		126, 40, 20000, 4, true, game);
	aIdleUp = new Animation("res/eve_up.png", width, height,
		124, 40, 20000, 4, true, game);

	aRunningRight = new Animation("res/eve_derecha_movimiento.png", width, height,
		131, 41, 6, 4, true, game);
	aRunningDown = new Animation("res/eve_down_movimiento.png", width, height,
		124, 41, 6, 4, true, game);

	aRunningLeft = new Animation("res/eve_left_movimiento.png", width, height,
		124, 40, 6, 4, true, game);
	aRunningUp = new Animation("res/eve_up_movimiento.png", width, height,
		126, 40, 6, 4, true, game);

	shootCadence = 30;
	playerSpeed = 6.0;
	lifes = 3;
}