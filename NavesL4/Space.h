#pragma once
#include "Actor.h"
#include <list>

class Space
{
public:
	Space();
	void addDynamicActor(Actor* actor);
	void addStaticActor(Actor* actor);
	void removeDynamicActor(Actor* actor);
	void removeStaticActor(Actor* actor);
	void update();
	void updateMoveRight(Actor* dynamicAct);
	void updateMoveLeft(Actor* dynamicAct);
	void updateMoveUp(Actor* dynamicAct);
	void updateMoveDown(Actor* dynamicAct);

	list<Actor*> dynamicActors;
	list<Actor*> staticActors;
};


