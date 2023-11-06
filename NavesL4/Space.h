#pragma once
#include "Actor.h"
#include <list>

class Space
{
public:
	Space(float gravity);
	void addDynamicActor(Actor* actor);
	void addStaticActor(Actor* actor);
	void removeDynamicActor(Actor* actor);
	void removeStaticActor(Actor* actor);
	void update();
	void updateMoveRight(Actor* dynamicAct);
	void updateMoveLeft(Actor* dynamicAct);
	void updateMoveTop(Actor* dynamicAct);
	void updateMoveDown(Actor* dynamicAct);
	float gravity;
	list<Actor*> dynamicActors;
	list<Actor*> staticActors;
};


