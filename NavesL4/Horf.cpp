#include "Horf.h"

Horf::Horf(float x, float y, Game* game)
	: Enemy("res/enemigos/horf.png", x, y, game) {

	state = game->stateMoving;

	aDying = new Animation("res/enemigos/horf_dying.png", width, height,
		122, 36, 6, 4, false, game);
	aMoving = new Animation("res/enemigos/horf.png", width, height,
		68, 36, 6, 2, true, game);
	animation = aMoving;

	vx = 0;
	vy = 0;

	enemySpeed = 0;
	vidas = 2;


}

void Horf::update(Player* p) {
	
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

	if (invulnerableTime > 0) {
		invulnerableTime--;
	}
	// Establecer velocidad
	if (state != game->stateDying) {
		// no está muerto y se ha quedado parado
		if (vx == 0) {
			vxIntelligence = vxIntelligence * -1;
			vx = vxIntelligence;
		}
		if (outRight) {
			// mover hacia la izquierda vx tiene que ser negativa
			if (vxIntelligence > 0) {
				vxIntelligence = vxIntelligence * -1;
			}
			vx = vxIntelligence;
		}
		if (outLeft) {
			// mover hacia la derecha vx tiene que ser positiva
			if (vxIntelligence < 0) {
				vxIntelligence = vxIntelligence * -1;
			}
			vx = vxIntelligence;
		}

	}
	else {
		vx = 0;
	}
	// Actualizar la animación
	if (shootTime > 0) {
		shootTime--;
	}

	
}


ProjectileEnemy* Horf::shoot(Player* p) {
	
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