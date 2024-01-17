#pragma once
#include "Point2D.h"

class GameObject
{
public:
	Point2D location;
	const char* name;

	GameObject();
	GameObject(Point2D startingLocation);

	Point2D getLocation();
	void setLocation(Point2D newLocation);
	const char* getName();
	void setName(const char* newName);

	void print();

	~GameObject() {}
};