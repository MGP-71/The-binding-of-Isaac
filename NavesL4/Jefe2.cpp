#include "Jefe2.h"
Jefe2::Jefe2(float x, float y, Game* game) : Enemy("res/enemigos/jefe2.png", x, y, game) {

	width = 42;
	height = 42;
	state = game->stateMoving;

	aDying = new Animation("res/enemigos/jefe2_muriendo.png", width, height,
		167, 40, 6, 4, false, game);
	aMoving = new Animation("res/enemigos/jefe2_corriendo.png", width, height,
		166, 40, 6, 4, true, game);
	animation = aMoving;

	vx = 0;
	vy = 0;

	enemySpeed = 3;
	vidas = 8;


}

void Jefe2::update(Player* p) {
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

		Uint32 tiempoActual = SDL_GetTicks();

		// Verificar si el enemigo está en la misma coordenada x o y que el jugador
		if (x == p->x || y == p->y) {
			// Aumentar la velocidad
			enemySpeed = 8; // Puedes ajustar el factor de aumento según tus necesidades
		}
		else {
			enemySpeed = 2;
		}

		// Mover hacia el jugador
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


