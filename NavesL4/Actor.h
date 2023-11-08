#pragma once

#include "Game.h"

class Actor
{
public:
	Actor(string filename, float x, float y, int width, int height, Game* game);
	~Actor();
	virtual void draw(float scrollX = 0, float scrollY = 0);
	bool isOverlap(Actor* actor);
	bool isInRender(float scrollX = 0, float scrollY = 0);
	bool containsPoint(int pointX, int pointY); // contiene punto
	SDL_Texture* texture;
	int x;
	int y;
	string filename;
	float vx;
	float vy;
	int width;
	int height;
	int fileWidth;
	int fileHeight;
	bool collisionTop;
	bool collisionRight;
	bool collisionDown;
	bool collisionLeft;
	bool outRight;
	bool outLeft;
	Game* game; // referencia al juego
};

