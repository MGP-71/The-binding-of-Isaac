#include "Enemy.h"
#include "ProjectileEnemy.h"
#include "Player.h"
#include "Monoojo.h"
class BigHorn : public Enemy
{
public:
	BigHorn(float x, float y, Game* game);
	void update(Player* p) override;
	ProjectileEnemy* shoot(Player* p);
	int shootCadence = 120;
	int shootTime = 0;
	int counterGenerate = 100;
	Monoojo* generateEnemy(int x, int y, Game* g);
};



