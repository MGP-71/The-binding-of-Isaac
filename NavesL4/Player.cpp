#include "Player.h"

Player::Player(float x, float y, Game* game, Character* cha) : Actor("", x, y, 47, 46, game) {
	orientation = game->orientationDown;
	state = game->stateMoving;


	/*audioShoot = new Audio("res/shot.wav", false);
	audioGolpeado = new Audio("res/hurt.wav", false);
	audioMuerte = new Audio("res/muerte.wav", false);*/


	character = cha;
	animation = character->aIdleDown;
	//setAnimations();
}

void Player::update() {
	/*cout << x << endl;
	cout << y << endl;*/

	if (x + vx < 125) {
		moveX(0);
	} else if (x + vx > 760) {
		moveX(0);
	}
	
	if (y + vy < 70) {
		moveY(0);
	} else if ( y + vy > 440) {
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
	
	if (state == game->stateExplotando) {
		animation = character->explotando;
	}
	if (state == game->stateMoving) {
		if (vx != 0) {
			if (orientation == game->orientationRight) {
				animation = character->aRunningRight;
			}
			if (orientation == game->orientationLeft) {
				animation = character->aRunningLeft;
			}
		}

		if (vx == 0) {
			if (orientation == game->orientationRight) {
				animation = character->aIdleRight;
			}
			if (orientation == game->orientationLeft) {
				animation = character->aIdleLeft;
			}
			if (orientation == game->orientationUp) {
				animation = character->aIdleUp;
			}
			if (orientation == game->orientationDown) {
				animation = character->aIdleDown;
			}
		}
		if (vy != 0) {
			if (orientation == game->orientationUp) {
				animation = character->aRunningUp;
			}
			if (orientation == game->orientationDown) {
				animation = character->aRunningDown;
			}
		}
	}

	if (shootTime > 0) {
		shootTime--;
	}
}

void Player::draw() {
	if (invulnerableTime == 0) {
		animation->draw(x, y);
	}
	else {
		if (invulnerableTime % 10 >= 0 && invulnerableTime % 10 <= 5) {
			animation->draw(x, y);
		}
	}
}

void Player::moveX(float axis) {
	vx = axis * character->playerSpeed;
}

void Player::moveY(float axis) {
	vy = axis * character->playerSpeed;
}

void Player::loseLife() {
	if (invulnerableTime <= 0) {
		if (character->lifes > 0) {
			//audioGolpeado->play();
			character->lifes--;
			invulnerableTime = 100;
			// 100 actualizaciones 
		}
		if (character->lifes == 0) {
			//audioMuerte->play();
			character->lifes--;
			invulnerableTime = 100;
		}
	}
}

Projectile* Player::shoot() {

	if (shootTime == 0) {
		state = game->stateShooting;
		//audioShoot->play();
		shootTime = character->shootCadence;
		character->aShootingLeft->currentFrame = 0; //"Rebobinar" animación
		character->aShootingRight->currentFrame = 0; //"Rebobinar" 
		character->aShootingUp->currentFrame = 0; //"Rebobinar" animación
		character->aShootingDown->currentFrame = 0; //"Rebobinar" animación
		Projectile* projectile = new Projectile(x, y, game);
		if (orientation == game->orientationUp) {
			animation = character->aShootingUp;
			projectile->vx = 0;
			projectile->vy = -9;
		} else if (orientation == game->orientationDown) {
			animation = character->aShootingDown;
			projectile->vx = 0;
			projectile->vy = 9;
		} else if (orientation == game->orientationLeft) {
			animation = character->aShootingLeft;
			projectile->vy = 0;
			projectile->vx = -9; // Invertir
		} else if (orientation == game->orientationRight) {
			animation = character->aShootingRight;
			projectile->vy = 0;
			projectile->vx = 9; // Invertir
		}
		return projectile;

	}
	else {
		return NULL;
	}
}

void Player::setAnimations() {
	character->aShootingRight = new Animation("res/isaac_derecha_disparando.png",
		width, height, 131, 41, 6, 4, false, game);
	character->aShootingLeft = new Animation("res/isaac_left_disparando.png",
		width, height, 124, 40, 6, 4, false, game);
	character->aShootingUp = new Animation("res/isaac_up_disparando.png",
		width, height, 124, 40, 6, 4, false, game);
	character->aShootingDown = new Animation("res/isaac_down_disparando.png",
		width, height, 127, 40, 6, 4, false, game);


	character->aIdleRight = new Animation("res/isaac_derecha.png", width, height,
		132, 39, 20000, 4, true, game);
	character->aIdleDown = new Animation("res/isaac.png", width, height,
		138, 40, 20000, 4, true, game);

	character->aIdleLeft = new Animation("res/isaac_left.png", width, height,
		126, 40, 20000, 4, true, game);
	character->aIdleUp = new Animation("res/isaac_up.png", width, height,
		122, 40, 20000, 4, true, game);

	character->aRunningRight = new Animation("res/isaac_derecha_movimiento.png", width, height,
		131, 41, 6, 4, true, game);
	character->aRunningDown = new Animation("res/isaac_down_movimiento.png", width, height,
		124, 41, 6, 4, true, game);

	character->aRunningLeft = new Animation("res/isaac_left_movimiento.png", width, height,
		124, 40, 6, 4, true, game);
	character->aRunningUp = new Animation("res/isaac_up_movimiento.png", width, height,
		124, 40, 6, 4, true, game);

	

}