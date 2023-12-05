#include "Judas.h"

Judas::Judas(Game* game) : Character(game) {
	aShootingRight = new Animation("res/judas_derecha_disparando.png",
		width, height, 131, 44, 6, 4, false, game);
	aShootingLeft = new Animation("res/judas_left_disparando.png",
		width, height, 124, 44, 6, 4, false, game);
	aShootingUp = new Animation("res/judas_up_disparando.png",
		width, height, 126, 44, 6, 4, false, game);
	aShootingDown = new Animation("res/judas_down_disparando.png",
		width, height, 127, 44, 6, 4, false, game);


	aIdleRight = new Animation("res/judas_derecha.png", width, height,
		132, 44, 20000, 4, true, game);
	aIdleDown = new Animation("res/judas.png", width, height,
		138, 44, 20000, 4, true, game);

	aIdleLeft = new Animation("res/judas_left.png", width, height,
		126, 44, 20000, 4, true, game);
	aIdleUp = new Animation("res/judas_up.png", width, height,
		124, 44, 20000, 4, true, game);

	aRunningRight = new Animation("res/judas_derecha_movimiento.png", width, height,
		131, 44, 6, 4, true, game);
	aRunningDown = new Animation("res/judas_down_movimiento.png", width, height,
		124, 44, 6, 4, true, game);

	aRunningLeft = new Animation("res/judas_left_movimiento.png", width, height,
		124, 44, 6, 4, true, game);
	aRunningUp = new Animation("res/judas_up_movimiento.png", width, height,
		126, 44, 6, 4, true, game);

	explotando = new Animation("res/judas_explotando.png", width, height,
		138, 44, 20000, 4, true, game);

	shootCadence = 30;
	playerSpeed = 6.0;
	lifes = 3;
	damage = 5;
}