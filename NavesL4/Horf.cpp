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


}

void Horf::update(Player* p) {
	// Actualizar la animación
	if (shootTime > 0) {
		shootTime--;
	}
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
	
}

ProjectileEnemy* Horf::shoot() {
	if (shootTime == 0) {
		cout << "disparando" << endl;
		shootTime = shootCadence;
		ProjectileEnemy* projectile;
		if (vx > 0) {
			projectile = new ProjectileEnemy(1, x, y, game);
		}
		else {
			projectile = new ProjectileEnemy(-1, x, y, game);
		}
		return projectile;
	}
	else {
		return NULL;
	}
}