#include "Azazel.h"
Azazel::Azazel(float x, float y, Game* game): Enemy("res/enemigos/azazel.png", x, y, game) {

	state = game->stateMoving;

	aDying = new Animation("res/enemigos/azazel.png", width, height,
		121, 44, 6, 2, false, game);
	aMoving = new Animation("res/enemigos/azazel.png", width, height,
		121, 44, 6, 2, true, game);
	animation = aMoving;

	vx = 0;
	vy = 0;

	enemySpeed = 1;
	vidas = 10;


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