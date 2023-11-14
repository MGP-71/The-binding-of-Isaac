#include "Tile.h"

Tile::Tile(string filename, float x, float y, float width, float height, Game* game)
	: Actor(filename, x, y, width, height, game) {
	this->filename = filename;
}
