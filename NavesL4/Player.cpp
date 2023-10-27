#include "Player.h"

Player::Player(float x, float y, Game* game)
	: Actor("res/isaac.png", x, y, 47, 46, game) {
	audioShoot = new Audio("res/efecto_disparo.wav", false);

	aIdleRight = new Animation("res/isaac_derecha_movimiento.png", width, height,
		131, 41, 6, 4, game);
	animation = aIdleRight;


}

void Player::update() {
	animation->update();
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
