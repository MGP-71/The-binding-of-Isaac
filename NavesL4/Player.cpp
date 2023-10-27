#include "Player.h"

Player::Player(float x, float y, Game* game)
	: Actor("res/isaac.png", x, y, 47, 46, game) {
	audioShoot = new Audio("res/efecto_disparo.wav", false);

	aIdleRight = new Animation("res/isaac_derecha.png", width, height,
		132, 39, 20000, 4, game);
	aIdleDown = new Animation("res/isaac.png", width, height,
		138, 46, 20000, 4, game);

	/*aIdleLeft = new Animation("res/isaac_izquierda.png", width, height,
		132, 39, 20000, 4, game);
	aIdleUp = new Animation("res/isaac_up.png", width, height,
		138, 46, 20000, 4, game);*/

	aRunningRight = new Animation("res/isaac_derecha_movimiento.png", width, height,
		131, 41, 6, 4, game);
	aRunningDown = new Animation("res/isaac_down_movimiento.png", width, height,
		124, 41, 6, 4, game);

	/*aRunningLeft = new Animation("res/isaac_izquierda_movimiento.png", width, height,
		131, 41, 6, 4, game);
	aRunningUp = new Animation("res/isaac_up_movimiento.png", width, height,
		124, 41, 6, 4, game);*/

	animation = aIdleRight;


}

void Player::update() {
	animation->update();

	if (vx > 0) {
		animation = aRunningRight;
	}
	if (vx < 0) {
		animation = aRunningLeft;
	}
	if (vx == 0) {
		animation = aIdleRight;
	}

	if (vy < 0) {
		animation = aRunningUp;
	}
	if (vy > 0) {
		animation = aRunningDown; 
	}
	/*if (vy == 0) {
		animation = aIdleDown;
	}*/

	if (shootTime > 0) {
		shootTime--;
	}

	x = x + vx;
	y = y + vy;
}

void Player::draw() {
	animation->draw(x, y);
}

void Player::moveX(float axis) {
	vx = axis * 3;
}

void Player::moveY(float axis) {
	vy = axis * 3;
}

Projectile* Player::shoot() {

	if (shootTime == 0) {
		audioShoot->play();
		shootTime = shootCadence;
		return new Projectile(x, y, game);
	}
	else {
		return NULL;
	}
}
