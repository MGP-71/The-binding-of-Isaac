#pragma once

#include "Layer.h"
#include "Player.h"
#include "Background.h"

#include "Enemy.h"
#include "Projectile.h"
#include "Text.h"
#include "Tile.h"

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

	void loadMap(string name);
	void loadMapObject(char character, float x, float y);

	int mapWidth;
	list<Tile*> tiles;
	Audio* audioBackground;
	Text* textPoints;
	int points;
	int newEnemyTime = 0;
	Player* player;
	Background* background;
	Actor* backgroundPoints;
	list<Enemy*> enemies;
	list<Projectile*> projectiles;
	Space* space;

	bool controlShoot = false;
	int controlMoveY = 0;
	int controlMoveX = 0;


};