#include "Player.h"

Player::Player(float x, float y, Game* game)
	: Actor("res/isaac.png", x, y, 47, 46, game) {
	orientation = game->orientationDown;
	state = game->stateMoving;

	audioShoot = new Audio("res/efecto_disparo.wav", false);

	aShootingRight = new Animation("res/isaac_derecha_disparando.png",
		width, height, 131, 41, 6, 4, false, game);
	aShootingLeft = new Animation("res/isaac_left_disparando.png",
		width, height, 124, 40, 6, 4, false, game);
	aShootingUp = new Animation("res/isaac_up_disparando.png",
		width, height, 124, 40, 6, 4, false, game);
	aShootingDown = new Animation("res/isaac_down_disparando.png",
		width, height, 127, 40, 6, 4, false, game);


	aIdleRight = new Animation("res/isaac_derecha.png", width, height,
		132, 39, 20000, 4, true, game);
	aIdleDown = new Animation("res/isaac.png", width, height,
		138, 40, 20000, 4, true, game);

	aIdleLeft = new Animation("res/isaac_left.png", width, height,
		126, 40, 20000, 4, true,game);
	aIdleUp = new Animation("res/isaac_up.png", width, height,
		122, 40, 20000, 4, true, game);

	aRunningRight = new Animation("res/isaac_derecha_movimiento.png", width, height,
		131, 41, 6, 4, true,game);
	aRunningDown = new Animation("res/isaac_down_movimiento.png", width, height,
		124, 41, 6, 4, true, game);

	aRunningLeft = new Animation("res/isaac_left_movimiento.png", width, height,
		124, 40, 6, 4, true,game);
	aRunningUp = new Animation("res/isaac_up_movimiento.png", width, height,
		124, 40, 6, 4, true, game);

	animation = aIdleDown;


}

void Player::update() {
	bool endAnimation = animation->update();

	// Acabo la animación, no sabemos cual
	if (endAnimation) {
		// Estaba disparando
		if (state == game->stateShooting) {
			state = game->stateMoving;
		}
	}


	// Establecer orientación
	if (vx > 0) {
		orientation = game->orientationRight;
	}
	if (vx < 0) {
		orientation = game->orientationLeft;
	}

	if (vy > 0) {
		orientation = game->orientationDown;
	}
	if (vy < 0) {
		orientation = game->orientationUp;
	}	


	// Selección de animación basada en estados
	if (state == game->stateShooting) {
		if (orientation == game->orientationRight) {
			animation = aShootingRight;
		}
		if (orientation == game->orientationLeft) {
			animation = aShootingLeft;
		}
		if (orientation == game->orientationUp) {
			animation = aShootingUp;
		}
		if (orientation == game->orientationDown) {
			animation = aShootingDown;
		}
	}
	if (state == game->stateMoving) {
		if (vx != 0) {
			if (orientation == game->orientationRight) {
				animation = aRunningRight;
			}
			if (orientation == game->orientationLeft) {
				animation = aRunningLeft;
			}
		}
		
		if (vx == 0) {
			if (orientation == game->orientationRight) {
				animation = aIdleRight;
			}
			if (orientation == game->orientationLeft) {
				animation = aIdleLeft;
			}
			if (orientation == game->orientationUp) {
				animation = aIdleUp;
			}
			if (orientation == game->orientationDown) {
				animation = aIdleDown;
			}
		}
		if (vy != 0) {
			if (orientation == game->orientationUp) {
				animation = aRunningUp;
			}
			if (orientation == game->orientationDown) {
				animation = aRunningDown;
			}
		}
	}

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
		state = game->stateShooting;
		audioShoot->play();
		shootTime = shootCadence;
		aShootingLeft->currentFrame = 0; //"Rebobinar" animación
		aShootingRight->currentFrame = 0; //"Rebobinar" 
		aShootingUp->currentFrame = 0; //"Rebobinar" animación
		aShootingDown->currentFrame = 0; //"Rebobinar" animación
		Projectile* projectile = new Projectile(x, y, game);
		if (orientation == game->orientationUp) {
			projectile->vx = 0;
			projectile->vy = -9;
		}
		if (orientation == game->orientationDown) {
			projectile->vx = 0;
			projectile->vy = 9;
		}
		if (orientation == game->orientationLeft) {
			projectile->vy = 0;
			projectile->vx = -9; // Invertir
		}
		if (orientation == game->orientationRight) {
			projectile->vy = 0;
			projectile->vx = 9; // Invertir
		}
		return projectile;

	}
	else {
		return NULL;
	}
}
