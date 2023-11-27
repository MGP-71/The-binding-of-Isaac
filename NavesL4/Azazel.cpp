#include "Azazel.h"
Azazel::Azazel(float x, float y, Game* game): Enemy("res/enemigos/azazel_singular.png", x, y, game) {

	width=130;
	height = 90;
	state = game->stateMoving;

	aDying = new Animation("res/enemigos/azazel_morir.png", width, height,
		508, 106, 6, 4, false, game);
	aMoving = new Animation("res/enemigos/azazel.png", width, height,
		518, 90, 6, 4, true, game);
	animation = aMoving;

	vx = 0;
	vy = 0;

	enemySpeed = 1;
	vidas = 2;


}

void Azazel::update(Player* p) {
	// Actualizar la animación
	if (shootTime > 0) {
		shootTime--;
	}
	// Actualizar la animación
	bool endAnimation = animation->update();

	// Acabo la animación, no sabemos cual
	if (endAnimation) {
		// Estaba muriendo
		if (state == game->stateDying) {
			state = game->stateDead;
		}
	}

	if (invulnerableTime > 0) {
		invulnerableTime--;
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


ProjectileEnemy* Azazel::shoot(Player* p) {

	if (shootTime == 0) {

		cout << "disparando" << endl;

		// Calcular la dirección hacia el jugador
		float directionX = p->x - x;
		float directionY = p->y - y;
		if (shootCadence == 120) {
			directionX = p->x - x;
			directionY = p->y - y;
			shootCadence = 1;
		}
		else if (shootCadence == 1) {
			directionX = p->x - x + 50;
			directionY = p->y - y + 50;
			shootCadence = 2;
		}
		else if (shootCadence == 2) {
			directionX = p->x - x - 50;
			directionY = p->y - y - 50;
			shootCadence = 120;
		}
		shootTime = shootCadence;
		

		// Normalizar la dirección para obtener un vector de longitud 1
		float length = sqrt(directionX * directionX + directionY * directionY);
		directionX /= length;
		directionY /= length;

		// Crear el proyectil con la dirección ajustada hacia el jugador
		ProjectileEnemy* projectile = new ProjectileEnemy(directionX, directionY, x, y, game);;

		projectile->vx = 5;
		projectile->vy = 5;

		if (x > p->x)
			projectile->vx = -projectile->vx * -directionX * 2;
		else
			projectile->vx = projectile->vx * directionX * 2;
		if (y > p->y)
			projectile->vy = -projectile->vy * -directionY * 2;
		else
			projectile->vy = projectile->vy * directionY * 2;

		return projectile;
	}
	else {
		return NULL;
	}
}