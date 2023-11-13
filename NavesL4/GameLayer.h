#pragma once

#include "Layer.h"
#include "Player.h"
#include "Background.h"

#include "Enemy.h"
#include "Projectile.h"
#include "Text.h"
#include "Tile.h"
#include "Horf.h"
#include "Monoojo.h"
#include "ProjectileEnemy.h"

#include "Audio.h"
#include "Pad.h"
#include "Space.h"

#include <fstream> // Leer ficheros
#include <sstream> // Leer líneas / String

#include <list>

class GameLayer : public Layer
{
public:
	GameLayer(Game* game);
	void init() override;
	void processControls() override;
	void update() override;
	void draw() override;
	void keysToControls(SDL_Event event);

	void calculateScroll();
	void actualizarVidas();
	void actualizarBombas();


	void loadMap(string name);
	void loadMapObject(char character, float x, float y);

	int mapWidth;
	int mapHeight;
	list<Tile*> tiles;
	list<Tile*> doors;
	list<Tile*> fuegos;


	Audio* audioBackground;
	int newEnemyTime = 0;
	Player* player;
	Background* background;
	list<Enemy*> enemies;
	list<Horf*> horfEnemies;
	list<Monoojo*> monoojoEnemies;
	list<Projectile*> projectiles;
	list<ProjectileEnemy*> projectilesEnemy;

	Space* space;
	float scrollX;
	float scrollY;

	bool controlShoot = false;
	int controlMoveY = 0;
	int controlMoveX = 0;


	//para saber si las puertas están abiertas
	bool puertasAbiertas;

	// Elementos de interfaz
	SDL_GameController* gamePad;
	Pad* pad;
	Actor* buttonJump;
	Actor* buttonShoot;
	list<Actor*> hearts;

	int nBombs;
	list<Tile*> bombs;
	Actor* bombsActor;
	Text* textBombs;
};