#pragma once
#include "Vector2.h"
#include <map>
#include <string>

class GameObject
{
	std::map<std::string, void*> components;

public:
	Vector2 position;
	float orientation;
	std::string name;

	GameObject();
	GameObject(Vector2 startingLocation);
	GameObject(Vector2 startingLocation, std::map<std::string, void*> components);

	Vector2 getLocation();
	void setPosition(Vector2 newPosition);
	std::string getName();
	void setName(std::string newName);

	void* getComponent(const std::string componentName);
	void addComponent(const std::string componentName, void* component);

	void print();

	~GameObject() {}
};