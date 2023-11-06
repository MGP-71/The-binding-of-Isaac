#pragma once

#include "Game.h"

class Actor
{
public:
	Actor(string filename, float x, float y, int width, int height, Game* game);
	~Actor();
	virtual void draw();
	bool isOverlap(Actor* actor);
	bool isInRender();
	SDL_Texture* texture;
	int x;
	int y;
	float vx;
	float vy;
	int width;
	int height;
	int fileWidth;
	int fileHeight;
	bool collisionUp;
	bool collisionRight;
	bool collisionDown;
	bool collisionLeft;
	bool outRight;
	bool outLeft;
	Game* game; // referencia al juego
};

