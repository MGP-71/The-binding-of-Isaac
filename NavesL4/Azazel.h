#include "Enemy.h"
#include "ProjectileEnemy.h"
#include "Player.h"
class Azazel : public Enemy
{
public:
	Azazel(float x, float y, Game* game);
	void update(Player* p) override;
	ProjectileEnemy* shoot(Player* p);
	int shootCadence = 120;
	int shootTime = 0;
};



