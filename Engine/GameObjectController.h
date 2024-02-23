#pragma once

//If we declare it here we don't have to include GameObject.h
class GameObject;

class GameObjectController
{
public:
	virtual void update(GameObject& controlledObject) = 0;
};