#include "GameObject.h"

GameObject::GameObject()
{
	location = Point2D();
	name = "";
}

GameObject::GameObject(Point2D startingLocation)
{
	location = startingLocation;
	name = "";
}

Point2D GameObject::getLocation()
{
	return location;
}

void GameObject::setLocation(Point2D newLocation)
{
	location = newLocation;
}

const char* GameObject::getName()
{
	return name;
}

void GameObject::setName(const char* newName)
{
	name = newName;
}

void GameObject::print()
{
	location.print();
}
