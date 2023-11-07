#include "Door.h"
Door::Door(string filename, float x, float y, Game* game)
	: Actor(filename, x, y, 51, 51, game) {
	this->filename = filename;
}


string Door::getPosicion()
{
	return posicion;
}
void Door::setPosicion(string posicion)
{
	this->posicion = posicion;
}
