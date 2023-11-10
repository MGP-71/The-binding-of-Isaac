#include "Monoojo.h"

Monoojo::Monoojo(float x, float y, Game* game)
	: Enemy("res/enemigos/monoojo.png", x, y, game) {

	state = game->stateMoving;

	aDying = new Animation("res/enemigos/monoojo_morir.png", width, height,
		138, 40, 6, 4, false, game);
	aMoving = new Animation("res/enemigos/monoojo_movimiento.png", width, height,
		124, 41, 6, 4, true, game);
	animation = aMoving;

	vx = enemySpeed;


}

void Monoojo::update(Player* p) {
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
ProjectileEnemy* Monoojo::shoot(Player* p) {

	if (shootTime == 0) {

		shootTime = shootCadence;

		// Calcular la dirección hacia el jugador
		float directionX = p->x - x;
		float directionY = p->y - y;

		// Normalizar la dirección para obtener un vector de longitud 1
		float length = sqrt(directionX * directionX + directionY * directionY);
		directionX /= length;
		directionY /= length;

		// Crear el proyectil con la dirección ajustada hacia el jugador
		ProjectileEnemy* projectile = new ProjectileEnemy(directionX, directionY, x, y, game);
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