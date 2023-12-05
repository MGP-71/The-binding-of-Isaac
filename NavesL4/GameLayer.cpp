#include "GameLayer.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <list>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iterator>


const Uint32 duracionInvencibilidad = 3000;
Uint32 tiempoInicioInvencibilidad = 0;

GameLayer::GameLayer(Game* game, int personaje)
	: Layer(game) {
	//llama al constructor del padre : Layer(renderer)
	this->personaje = personaje;
	

	init();


}

void GameLayer::init() {
	space = new Space(0);
	scrollX = 0;
	scrollY = 0;
	tiles.clear();
	fuegos.clear();
	rocas.clear();
	doors.clear();
	hearts.clear();
	bombs.clear();
	keys.clear();
	corazones.clear();
	objetos.clear();
	roomsCleared.clear();
	pills.clear();
	objetosTotales = { 1, 2, 3 };

	habitacionVertical = 0;
	habitacionHorizontal = 0;

	audioBackground = new Audio("res/musica_ambiente.mp3", true);
	audioFinal = new Audio("res/final.wav", true);
	audioPill = new Audio("res/pill.wav", false);
	audioBomb = new Audio("res/Pestilence_headexplode.wav", false);
	audioObject = new Audio("res/Mom_Evil_laugh.wav", false); 
	audioKillEnemy = new Audio("res/Hitting_wall.wav", false);
	audioChangeFloor = new Audio("res/Mom.wav", false);
	audioShoot = new Audio("res/shot.wav", false);
	audioGolpeado = new Audio("res/hurt.wav", false);
	audioMuerte = new Audio("res/muerte.wav", false);

	audioBackground->play();


	background = new Background("res/fondos/habitacion.png", WIDTH * 0.5, HEIGHT * 0.5, -1, game);
	textBombs = new Text("hola", WIDTH * 0.05, HEIGHT * 0.05, game);
	textBombs->content = to_string(0);
	textKeys = new Text("hola", WIDTH * 0.05, HEIGHT * 0.1, game);
	textKeys->content = to_string(0);
	objConseguido = new Text(" ", WIDTH * 0.1, HEIGHT * 0.95, game);
	textActivo = new Text(" ", WIDTH * 0.5, HEIGHT * 0.95, game);

	speedActor = new Actor("res/Speed_Stat_Icon.png", WIDTH * 0.02, HEIGHT * 0.25, 19, 12, game);
	textSpeed = new Text("hola", WIDTH * 0.06, HEIGHT * 0.25, game);
	textSpeed->content = to_string(0);
	damageActor = new Actor("res/Damage_Stat_Icon.png", WIDTH * 0.02, HEIGHT * 0.30, 13, 13, game);
	textDamage = new Text("hola", WIDTH * 0.06, HEIGHT * 0.30, game);
	textDamage->content = to_string(0);
	tearsActor = new Actor("res/Tears_Stat_Icon.png", WIDTH * 0.02, HEIGHT * 0.35, 23, 13, game);
	textTears = new Text("hola", WIDTH * 0.06, HEIGHT * 0.35, game);
	textTears->content = to_string(0);

	enemies.clear(); // Vaciar por si reiniciamos el juego
	projectiles.clear(); // Vaciar por si reiniciamos el juego
	horfEnemies.clear(); // Vaciar por si reiniciamos el juego
	projectilesEnemy.clear(); // Vaciar por si reiniciamos el juego
	monoojoEnemies.clear();
	nKeys = 0;
	nBombs = 0;
	nPills = 0;
	activeBomb = NULL;
	azazel = NULL;
	jefe2 = NULL;
	bigHorn = NULL;
	trapdoor = NULL;
	trofeo = NULL;

	chooseCharater();

	nameFile = "res/fondos/1_0_0.txt";
	
	loadMap(nameFile);
	
	floor = 1;
	actualizarVidas();
	actualizarBombas();
	actualizarLlaves();
	actualizarPills();

	reproduccionesBomba = 1;
	won = false;

	srand(static_cast<unsigned int>(time(0)));
}

void GameLayer::processControls() {
	// obtener controles
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		keysToControls(event);
	}
	//procesar controles
	//procesar controles
	// Disparar
	if (controlShoot) {
		Projectile* newProjectile = player->shoot();
		if (newProjectile != NULL) {
			audioShoot->play();
			space->addDynamicActor(newProjectile);
			projectiles.push_back(newProjectile);
		}

	}

	// Eje X
	if (controlMoveX > 0) {
		if (player->x < 760)
			player->moveX(1);
	}
	else if (controlMoveX < 0) {
		if (player->x > 125)
			player->moveX(-1);
	}
	else {
		player->moveX(0);
	}

	// Eje Y
	if (controlMoveY > 0) {
		if (player->y < 440)
			player->moveY(1);

	}
	else if (controlMoveY < 0) {
		if (player->y > 70)
			player->moveY(-1);
	}
	else {
		player->moveY(0);
	}

}

void GameLayer::update() {

	
	space->update();
	background->update();
	player->update();

	for (auto const& enemy : enemies) {
		enemy->update(player);
	}
	for (auto const& enemy : fuegos) {
		enemy->update(player);
	}
	for (auto const& enemy : horfEnemies) {
		enemy->update(player);
	}
	for (auto const& enemy : monoojoEnemies) {
		enemy->update(player);
	}
	for (auto const& projectile : projectiles) {
		projectile->update();
	}

	for (auto const& projectile : projectilesEnemy) {
		projectile->update();
	}

	for (auto const& enemy : horfEnemies) {
		ProjectileEnemy* newProjectile = enemy->shoot(player);
		if (newProjectile != NULL) {
			projectilesEnemy.push_back(newProjectile);
			space->addDynamicActor(newProjectile);
		}
	}
	for (auto const& enemy : monoojoEnemies) {
		ProjectileEnemy* newProjectile = enemy->shoot(player);
		if (newProjectile != NULL) {
			projectilesEnemy.push_back(newProjectile);
			space->addDynamicActor(newProjectile);
		}
	}
	
	if (azazel != NULL) {
		ProjectileEnemy* projAaza = azazel->shoot(player);
		if (projAaza != NULL) {
			projectilesEnemy.push_back(projAaza);
			space->addDynamicActor(projAaza);
		}
		if (azazel->state == game->stateDead) {
			azazel = NULL;
		}
	}

	if (jefe2 != NULL) {
		if (jefe2->state == game->stateDead) {
			jefe2 = NULL;
		}
	}

	if (bigHorn != NULL) {
		Monoojo* enemyS = bigHorn->generateEnemy(bigHorn->x, bigHorn->y, game);
		if (enemyS != NULL) {
			//enemyS->y = enemyS->y - enemyS->height / 2;
			enemies.push_back(enemyS);
			monoojoEnemies.push_back(enemyS);
			space->addDynamicActor(enemyS);
		}
		if (bigHorn->state == game->stateDead) {
			audioBackground->stop();
			audioFinal->play();
			bigHorn = NULL;
		}
	}
	
	
	//Abrir puertas
	bool allDead = true;
	for (auto const& enemy : enemies) {
		if (enemy->state != game->stateDead) {
			allDead = false;
		}
	}
	if (allDead) {
		puertasAbiertas = true;
		for (auto const& door : space->dynamicActors) {
			if (door->filename.compare("res/puerta_up_cerrada.png") == 0) {
				door->filename = "res/puerta_up_abierta.png";
				door->texture = game->getTexture(door->filename);
			}
			if (door->filename.compare("res/door_boss_cerrada.png") == 0) {
				door->filename = "res/door_boss_abierta.png";
				door->texture = game->getTexture(door->filename);
			}
			else if (door->filename.compare("res/puerta_izquierda_cerrada.png") == 0) {
				door->filename = "res/puerta_izquierda_abierta.png";
				door->texture = game->getTexture(door->filename);
			}
			else if (door->filename.compare("res/puerta_derecha_cerrada.png") == 0) {
				door->filename = "res/puerta_derecha_abierta.png";
				door->texture = game->getTexture(door->filename);
			}
			else if (door->filename.compare("res/puerta_abajo_cerrada.png") == 0) {
				door->filename = "res/puerta_abajo_abierta.png";
				door->texture = game->getTexture(door->filename);
			}
		}

		////para que se pueda pasar por las puertas al estar abiertas

		/*for (auto const& door : doors) {
			space->removeDynamicActor(door);
		}
		for (auto const& door : doors) {
			space->addStaticActor(door);
		}*/


	}
	// Colisiones
	for (auto const& enemy : enemies) {
		if (player->isOverlap(enemy)) {
			if (!playerCharacter->invencibilidad) {
				audioGolpeado->play();
				player->loseLife();
				actualizarVidas();
				if (player->character->lifes <= 0) {
					audioMuerte->play();
					init();
					return;
				}
			}
			else {
				enemy->impacted(1);
			}
			
			return; // Cortar el for
		}
	}

	//Colisiones jugador-fuego
	for (auto const& fuego : fuegos) {
		if (player->isOverlap(fuego)) {
			if (!playerCharacter->invencibilidad) {
				audioGolpeado->play();
				player->loseLife();
				actualizarVidas();
				if (player->character->lifes <= 0) {
					audioMuerte->play();
					init();
					return;
				}
				return;
			}
		}
	}

	list<Tile*> deleteRocas;

	if (playerCharacter->rompeRocas) {
		for (Tile* tile : rocas) {
			space->addDynamicActor(tile);
		}
	}
	else {
		for (Tile* tile : rocas) {
			space->removeDynamicActor(tile);
			space->addStaticActor(tile);
		}
	}

	if (playerCharacter->rompeRocas) {
		// Marcar elementos para eliminación
		for (auto const& tile : rocas) {
			if (player->isOverlap(tile)) {
				deleteRocas.push_back(tile);
			}
		}

		// Eliminar elementos marcados
		for (auto const& b : deleteRocas) {
			rocas.remove(b);
			space->removeDynamicActor(b);
			space->removeStaticActor(b);
			delete b;
		}

		deleteRocas.clear();
	}
	

	list<Tile*> deleteObjetos;
	//Colisiones jugador-objetos
	for (auto const& obj : objetos) {
		if (player->isOverlap(obj)) {
			bool pInList = std::find(deleteObjetos.begin(),
				deleteObjetos.end(),
				obj) != deleteObjetos.end();

			if (!pInList) {
				objetoConseguido(obj);
				deleteObjetos.push_back(obj);
				for (auto const& b : deleteObjetos) {
					objetos.remove(b);
					space->removeDynamicActor(b);
					delete b;
				}
				deleteObjetos.clear();

			}

			return;
		}
	}

	list<Tile*> deleteBombs;
	//Colisiones jugador-bomba
	for (auto const& bomb : bombs) {
		if (player->isOverlap(bomb)) {
			bool pInList = std::find(deleteBombs.begin(),
				deleteBombs.end(),
				bomb) != deleteBombs.end();

			if (!pInList) {
				deleteBombs.push_back(bomb);
				nBombs++;
				for (auto const& b : deleteBombs) {
					bombs.remove(b);
					space->removeDynamicActor(b);
					delete b;
					actualizarBombas();
				}
				deleteBombs.clear();
			
			}
			
			return;
		}
	}

	list<Tile*> deleteCorazones;
	//Colisiones jugador-corazon
	for (auto const& corazon : corazones) {
		if (player->isOverlap(corazon) && hearts.size() < 5) {
			bool pInList = std::find(deleteCorazones.begin(),
				deleteCorazones.end(),
				corazon) != deleteCorazones.end();

			if (!pInList) {
				deleteCorazones.push_back(corazon);
				for (auto const& b : deleteCorazones) {
					corazones.remove(b);
					space->removeDynamicActor(b);
					delete b;
					player->character->lifes++;
					actualizarVidas();
				}
				deleteCorazones.clear();

			}

			return;
		}
	}

	list<Tile*> deleteKeys;
	//Colisiones jugador-llave
	for (auto const& key : keys) {
		if (player->isOverlap(key)) {
			bool pInList = std::find(deleteKeys.begin(),
				deleteKeys.end(),
				key) != deleteKeys.end();

			if (!pInList) {
				deleteKeys.push_back(key);
				nKeys++;
				for (auto const& b : deleteKeys) {
					keys.remove(b);
					space->removeDynamicActor(b);
					delete b;
					actualizarLlaves();
				}
				deleteKeys.clear();

			}

			return;
		}
	}
	list<Tile*> deletePills;
	//Colisiones jugador-pildora
	for (auto const& pill : pills) {
		if (player->isOverlap(pill) && pillsActor == NULL) {
			bool pInList = std::find(deletePills.begin(),
				deletePills.end(),
				pill) != deletePills.end();

			if (!pInList) {
				deletePills.push_back(pill);
				nPills++;
				for (auto const& b : deletePills) {
					pills.remove(b);
					space->removeDynamicActor(b);
					delete b;
					actualizarPills();
				}
				deletePills.clear();

			}

			return;
		}
	}
	
	bool isOverlap = false;
	//Colisiones Puerta-Personaje
	for (auto const& tile : space->dynamicActors) {
		//si el personaje overlapea la puerta
		if (tile->isOverlap(player)) {
			saveRoom(nameFile);
			//y la puerta es la de arriba y está abierta y el personaje está mirando pa arriba 
			if (tile->filename.compare("res/puerta_up_abierta.png") == 0 && player->orientation == game->orientationUp) {
				lastDoorCrossed = 1;
				isOverlap = true;
				habitacionVertical++;
				checkRoomCleared();
				break;
			}
			//por ahora solo hay habitaciones amarillas arriba de las habitaciones
			else if (tile->filename.compare("res/puerta_amarilla_cerrada.png") == 0 && player->orientation == game->orientationUp && nKeys>0) {
				lastDoorCrossed = 1;
				nKeys--;
				actualizarLlaves();
				isOverlap = true;
				habitacionVertical++;
				checkRoomCleared();
				break;
			}
			else if (tile->filename.compare("res/puerta_amarilla_abierta_down.png") == 0 && player->orientation == game->orientationDown ) {
				lastDoorCrossed = 2;
				isOverlap = true;
				habitacionVertical--;
				//al salir de la habitación amarilla se quita el texto
				objConseguido->content = " ";
				checkRoomCleared();
				break;
			}
			//y la puerta es la de arriba y está abierta y el personaje está mirando pa arriba 
			else if (tile->filename.compare("res/puerta_abajo_abierta.png") == 0 && player->orientation == game->orientationDown) {
				lastDoorCrossed = 2;
				isOverlap = true;
				habitacionVertical--;
				checkRoomCleared();
				break;
			}
			else if (tile->filename.compare("res/door_boss_abierta.png") == 0 && player->orientation == game->orientationDown) {
				lastDoorCrossed = 2;
				isOverlap = true;
				habitacionVertical--;
				checkRoomCleared();
				break;
			}
			//y la puerta es la de arriba y está abierta y el personaje está mirando pa arriba 
			else if (tile->filename.compare("res/puerta_derecha_abierta.png") == 0 && player->orientation == game->orientationRight) {
				lastDoorCrossed = 3;
				isOverlap = true;
				habitacionHorizontal++;
				checkRoomCleared();
				break;
			}
			//y la puerta es la de arriba y está abierta y el personaje está mirando pa arriba 
			else if (tile->filename.compare("res/puerta_izquierda_abierta.png") == 0 && player->orientation == game->orientationLeft) {
				lastDoorCrossed = 4;
				isOverlap = true;
				habitacionHorizontal--;
				checkRoomCleared();
				break;
			}
		}
	}
	if (isOverlap) {
		deleteMap();
		isOverlap = false;
		loadMap(nameFile);
		if (lastDoorCrossed == 1) { //up
			player->x = 440;
			player->y = 430;
		} else if (lastDoorCrossed == 2) { //down
			player->x = 440;
			player->y = 80;
		}
		else if (lastDoorCrossed == 3) { //right
			player->x = 135;
			player->y = 255;
		}
		else if (lastDoorCrossed == 4) { //left
			player->x = 750;
			player->y = 255;
		}
		return;
	}

	// Colisiones , Enemy - Projectile

	list<Enemy*> deleteEnemies;
	list<Horf*> deleteEnemiesHorf;
	list <Monoojo*> deleteEnemiesMonoojo;
	list<Projectile*> deleteProjectiles;

	for (auto const& projectile : projectiles) {
		if (projectile->isInRender() == false || (projectile->vx == 0 && projectile->vy == 0) || 
			(projectile->x < 120 || projectile->x > 770 || projectile->y < 60 || projectile->y > 450)) {

			bool pInList = std::find(deleteProjectiles.begin(),
				deleteProjectiles.end(),
				projectile) != deleteProjectiles.end();

			if (!pInList) {
				deleteProjectiles.push_back(projectile);
			}
		}
	}

	for (auto const& enemy : enemies) {
		for (auto const& projectile : projectiles) {
			if (enemy->isOverlap(projectile)) {
				bool pInList = std::find(deleteProjectiles.begin(),
					deleteProjectiles.end(),
					projectile) != deleteProjectiles.end();

				if (!pInList) {
					deleteProjectiles.push_back(projectile);
				}
				audioKillEnemy->play();
				enemy->impacted(player->character->damage);
			}
		}
	}

	for (auto const& enemy : enemies) {
		if (enemy->state == game->stateDead) {
			bool eInList = std::find(deleteEnemies.begin(),
				deleteEnemies.end(),
				enemy) != deleteEnemies.end();

			if (!eInList) {
				deleteEnemies.push_back(enemy);
			}
		}
	}

	for (auto const& enemy : horfEnemies) {
		if (enemy->state == game->stateDead) {
			bool eInList = std::find(deleteEnemiesHorf.begin(),
				deleteEnemiesHorf.end(),
				enemy) != deleteEnemiesHorf.end();

			if (!eInList) {
				deleteEnemiesHorf.push_back(enemy);
			}
		}
	}

	for (auto const& enemy : monoojoEnemies) {
		if (enemy->state == game->stateDead) {
			bool eInList = std::find(deleteEnemiesMonoojo.begin(),
				deleteEnemiesMonoojo.end(),
				enemy) != deleteEnemiesMonoojo.end();

			if (!eInList) {
				deleteEnemiesMonoojo.push_back(enemy);
			}
		}
	}



	for (auto const& delEnemy : deleteEnemiesHorf) {
		horfEnemies.remove(delEnemy);
		space->removeDynamicActor(delEnemy);
	}
	for (auto const& delEnemy : deleteEnemiesMonoojo) {
		monoojoEnemies.remove(delEnemy);
		space->removeDynamicActor(delEnemy);
	}

	for (auto const& delEnemy : deleteEnemies) {
		audioKillEnemy->stop();
		enemies.remove(delEnemy);
		space->removeDynamicActor(delEnemy);
	}
	deleteEnemies.clear();
	deleteEnemiesHorf.clear();
	deleteEnemiesMonoojo.clear();


	for (auto const& delProjectile : deleteProjectiles) {
		projectiles.remove(delProjectile);
		space->removeDynamicActor(delProjectile);
		delete delProjectile;
	}
	deleteProjectiles.clear();

	// Colisiones , Player - Projectile
	list<ProjectileEnemy*> deleteEnemyProjectiles;

	for (auto const& projectile : projectilesEnemy) {
		for (auto const& tile : rocas) {
			if (tile->isOverlap(projectile)) {
				bool pInList = std::find(deleteEnemyProjectiles.begin(),
					deleteEnemyProjectiles.end(),
					projectile) != deleteEnemyProjectiles.end();

				if (!pInList) {
					deleteEnemyProjectiles.push_back(projectile);
				}
				space->removeDynamicActor(projectile);
			}
		}
	}

	for (auto const& projectile : projectilesEnemy) {
		if (projectile->isInRender() == false || ((projectile->x < 125 || projectile->x > 760 || projectile->y < 70 || projectile->y > 450))) {

			bool pInList = std::find(deleteEnemyProjectiles.begin(),
				deleteEnemyProjectiles.end(),
				projectile) != deleteEnemyProjectiles.end();

			if (!pInList) {
				deleteEnemyProjectiles.push_back(projectile);
			}
		}
	}
	for (auto const& projectile : projectilesEnemy) {
		if (player->isOverlap(projectile)) {
			bool pInList = std::find(deleteEnemyProjectiles.begin(),
				deleteEnemyProjectiles.end(),
				projectile) != deleteEnemyProjectiles.end();

			if (!pInList) {
				deleteEnemyProjectiles.push_back(projectile);
			}

			if (!playerCharacter->invencibilidad) {
				audioGolpeado->play();
				player->loseLife();
				actualizarVidas();
				if (player->character->lifes <= 0) {
					audioMuerte->play();
					init();
					return;
				}
			}
			
			break;
		}
	}

	for (auto const& delProjectile : deleteEnemyProjectiles) {
		projectilesEnemy.remove(delProjectile);
		space->removeDynamicActor(delProjectile);
		delete delProjectile;
	}
	deleteEnemyProjectiles.clear();

	
	// Actualizar la invencibilidad si está activa
	if (playerCharacter->invencibilidad) {
		Uint32 tiempoActual = SDL_GetTicks();
	
		if (tiempoActual - tiempoInicioInvencibilidad >= duracionInvencibilidad) {
			// Desactivar la invencibilidad después de la duración deseada
			playerCharacter->invencibilidad = false;
			textActivo->content = " ";

		}
	}


	time(&endBomb);
	if ((endBomb - startBomb) > 2 && activeBomb != NULL) {
		time(&timeExplosion);
		activeBomb = new Tile("res/recolectables/explosion.png", activeBomb->x, activeBomb->y, 38, 38, game);

		if (reproduccionesBomba == 1) {
			audioBomb->play();
			reproduccionesBomba = 0;
		}
		
		for (auto const& tile : rocas) {
			if (calculateDistance(activeBomb, tile) < 100) {
				deleteRocas.push_back(tile);
				space->removeStaticActor(tile);
			}
		}
		for (auto const& e : enemies) {
			if (calculateDistance(activeBomb, e) < 100) {
				e->state = game->stateDying;
			}
		}
		for (auto const& b : deleteRocas) {
			rocas.remove(b);
			delete b;
		}
		deleteRocas.clear();
		if (personaje != 4) {
			if (calculateDistance(activeBomb, player) < 100) {
				if (!playerCharacter->invencibilidad) {
					audioGolpeado->play();
					player->loseLife();
					actualizarVidas();
					if (player->character->lifes <= 0) {
						audioMuerte->play();
						init();
						return;
					}
				}
			}
		}
		
		if ((endBomb - startBomb) > 3) {
			activeBomb = NULL;
			reproduccionesBomba = 1;
		}
	}

	if ((azazel == NULL && jefe2 == NULL && bigHorn == NULL) && trapdoor != NULL && player->isOverlap(trapdoor)) {
		audioChangeFloor->play();
		floor++;
		trapdoor = NULL;
		deleteMap();
		if (floor == 1) {
			nameFile = "res/fondos/1_0_0.txt";
		}
		else if (floor == 2) {
			nameFile = "res/fondos/2_0_0.txt";
		}
		else if (floor == 3) {
			nameFile = "res/fondos/3_0_0.txt";
		}
		habitacionVertical = 0;
		habitacionHorizontal = 0;
		loadMap(nameFile);
		return;
	}
	if (bigHorn == NULL && trofeo != NULL && nameFile == "res/fondos/3_0_4.txt" && !won) {
		won = true;
		tiles.push_back(trofeo);
		space->addDynamicActor(trofeo);
	}
	if (bigHorn == NULL && trofeo != NULL && player->isOverlap(trofeo)) {
		//esto hacerlo cuando se mate al ultimo boss y quitar la trampilla porque no es necesariaaaaa
		textActivo->content = "Eres un crack!!!";
		return;
	}
	textSpeed->content = to_string((int)player->character->playerSpeed);
	textDamage->content = to_string(player->character->damage);
	textTears->content = to_string(player->character->shootCadence);
	
	cout << "update GameLayer " << endl;
}
/*
* void GameLayer::calculateScroll() {
	// limite izquierda
	if (player->x > WIDTH * 0.4) {
		if (player->x - scrollX < WIDTH * 0.4) {
			scrollX = player->x - WIDTH * 0.4;
		}
	}

	// limite derecha
	if (player->x < mapWidth - WIDTH * 0.4) {
		if (player->x - scrollX > WIDTH * 0.6) {
			scrollX = player->x - WIDTH * 0.6;
		}
	}
	
	// limite izquierda
	if (player->y > HEIGHT * 0.4) {
		if (player->y - scrollY < HEIGHT * 0.4) {
			scrollY = player->y - HEIGHT * 0.4;
		}
	}

	// limite derecha
	if (player->y < mapHeight - HEIGHT * 0.4) {
		if (player->y - scrollY > HEIGHT * 0.6) {
			scrollY = player->y - HEIGHT * 0.6;
		}
	}
}
*/


void GameLayer::draw() {
	//calculateScroll();
	background->draw();
	textBombs->draw();
	textKeys->draw();
	textActivo->draw();
	objConseguido->draw();
	
	for (auto const& tile : tiles) {
		tile->draw();
	}
	for (auto const& tile : rocas) {
		tile->draw();
	}
	for (auto const& tile : doors) {
		tile->draw();
	}
	for (auto const& tile : bombs) {
		tile->draw();
	}

	for (auto const& tile : corazones) {
		tile->draw();
	}
	for (auto const& tile : keys) {
		tile->draw();
	}
	for (auto const& tile : pills) {
		tile->draw();
	}
	for (auto const& tile : fuegos) {
		tile->state = game->stateMoving;
		tile->draw();
	}
	for (auto const& tile : objetos) {
		tile->draw();
	}
	for (auto const& projectile : projectiles) {
		projectile->draw();
	}
	for (auto const& projectile : projectilesEnemy) {
		projectile->draw();
	}

	player->draw();
	for (auto const& enemy : enemies) {
		enemy->draw();
	}
	for (auto const& heart : hearts) {
		heart->draw();
	}

	bombsActor->draw();
	keysActor->draw();
	if (pillsActor != NULL)
		pillsActor->draw();
	if (activeBomb != NULL)
		activeBomb->draw();
	
	speedActor->draw();
	textSpeed->draw();
	damageActor->draw();
	textDamage->draw();
	tearsActor->draw();
	textTears->draw();
	//creo que no hace falta porque ya se pinta en enemies
	/*for (auto const& enemy : horfEnemies) {
		enemy->draw(scrollX, scrollY);
	}*/
	SDL_RenderPresent(game->renderer); // Renderiza
}	bool aux = false;;


void GameLayer::keysToControls(SDL_Event event) {
	float s  = playerCharacter->playerSpeed;
	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;

		// Pulsada
		switch (code) {
		case SDLK_ESCAPE:
			game->loopActive = false;
			break;
		case SDLK_SPACE:
			if (personaje == 1 && (player->vx != 0 ||  player->vy!=0)) {
				playerCharacter->playerSpeed = s + 5;
				textActivo->content = "Tienes supervelocidad!!";
				aux = true;

			}
			if (personaje == 2) {
				playerCharacter->rompeRocas = true;
				textActivo->content = "Puedes romper rocas!!";
			}
			if (personaje == 3) {
				if (vecesInvencible == 0) {
					playerCharacter->invencibilidad = true;
					tiempoInicioInvencibilidad = SDL_GetTicks();
					textActivo->content = "Eres invencible durante 3 segundos!!";
					vecesInvencible++;

				}
			}
			if (personaje == 4) {
				textActivo->content = "Explotando!!";
				list<Tile*> deleteRocas;
				player->state = game->stateExplotando;
				for (auto const& tile : rocas) {
					if (calculateDistance(player, tile) < 100) {
						deleteRocas.push_back(tile);
						space->removeStaticActor(tile);
					}
				}
				for (auto const& e : enemies) {
					if (calculateDistance(player, e) < 100) {
						e->state = game->stateDying;
					}
				}
				for (auto const& b : deleteRocas) {
					rocas.remove(b);
					delete b;
				}
				deleteRocas.clear();
			
				if ((endBomb - startBomb) > 3) {
					activeBomb = NULL;
				}
			}
			break;
		case SDLK_1:
			game->scale();
			break;
		case SDLK_q:
			if (nPills == 1) {
				nPills = 0;
				actualizarPills();
				cogerPill();
			}
			break;
		case SDLK_d: // derecha
			player->orientation = game->orientationRight;
			controlMoveX = 1;
			break;
		case SDLK_a: // izquierda
			player->orientation = game->orientationLeft;
			controlMoveX = -1;
			break;
		case SDLK_w: // arriba
			player->orientation = game->orientationUp;
			controlMoveY = -1;
			break;
		case SDLK_s: // abajo
			player->orientation = game->orientationDown;
			controlMoveY = 1;
			break;
		case SDLK_DOWN: // dispara
			player->orientation = game->orientationDown;
			controlShoot = true;
			break;
		case SDLK_UP: // dispara
			player->orientation = game->orientationUp;
			controlShoot = true;
			break;
		case SDLK_LEFT: // dispara
			player->orientation = game->orientationLeft;
			controlShoot = true;
			break;
		case SDLK_RIGHT: // dispara
			player->orientation = game->orientationRight;
			controlShoot = true;
			break;
		case SDLK_e: // bomba
			dropBomb();
			break;
		}



	}
	if (event.type == SDL_KEYUP) {
		int code = event.key.keysym.sym;
		// Levantada
		switch (code) {
		case SDLK_d: // derecha
			if (controlMoveX == 1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_q:
			objConseguido->content = " ";
			break;
		case SDLK_a: // izquierda
			if (controlMoveX == -1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_SPACE:
			if (personaje == 1 && aux ==true) {
				playerCharacter->playerSpeed = s - 5;
				textActivo->content = " ";
				aux = false;
			}
			if (personaje == 2) {
				playerCharacter->rompeRocas = false;
				textActivo->content = " ";
			}
			/*if (personaje == 3) {
				playerCharacter->invencibilidad = false;
			}*/
			if (personaje == 4) {
				textActivo->content = " ";
				player->state = game->stateMoving;
			}
			break;
		case SDLK_w: // arriba
			if (controlMoveY == -1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_s: // abajo
			if (controlMoveY == 1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_DOWN: // dispara
			controlShoot = false;
			break;
		case SDLK_UP: // dispara
			controlShoot = false;
			break;
		case SDLK_LEFT: // dispara
			controlShoot = false;
			break;
		case SDLK_RIGHT: // dispara
			controlShoot = false;
			break;
		}

	}

}

void GameLayer::loadMap(string name) {
	char character;
	string line;
	ifstream streamFile(name.c_str());
	if (!streamFile.is_open()) {
		cout << "Falla abrir el fichero de mapa" << endl;
		return;
	}
	else {
		// Por línea

		for (int i = 0; getline(streamFile, line); i++) {
			istringstream streamLine(line);
			mapWidth = line.length() * 40; // Ancho del mapa en pixels
			// Por carácter (en cada línea)
			for (int j = 0; !streamLine.eof(); j++) {
				streamLine >> character; // Leer character 
				cout << character;
				float x = 40 / 2 + j * 40; // x central
				float y = 32 + i * 32; // y suelo
				loadMapObject(character, x, y);
			}

			//cout << character << endl;
		}
	}
	streamFile.close();
}


void GameLayer::loadMapObject(char character, float x, float y)
{
	vecesInvencible = 0;
	switch (character) {
	case '1': {
		player = new Player(x, y, game, playerCharacter);
		// modificación para empezar a contar desde el suelo.
		player->y = player->y - player->height / 2;
		space->addDynamicActor(player);
		break;
	}
	case '#': {
		Tile* tile = new Tile("res/fondos/rock.png", x, y, 36, 34, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		//tiles.push_back(tile);
		rocas.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	case '*': {
		Enemy* tile = new Enemy("res/fondos/fuego.png", x, y, game);
		tile->state = game->stateMoving;
		tile->aDying = new Animation("res/fondos/fuego_movimiento.png", 51, 51,
			105, 51, 6, 2, false, game);
		tile->aMoving = new Animation("res/fondos/fuego_movimiento.png", 51, 51,
			105, 51, 6, 2, true, game);
		tile->animation = tile->aMoving;
		tile->vx = 0;
		tile->vy = 0;
		tile->enemySpeed = 0;
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		fuegos.push_back(tile);
		space->addDynamicActor(tile);
		break;
	}
	case '/': {
		Tile* tile = new Tile("res/pills/pill.png", x, y, 25, 25, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		pills.push_back(tile);
		//tiles.push_back(tile);
		space->addDynamicActor(tile);
		break;
	}
	case 'C': {
		Tile* tile = new Tile("res/recolectables/corazon.png", x, y, 44, 36, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		corazones.push_back(tile);
		space->addDynamicActor(tile);
		break;
	}
	case 'B': {
		Tile* tile = new Tile("res/recolectables/bomb.png", x, y, 28, 38, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		bombs.push_back(tile);
		//tiles.push_back(tile);
		space->addDynamicActor(tile);
		break;
	}
	case 'K': {
		Tile* tile = new Tile("res/recolectables/key.png", x, y, 28, 31, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		keys.push_back(tile);
		space->addDynamicActor(tile);
		break;
	}
	case 'F': {
		Fatty* enemy = new Fatty(x, y, game);
		// modificación para empezar a contar desde el suelo.
		enemy->y = enemy->y - enemy->height / 2;
		enemies.push_back(enemy);
		space->addDynamicActor(enemy);
		break;
	}
	case 'M': {
		Monoojo* enemy = new Monoojo(x, y, game);
		// modificación para empezar a contar desde el suelo.
		enemy->y = enemy->y - enemy->height / 2;
		enemies.push_back(enemy);
		monoojoEnemies.push_back(enemy);
		space->addDynamicActor(enemy);
		break;
	}
	case 'H': {
		Horf* enemy = new Horf(x, y, game);
		// modificación para empezar a contar desde el suelo.
		enemy->y = enemy->y - enemy->height / 2;
		enemies.push_back(enemy);
		horfEnemies.push_back(enemy);
		space->addDynamicActor(enemy);
		break;
	}
	case 'P': {
		Tile* door = new Tile("res/puerta_up_cerrada.png", x - 15, y + 8, 49, 33, game);
		// modificación para empezar a contar desde el suelo.
		door->y = door->y - door->height / 2;
		//tiles.push_back(door);
		doors.push_back(door);
		space->addDynamicActor(door);
		break;
	}
	case '+': {
		Tile* door = new Tile("res/door_boss_cerrada.png", x - 15, y + 20, 64, 40, game);
		// modificación para empezar a contar desde el suelo.
		door->y = door->y - door->height / 2;
		//tiles.push_back(door);
		doors.push_back(door);
		space->addDynamicActor(door);
		break;
	}
	case '-': {
		Tile* door = new Tile("res/door_boss_cerrada2.png", x - 15, y + 8, 64, 40, game);
		// modificación para empezar a contar desde el suelo.
		door->y = door->y - door->height / 2;
		//tiles.push_back(door);
		doors.push_back(door);
		space->addDynamicActor(door);
		break;
	}
	case 'A': {
		Tile* door = new Tile("res/puerta_amarilla_cerrada.png", x - 15, y + 12, 49, 38, game);
		// modificación para empezar a contar desde el suelo.
		door->y = door->y - door->height / 2;
		//tiles.push_back(door);
		doors.push_back(door);
		space->addDynamicActor(door);
		break;
	}
	case 'X': {
		Tile* door = new Tile("res/puerta_amarilla_abierta_down.png", x - 15, y + 8, 49, 38, game);
		// modificación para empezar a contar desde el suelo.
		door->y = door->y - door->height / 2;
		//tiles.push_back(door);
		doors.push_back(door);
		space->addDynamicActor(door);
		break;
	}
	case 'L': {
		Tile* door = new Tile("res/puerta_izquierda_cerrada.png", x, y, 33, 49, game);
		// modificación para empezar a contar desde el suelo.
		door->y = door->y - door->height / 2;
		//tiles.push_back(door);
		doors.push_back(door);
		space->addDynamicActor(door);
		break;
	}
	case 'R': {
		Tile* door = new Tile("res/puerta_derecha_cerrada.png", x + 18, y, 33, 49, game);
		// modificación para empezar a contar desde el suelo.
		door->y = door->y - door->height / 2;
		//tiles.push_back(door);
		doors.push_back(door);
		space->addDynamicActor(door);
		break;
	}
	case 'D': {
		Tile* door = new Tile("res/puerta_abajo_cerrada.png", x - 12, y + 12, 49, 33, game);
		// modificación para empezar a contar desde el suelo.
		door->y = door->y - door->height / 2;
		//tiles.push_back(door);
		doors.push_back(door);
		space->addDynamicActor(door);
		break;
	}

	case 'T': {
		trapdoor = new Tile("res/trampilla.png", x - 12, y + 12, 64, 64, game);
		// modificación para empezar a contar desde el suelo.
		trapdoor->y = trapdoor->y - trapdoor->height / 2;
		tiles.push_back(trapdoor);
		space->addDynamicActor(trapdoor);
		break;
	}
	case '2': {
		Tile* door = objetoRandom(x, y);
		// modificación para empezar a contar desde el suelo.
		door->y = door->y - door->height / 2;
		objetos.push_back(door);
		space->addDynamicActor(door);
		break;
	}
	case 'Z': {
		azazel = new Azazel(x, y, game);
		// modificación para empezar a contar desde el suelo.
		azazel->y = azazel->y - azazel->height / 2;
		enemies.push_back(azazel);
		space->addDynamicActor(azazel);
		break;
	}
	case 'J': {
		jefe2 = new Jefe2(x, y, game);
		// modificación para empezar a contar desde el suelo.
		jefe2->y = jefe2->y - jefe2->height / 2;
		enemies.push_back(jefe2);
		space->addDynamicActor(jefe2);
		break;
	}
	case 'G': {
		bigHorn = new BigHorn(x, y, game);
		// modificación para empezar a contar desde el suelo.
		bigHorn->y = bigHorn->y - bigHorn->height / 2;
		enemies.push_back(bigHorn);
		space->addDynamicActor(bigHorn);
		break;
	}
	case 'U': {
		trofeo = new Tile("res/trophy.png", x - 12, y + 12, 64, 64, game);
		// modificación para empezar a contar desde el suelo.
		trofeo->y = trofeo->y - trofeo->height / 2;
		/*tiles.push_back(trofeo);
		space->addDynamicActor(trofeo);*/
		break;
	}
	}
}

void GameLayer::cogerPill()
{
	int max = 12;
	int min = 1;
	int d = rand() % (max - min + 1) + min;
	if (d == 1) {
		audioPill->play();
		objConseguido->content = "+ velocidad";
		player->character->playerSpeed += 1.0;
	} else if (d == 2) {
		audioPill->play();
		objConseguido->content = "+ vida";
		if (hearts.size() < 5) {
			player->character->lifes++;
			actualizarVidas();
		}
	}
	else if (d == 3) {
		audioPill->play();
		objConseguido->content = "+ daño";
		player->character->damage++;
	}
	else if (d == 4) {
		audioPill->play();
		objConseguido->content = "+ cadencia";
		if (player->character->shootCadence > 5)
			player->character->shootCadence -= 3;
	}
	else if (d == 5) {
		audioPill->play();
		objConseguido->content = "+ llaves";
		nKeys++;
		actualizarLlaves();
	} else if (d == 6) {
		audioPill->play();
		objConseguido->content = "+ bombas";
		nBombs++;
		actualizarBombas();
	} else if (d == 7) {
		audioPill->play();
		objConseguido->content = "- velocidad";
		if (player->character->playerSpeed > 2)
			player->character->playerSpeed -= 1.0;
	}
	else if (d == 8) {
		audioPill->play();
		objConseguido->content = "- vida";
		audioGolpeado->play();
		player->loseLife();
		actualizarVidas();
		if (player->character->lifes <= 0) {
			audioMuerte->play();
			init();
			return;
		}
	}
	else if (d == 9) {
		audioPill->play();
		objConseguido->content = "- daño";
		if (player->character->damage > 2)
			player->character->damage--;
	}
	else if (d == 10) {
		audioPill->play();
		objConseguido->content = "- cadencia";
		player->character->shootCadence += 3;
	}
	else if (d == 11) {
		audioPill->play();
		objConseguido->content = "- llaves";
		if (nKeys > 0)
			nKeys--;
		actualizarLlaves();
	}
	else if (d == 12) {
		audioPill->play();
		objConseguido->content = "- bombas";
		if (nBombs > 0)
			nBombs--;
		actualizarBombas();
	}
}

void GameLayer::actualizarVidas() {
	hearts.clear();
	for (int i = 0; i < player->character->lifes; i++) {
		hearts.push_back(new Actor("res/corazones/corazon.png", WIDTH * 0.1 + i * 45, HEIGHT * 0.05, 44, 36, game));
	}
}

void GameLayer::actualizarBombas() {
	bombsActor = new Actor("res/recolectables/bomb_icono.png", WIDTH * 0.02 , HEIGHT * 0.05, 16, 22, game);
	textBombs->content = to_string(nBombs);
}

void GameLayer::actualizarLlaves() {
	keysActor = new Actor("res/recolectables/key_icono.png", WIDTH * 0.02, HEIGHT * 0.1, 16, 18, game);
	textKeys->content = to_string(nKeys);
}

void GameLayer::actualizarPills() {
	if (nPills == 1)
		pillsActor = new Actor("res/pills/pillActor.png", WIDTH * 0.95, HEIGHT * 0.95, 50, 50, game);
	else
		pillsActor = NULL;
}

void GameLayer::objetoConseguido(Tile* t) {
	audioObject->play();
	if (t->filename.compare("res/objetos/crickets_head.png") == 0) {
		objConseguido->content = "+ daño";
		player->character->damage += 3;
	} else if (t->filename.compare("res/objetos/crickets_body.png") == 0) {
		objConseguido->content = "+ velocidad";
		player->character->playerSpeed += 3.0;
	} else if (t->filename.compare("res/objetos/cuchara.png") == 0) {
		objConseguido->content = "+ cadencia";
		player->character->shootCadence -= 10;
	}
}

void GameLayer::deleteMap() {
	for (auto const& tile : tiles) {
		space->removeStaticActor(tile);
	}
	for (auto const& rocas : rocas) {
		space->removeStaticActor(rocas);
	}
	for (auto const& tile : doors) {
		space->removeDynamicActor(tile);
	}
	tiles.clear();
	projectiles.clear();
	projectilesEnemy.clear();
	doors.clear();
	fuegos.clear();
	rocas.clear();
	bombs.clear();
	keys.clear();
	pills.clear();
	corazones.clear();
	objetos.clear();
	enemies.clear();
}

void GameLayer::chooseCharater() {
	if (personaje == 2) {
		playerCharacter = new Cain(game);
	}
	else if (personaje == 3) {
		playerCharacter = new Eve(game);
	}
	else if (personaje == 1) {
		playerCharacter = new Eden(game);
	}
	else if (personaje == 4) {
		playerCharacter = new Judas(game);
	}
	else {
		playerCharacter = new Isaac(game);
	}
}

void GameLayer::saveRoom(string nameFile) {
	roomsCleared.push_back(nameFile);
}

void GameLayer::checkRoomCleared() {
	if (floor == 1) {
		nameFile = "res/fondos/1_" + std::to_string(habitacionVertical) + "_" + std::to_string(habitacionHorizontal) + ".txt";
		if (std::find(roomsCleared.begin(), roomsCleared.end(), nameFile) != roomsCleared.end()) {
			nameFile = "res/fondos/1_" + std::to_string(habitacionVertical) + "_" + std::to_string(habitacionHorizontal) + " - X" + ".txt";
		}
	}
	else if (floor == 2) {
		nameFile = "res/fondos/2_" + std::to_string(habitacionVertical) + "_" + std::to_string(habitacionHorizontal) + ".txt";
		if (std::find(roomsCleared.begin(), roomsCleared.end(), nameFile) != roomsCleared.end()) {
			nameFile = "res/fondos/2_" + std::to_string(habitacionVertical) + "_" + std::to_string(habitacionHorizontal) + " - X" + ".txt";
		}
	}
	else if (floor == 3) {
		nameFile = "res/fondos/3_" + std::to_string(habitacionVertical) + "_" + std::to_string(habitacionHorizontal) + ".txt";
		if (std::find(roomsCleared.begin(), roomsCleared.end(), nameFile) != roomsCleared.end()) {
			nameFile = "res/fondos/3_" + std::to_string(habitacionVertical) + "_" + std::to_string(habitacionHorizontal) + " - X" + ".txt";
		}
	}
}

void GameLayer::dropBomb() {
	if (nBombs <= 0 || activeBomb != NULL) return;
	activeBomb = new Tile("res/recolectables/bomb.png", player->x, player->y, 28, 38, game);
	time(&startBomb);
	nBombs--;
	actualizarBombas();
}

double GameLayer::calculateDistance(Actor* actor1, Actor* actor2) {
	double deltaX = actor1->x - actor2->x;
	double deltaY = actor1->y - actor2->y;

	return std::sqrt(deltaX * deltaX + deltaY * deltaY);
}

Tile* GameLayer::objetoRandom(int x, int y) {
	Tile* tile = NULL;
	int rd = 0;
	std::size_t listSize = objetosTotales.size()-1;
	int min = 0;
	while (true) {
		rd = rand() % (listSize - min + 1) + min;
		int valorEnPosicion = objetosTotales.front();
		auto it = std::next(objetosTotales.begin(), rd);
		valorEnPosicion = *it;
		if (valorEnPosicion == 1) {
			tile = new Tile("res/objetos/crickets_head.png", x, y, 34, 30, game);
			objetosTotales.remove(valorEnPosicion);
			break;
		}
		else if (valorEnPosicion == 2) {
			tile = new Tile("res/objetos/crickets_body.png", x, y, 35, 33, game);
			objetosTotales.remove(valorEnPosicion);
			break;
		}
		else if (valorEnPosicion == 3) {
			tile = new Tile("res/objetos/cuchara.png", x, y, 32, 29, game);
			objetosTotales.remove(valorEnPosicion);
			break;
		}
	}
	return tile;
}
