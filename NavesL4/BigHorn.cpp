#include "BigHorn.h"

BigHorn::BigHorn(float x, float y, Game* game) : Enemy("res/enemigos/horn.png", x, y, game) {

	width = 192;
	height = 168;
	state = game->stateMoving;

	aDying = new Animation("res/enemigos/horn_muriendo.png", width, height,
		654, 168, 6, 3, false, game);
	aMoving = new Animation("res/enemigos/horn_movimiento.png", width, height,
		731, 168, 6, 4, true, game);
	animation = aMoving;

	vx = 0;
	vy = 0;

	enemySpeed = 0;
	vidas = 2;


}

void BigHorn::update(Player* p) {
	// Actualizar la animaci�n
	if (shootTime > 0) {
		shootTime--;
	}
	// Actualizar la animaci�n
	bool endAnimation = animation->update();

	// Acabo la animaci�n, no sabemos cual
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
	counterGenerate--;
}


ProjectileEnemy* BigHorn::shoot(Player* p) {

	if (shootTime == 0) {

		cout << "disparando" << endl;

		// Calcular la direcci�n hacia el jugador
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


		// Normalizar la direcci�n para obtener un vector de longitud 1
		float length = sqrt(directionX * directionX + directionY * directionY);
		directionX /= length;
		directionY /= length;

		// Crear el proyectil con la direcci�n ajustada hacia el jugador
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

Monoojo* BigHorn::generateEnemy(int x, int y, Game* g) {
	if (counterGenerate <= 0) {
		counterGenerate = 100;
		return new Monoojo(x, y, g);
	}
	return NULL;
}