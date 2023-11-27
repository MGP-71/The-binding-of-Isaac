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
#include "Cain.h"
#include "Eve.h"
#include "Eden.h"
#include "Isaac.h"
#include "Character.h"
#include "Horf.h"
#include "Fatty.h"
#include "Monoojo.h"
#include "Judas.h"
#include "Azazel.h"

#include <fstream> // Leer ficheros
#include <sstream> // Leer líneas / String

#include <list>

class GameLayer : public Layer
{
public:
	GameLayer(Game* game, int personaje);
	void init() override;
	void processControls() override;
	void update() override;
	void draw() override;
	void keysToControls(SDL_Event event);

	//void calculateScroll();
	void deleteMap();
	void actualizarVidas();
	void actualizarBombas();
	void actualizarLlaves();
	void actualizarPills();
	void chooseCharater();
	void saveRoom(string nameFile);
	void checkRoomCleared();
	void dropBomb();
	double calculateDistance(Actor* actor1, Actor* actor2);

	int personaje;

	void loadMap(string name);

	int habitacionVertical;
	int habitacionHorizontal;
	void loadMapObject(char character, float x, float y);

	int mapWidth;
	int mapHeight;
	list<Tile*> tiles;
	list<Tile*> doors;
	list<Tile*> fuegos;
	int vecesInvencible = 0;

	Tile* activeBomb;
	time_t startBomb, endBomb;
	time_t timeExplosion;

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

	int nKeys;
	list<Tile*> keys;
	Actor* keysActor;
	Text* textKeys;

	list<Tile*> rocas;

	int nPills;
	list<Tile*> pills;
	Actor* pillsActor;
	list<Tile*> corazones;
	list<Tile*> objetos;
	void objetoConseguido(Tile* t);
	Text* objConseguido;
	Text* textActivo;

	Character* playerCharacter;

	int lastDoorCrossed = 0;

	list<string> roomsCleared;
	string nameFile;

	Azazel* azazel;
};