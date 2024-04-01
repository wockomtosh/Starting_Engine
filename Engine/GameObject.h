#pragma once
#include "Vector2.h"
#include <map>
#include <string>
#include <functional>
#include <memory>
#include "GameObjectController.h"

class GameObject
{
	//TODO: make a component interface?? It would basically just have name and type, but it could be useful.
	std::map<std::string, std::shared_ptr<void>> components;

	std::shared_ptr<GameObjectController> controller;

public:
	std::string name;
	Vector2 position;
	float orientation;

	GameObject();
	GameObject(Vector2 startingPosition);
	GameObject(Vector2 startingPosition, std::map<std::string, std::shared_ptr<void>> components);
	GameObject(Vector2 startingPosition, float startingOrientation);
	GameObject(const GameObject& oldObject);

	std::shared_ptr<void> getComponent(const std::string componentName);
	void addComponent(const std::string componentName, std::shared_ptr<void> component);
	std::shared_ptr<void> ensureComponent(std::string componentName, std::function<std::shared_ptr<void>(void)> componentCreator);

	inline void setCurrentController(std::shared_ptr<GameObjectController> newController)
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