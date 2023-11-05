#include "Enemy.h"

Enemy::Enemy(float x, float y, Game* game)
	: Actor("res/enemigos/fatty.png", x, y, 43, 56, game) {

	state = game->stateMoving;

	aDying = new Animation("res/enemigos/fatty_morir.png", width, height,
		175, 56, 6, 4, false, game);
	aMoving = new Animation("res/enemigos/fatty_movimiento.png", width, height,
		175, 56, 6, 4, true, game);
	animation = aMoving;

	vx = 1;
}

void Enemy::update() {
	// Actualizar la animación
	bool endAnimation = animation->update();

	// Acabo la animación, no sabemos cual
	if (endAnimation) {
		// Estaba muriendo
		if (state == game->stateDying) {
			state = game->stateDead;
		}
	}


	if (state == game->stateMoving) {
		animation = aMoving;
	}
	if (state == game->stateDying) {
		animation = aDying;
	}

	if (state != game->stateDying) {
		vx = -1;
		x = x + vx;
	}


}

void Enemy::draw() {
	animation->draw(x, y);
}

void Enemy::impacted() {
	if (state != game->stateDying) {
		state = game->stateDying;
	}
}
