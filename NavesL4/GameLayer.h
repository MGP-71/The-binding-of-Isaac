#pragma once

#include "Layer.h"
#include "Player.h"
#include "Background.h"

#include "Enemy.h"
#include "Projectile.h"
#include "Text.h"
#include "Tile.h"
#include "Horf.h"
#include "ProjectileEnemy.h"

#include "Audio.h"
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

	void loadMap(string name);
	void loadMapObject(char character, float x, float y);

	int mapWidth;
	int mapHeight;
	list<Tile*> tiles;
	list<Tile*> doors;

	Audio* audioBackground;
	int newEnemyTime = 0;
	Player* player;
	Background* background;
	list<Enemy*> enemies;
	list<Horf*> horfEnemies;
	list<Projectile*> projectiles;
	list<ProjectileEnemy*> projectilesHorf;

	Space* space;
	float scrollX;
	float scrollY;

	bool controlShoot = false;
	int controlMoveY = 0;
	int controlMoveX = 0;


	//para saber si las puertas están abiertas
	bool puertasAbiertas;

};