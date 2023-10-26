#pragma once

#include "Game.h"
class Game;

class Layer
{
public:
	Layer(Game* game);
	virtual void init() {};
	virtual void processControls() {};
	virtual void update() {};
	virtual void draw() {};

	Game* game;
};
