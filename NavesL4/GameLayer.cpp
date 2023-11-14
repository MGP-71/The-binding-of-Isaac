#include "GameLayer.h"
#include "Horf.h"
#include "Fatty.h"
#include "Monoojo.h"

GameLayer::GameLayer(Game* game)
	: Layer(game) {
	//llama al constructor del padre : Layer(renderer)
	init();
}

void GameLayer::init() {
	space = new Space(0);
	scrollX = 0;
	scrollY = 0;
	tiles.clear();
	fuegos.clear();
	doors.clear();
	hearts.clear();
	bombs.clear();
	keys.clear();

	audioBackground = new Audio("res/musica_ambiente.mp3", true);
	//audioBackground->play();


	background = new Background("res/fondos/catacombs.png", WIDTH * 0.5, HEIGHT * 0.5, -1, game);
	textBombs = new Text("hola", WIDTH * 0.075, HEIGHT * 0.4, game);
	textBombs->content = to_string(0);
	textKeys = new Text("hola", WIDTH * 0.075, HEIGHT * 0.45, game);
	textKeys->content = to_string(0);

	enemies.clear(); // Vaciar por si reiniciamos el juego
	projectiles.clear(); // Vaciar por si reiniciamos el juego
	horfEnemies.clear(); // Vaciar por si reiniciamos el juego
	projectilesEnemy.clear(); // Vaciar por si reiniciamos el juego
	monoojoEnemies.clear();

	loadMap("res/fondos/catacombs.txt");
	actualizarVidas();
	actualizarBombas();
	actualizarLlaves();
	actualizarPills();
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
			space->addDynamicActor(newProjectile);
			projectiles.push_back(newProjectile);
		}

	}

	// Eje X
	if (controlMoveX > 0) {
		if (player->x < 840)
			player->moveX(1);
	}
	else if (controlMoveX < 0) {
		if (player->x > 20)
			player->moveX(-1);
	}
	else {
		player->moveX(0);
	}

	// Eje Y
	if (controlMoveY > 0) {
		if (player->y < 690)
			player->moveY(1);

	}
	else if (controlMoveY < 0) {
		if (player->y > 20)
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

	//Abrir puertas
	bool allDead = true;
	for (auto const& enemy : enemies) {
		if (enemy->state != game->stateDead) {
			allDead = false;
		}
	}
	if (allDead) {
		puertasAbiertas = true;
		for (auto const& door : space->staticActors) {
			if (door->filename.compare("res/puerta_up_cerrada.png") == 0) {
				door->filename = "res/puerta_up_abierta.png";
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

		//para que se pueda pasar por las puertas al estar abiertas
		for (auto const& door : doors) {
			space->removeStaticActor(door);
			space->addDynamicActor(door);
		}
	}
	// Colisiones
	for (auto const& enemy : enemies) {
		if (player->isOverlap(enemy)) {
			player->loseLife();
			actualizarVidas();
			if (player->lifes <= 0) {
				init();
				return;
			}
			return; // Cortar el for
		}
	}

	//Colisiones jugador-fuego
	for (auto const& fuego : fuegos) {
		if (player->isOverlap(fuego)) {
			player->loseLife();
			actualizarVidas();
			if (player->lifes <= 0) {
				actualizarVidas();
				init();
				return;
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
	//Colisiones jugador-bomba
	for (auto const& pill : pills) {
		if (player->isOverlap(pill)) {
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
	

	//Colisiones Puerta-Personaje
	for (auto const& tile : doors) {
		//si el personaje overlapea la puerta
		if (tile->isOverlap(player)) {
			//y la puerta es la de arriba y está abierta y el personaje está mirando pa arriba 
			if (tile->filename.compare("res/puerta_up_abierta.png") == 0 && player->orientation == game->orientationUp) {
				cout << "pasa" << endl;
			
			}
			//y la puerta es la de arriba y está abierta y el personaje está mirando pa arriba 
			if (tile->filename.compare("res/puerta_abajo_abierta.png") == 0 && player->orientation == game->orientationDown) {
				cout << "pasa" << endl;

			}

			//y la puerta es la de arriba y está abierta y el personaje está mirando pa arriba 
			if (tile->filename.compare("res/puerta_derecha_abierta.png") == 0 && player->orientation == game->orientationRight) {
				cout << "pasa" << endl;

			}
			//y la puerta es la de arriba y está abierta y el personaje está mirando pa arriba 
			if (tile->filename.compare("res/puerta_izquierda_abierta.png") == 0 && player->orientation == game->orientationLeft) {
				cout << "pasa" << endl;

			}
		}
	}

	// Colisiones , Enemy - Projectile

	list<Enemy*> deleteEnemies;
	list<Horf*> deleteEnemiesHorf;
	list <Monoojo*> deleteEnemiesMonoojo;
	list<Projectile*> deleteProjectiles;

	for (auto const& projectile : projectiles) {
		if (projectile->isInRender(scrollX, scrollY) == false || (projectile->vx == 0 && projectile->vy == 0)) {

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

				enemy->impacted();

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
		for (auto const& tile : tiles) {
			if (tile->isOverlap(projectile)) {
				projectile->vx = 0;
				projectile->vy = 0;
			}
		}
	}

	for (auto const& projectile : projectilesEnemy) {
		if (projectile->isInRender(scrollX, scrollY) == false || (projectile->vx == 0 && projectile->vy == 0)) {

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

			player->loseLife();
			actualizarVidas();
			if (player->lifes <= 0) {
				init();
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

	
		
	cout << "update GameLayer" << endl;
}

void GameLayer::calculateScroll() {
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

void GameLayer::draw() {
	calculateScroll();
	background->draw();
	textBombs->draw();
	textKeys->draw();
	for (auto const& tile : tiles) {
		tile->draw(scrollX, scrollY);
	}
	for (auto const& tile : bombs) {
		tile->draw(scrollX, scrollY);
	}
	for (auto const& tile : pills) {
		tile->draw(scrollX, scrollY);
	}
	for (auto const& tile : keys) {
		tile->draw(scrollX, scrollY);
	}


	for (auto const& projectile : projectiles) {
		projectile->draw(scrollX, scrollY);
	}
	for (auto const& projectile : projectilesEnemy) {
		projectile->draw(scrollX, scrollY);
	}

	player->draw(scrollX, scrollY);
	for (auto const& enemy : enemies) {
		enemy->draw(scrollX, scrollY);
	}
	for (auto const& heart : hearts) {
		heart->draw();
	}
		bombsActor->draw();
		keysActor->draw();
		pillsActor->draw();
	
	//creo que no hace falta porque ya se pinta en enemies
	/*for (auto const& enemy : horfEnemies) {
		enemy->draw(scrollX, scrollY);
	}*/
	SDL_RenderPresent(game->renderer); // Renderiza
}

void GameLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;
		// Pulsada
		switch (code) {
		case SDLK_ESCAPE:
			game->loopActive = false;
			break;
		case SDLK_1:
			game->scale();
			break;
		case SDLK_d: // derecha
			controlMoveX = 1;
			break;
		case SDLK_a: // izquierda
			controlMoveX = -1;
			break;
		case SDLK_w: // arriba
			controlMoveY = -1;
			break;
		case SDLK_s: // abajo
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
		case SDLK_a: // izquierda
			if (controlMoveX == -1) {
				controlMoveX = 0;
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
			mapHeight = i * 33;
		}
	}
	streamFile.close();
}


void GameLayer::loadMapObject(char character, float x, float y)
{
	switch (character) {
	case '1': {
		player = new Player(x, y, game);
		// modificación para empezar a contar desde el suelo.
		player->y = player->y - player->height / 2;
		space->addDynamicActor(player);
		break;
	}
	case '#': {
		Tile* tile = new Tile("res/fondos/piedra.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	case '*': {
		Tile* tile = new Tile("res/fondos/fuego.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		fuegos.push_back(tile);
		space->addDynamicActor(tile);
		break;
	}
	case '/': {
		Tile* tile = new Tile("res/pills/pill.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		pills.push_back(tile);
		//tiles.push_back(tile);
		space->addDynamicActor(tile);
		break;
	}
	case 'B': {
		Tile* tile = new Tile("res/recolectables/bomb.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		bombs.push_back(tile);
		//tiles.push_back(tile);
		space->addDynamicActor(tile);
		break;
	}
	case 'K': {
		Tile* tile = new Tile("res/recolectables/key.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		keys.push_back(tile);
		space->addDynamicActor(tile);
		break;
	}
	case 'F': {
		Fatty* enemy = new Fatty( x, y, game);
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
		Tile* door = new Tile("res/puerta_up_cerrada.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		door->y = door->y - door->height / 2;
		tiles.push_back(door);
		doors.push_back(door);
		space->addStaticActor(door);
		break;
	}
	case 'L': {
		Tile* door = new Tile("res/puerta_izquierda_cerrada.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		door->y = door->y - door->height / 2;
		tiles.push_back(door);
		doors.push_back(door);
		space->addStaticActor(door);
		break;
	}
	case 'R': {
		Tile* door = new Tile("res/puerta_derecha_cerrada.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		door->y = door->y - door->height / 2;
		tiles.push_back(door);
		doors.push_back(door);
		space->addStaticActor(door);
		break;
	}
	case 'D': {
		Tile* door = new Tile("res/puerta_abajo_cerrada.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		door->y = door->y - door->height / 2;
		tiles.push_back(door);
		doors.push_back(door);
		space->addStaticActor(door);
		break;
	}

	}
}

void GameLayer::actualizarVidas() {
	hearts.clear();
	for (int i = 0; i < player->lifes; i++) {
		hearts.push_back(new Actor("res/corazones/corazon.png", WIDTH * 0.1 + i * 45, HEIGHT * 0.05, 44, 36, game));
	}
}

void GameLayer::actualizarBombas() {
	bombsActor = new Actor("res/recolectables/bomb_icono.png", WIDTH * 0.05 , HEIGHT * 0.4, 16, 22, game);
	textBombs->content = to_string(nBombs);

	
}

void GameLayer::actualizarLlaves() {
	keysActor = new Actor("res/recolectables/key_icono.png", WIDTH * 0.05, HEIGHT * 0.45, 16, 18, game);
	textKeys->content = to_string(nKeys);
}

void GameLayer::actualizarPills() {
	if(nPills == 1)
		pillsActor = new Actor("res/pills/pill.png", WIDTH * 0.95, HEIGHT * 0.95, 16, 18, game);
	else
		pillsActor = new Actor("res/pills/pill.png", WIDTH * 0.95, HEIGHT * 0.95, 0, 0, game); //esto es una chapuza que funciona
}