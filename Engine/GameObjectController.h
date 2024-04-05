#pragma once
//If we declare it here we don't have to include GameObject.h
class GameObject;

class GameObjectController
{
public:
	//I think it makes sense here to take a direct reference rather than a pointer,
	//since the GameObject is the one passing itself into the controller update.
	virtual void update(float dt, GameObject& controlledObject) = 0;
};