#pragma once
#include "Actor.h"

class Door : public Actor
{
public:
	Door(string filename, float x, float y, Game* game);
	string posicion;
	string getPosicion();
	void setPosicion(string posicion);
};
