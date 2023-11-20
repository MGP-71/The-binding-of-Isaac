#include "Cain.h"

Cain::Cain( float x, float y, Game* game)
	: Player(x, y, game) {
	orientation = game->orientationDown;
	state = game->stateMoving;

	audioShoot = new Audio("res/efecto_disparo.wav", false);

	aShootingRight = new Animation("res/cain_derecha_disparando.png",
		width, height, 131, 41, 6, 4, false, game);
	aShootingLeft = new Animation("res/cain_left_disparando.png",
		width, height, 124, 40, 6, 4, false, game);
	aShootingUp = new Animation("res/cain_up_disparando.png",
		width, height, 124, 40, 6, 4, false, game);
	aShootingDown = new Animation("res/cain_down_disparando.png",
		width, height, 127, 40, 6, 4, false, game);


	aIdleRight = new Animation("res/cain_derecha.png", width, height,
		132, 39, 20000, 4, true, game);
	aIdleDown = new Animation("res/cain.png", width, height,
		138, 40, 20000, 4, true, game);

	aIdleLeft = new Animation("res/cain_left.png", width, height,
		126, 40, 20000, 4, true, game);
	aIdleUp = new Animation("res/cain_up.png", width, height,
		122, 40, 20000, 4, true, game);

	aRunningRight = new Animation("res/cain_derecha_movimiento.png", width, height,
		131, 41, 6, 4, true, game);
	aRunningDown = new Animation("res/cain_down_movimiento.png", width, height,
		124, 41, 6, 4, true, game);

	aRunningLeft = new Animation("res/cain_left_movimiento.png", width, height,
		124, 40, 6, 4, true, game);
	aRunningUp = new Animation("res/cain_up_movimiento.png", width, height,
		124, 40, 6, 4, true, game);

	animation = aIdleDown;

	playerSpeed = 6.0;


}

void Cain::update() {
	cout << x << endl;
	cout << y << endl;

	if (x + vx < 125) {
		moveX(0);
	}
	else if (x + vx > 760) {
		moveX(0);
	}

	if (y + vy < 70) {
		moveY(0);
	}
	else if (y + vy > 440) {
		moveY(0);
	}

	bool endAnimation = animation->update();

	// Acabo la animación, no sabemos cual
	if (endAnimation) {
		// Estaba disparando
		if (state == game->stateShooting) {
			state = game->stateMoving;
		}
	}

	if (invulnerableTime > 0) {
		invulnerableTime--;
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
	/*
	* if (state == game->stateShooting) {
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
	*/

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
}


void Cain::draw() {
	if (invulnerableTime == 0) {
		animation->draw(x, y);
	}
	else {
		if (invulnerableTime % 10 >= 0 && invulnerableTime % 10 <= 5) {
			animation->draw(x, y);
		}
	}
}


void Cain::moveX(float axis) {
	vx = axis * playerSpeed;
}

void Cain::moveY(float axis) {
	vy = axis * playerSpeed;
}

void Cain::loseLife() {
	if (invulnerableTime <= 0) {
		if (lifes > 0) {
			lifes--;
			invulnerableTime = 100;
			// 100 actualizaciones 
		}
	}
}
Projectile* Cain::shoot() {

	if (shootTime == 0) {
		state = game->stateShooting;
		//audioShoot->play();
		shootTime = shootCadence;
		aShootingLeft->currentFrame = 0; //"Rebobinar" animación
		aShootingRight->currentFrame = 0; //"Rebobinar" 
		aShootingUp->currentFrame = 0; //"Rebobinar" animación
		aShootingDown->currentFrame = 0; //"Rebobinar" animación
		Projectile* projectile = new Projectile(x, y, game);
		if (orientation == game->orientationUp) {
			animation = aShootingUp;
			projectile->vx = 0;
			projectile->vy = -9;
		}
		else if (orientation == game->orientationDown) {
			animation = aShootingDown;
			projectile->vx = 0;
			projectile->vy = 9;
		}
		else if (orientation == game->orientationLeft) {
			animation = aShootingLeft;
			projectile->vy = 0;
			projectile->vx = -9; // Invertir
		}
		else if (orientation == game->orientationRight) {
			animation = aShootingRight;
			projectile->vy = 0;
			projectile->vx = 9; // Invertir
		}
		return projectile;

	}
	else {
		return NULL;
	}
}

