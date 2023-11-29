#include "Enemy.h"
#include "ProjectileEnemy.h"
#include "Player.h"
class Jefe2 : public Enemy
{
public:
	Jefe2(float x, float y, Game* game);
	void update(Player* p) override;
	ProjectileEnemy* shoot(Player* p);
	int shootCadence = 120;
	int shootTime = 0;
	int stopTime;
};



