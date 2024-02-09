#include "GameObject.h"

GameObject::GameObject() :
	position(Vector2()), orientation(0), name(""), components({ {"", nullptr} })
{
}

GameObject::GameObject(Vector2 startingLocation) :
	position(startingLocation), orientation(0), name(""), components({ {"", nullptr} })
{
}

GameObject::GameObject(Vector2 startingLocation, std::map<std::string, void*> components) :
	position(startingLocation), orientation(0), name(""), components(components)
{
}

GameObject::GameObject(GameObject &oldObject)
{
	name = oldObject.name;
	position = oldObject.position;
	orientation = oldObject.orientation;
	components = oldObject.components;
}

Vector2 GameObject::getLocation()
{
	return position;
}

void GameObject::setPosition(Vector2 newLocation)
{
	position = newLocation;
}

std::string GameObject::getName()
{
	return name;
}

void GameObject::setName(std::string newName)
{
	name = newName;
}

void* GameObject::getComponent(const std::string componentName)
{
	if (!components.empty()) 
	{
		auto component = components.find(componentName);
		if (component == components.end())
		{
			return nullptr;
		}
		return components[componentName];
	}
	
}

void GameObject::addComponent(const std::string componentName, void* component)
{
	components[componentName] = component;
}

void GameObject::print()
{
	position.print();
}

GameObject::~GameObject()
{
	//TODO: delete components!!!
}
