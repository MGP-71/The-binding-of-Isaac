#include "Enemy.h"

Enemy::Enemy(string filename, float x, float y, Game* game)
	: Actor(filename, x, y, 43, 56, game) {

	state = game->stateMoving;

	aDying = new Animation("res/enemigos/fatty_morir.png", width, height,
		175, 56, 6, 4, false, game);
	aMoving = new Animation("res/enemigos/fatty_movimiento.png", width, height,
		175, 56, 6, 4, true, game);
	animation = aMoving;

	vx = enemySpeed;

	
}

void Enemy::update(Player* p) {
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

	if (state == game->stateMoving) {
		animation = aMoving;
	}
	if (state == game->stateDying) {
		animation = aDying;
	}
	
	 
	if (state == game->stateMoving) {
		if (x > p->x)
			vx = -enemySpeed;
		else
			vx = enemySpeed;
		if (y > p->y)
			vy = -enemySpeed;
		else
			vy = enemySpeed;
	}
	else {
		vx = 0;
		vy = 0;
	}
}

void Enemy::draw(float scrollX, float scrollY) {
	animation->draw(x - scrollX, y - scrollY);
}

void Enemy::impacted() {
	if (state != game->stateDying) {
		state = game->stateDying;
	}
}