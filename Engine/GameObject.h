#pragma once
#include "Vector2.h"
#include <map>
#include <string>
#include <functional>
#include "GameObjectController.h"

class GameObject
{
	//TODO: make a component interface?? It would basically just have name and type, but it could be useful.
	std::map<std::string, void*> components;

	GameObjectController* controller;

public:
	std::string name;
	Vector2 position;
	float orientation;

	GameObject();
	GameObject(Vector2 startingPosition);
	GameObject(Vector2 startingPosition, std::map<std::string, void*> components);
	GameObject(Vector2 startingPosition, float startingOrientation);
	GameObject(const GameObject& oldObject);

	void* getComponent(const std::string componentName);
	void addComponent(const std::string componentName, void* component);
	void* ensureComponent(const char* componentName, std::function<void*(void)> componentCreator);

	inline void setCurrentController(GameObjectController* newController)
	{
		controller = newController;
	}

	inline void update()
	{
		if (controller)
		{
			controller->update(*this);
		}
	}

	void print();

	~GameObject();
};