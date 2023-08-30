#pragma once
#include "Point2D.h"

class GameObject
{
	Point2D location;
	const char* name;

public:
	GameObject();
	GameObject(Point2D startingLocation);

	Point2D getLocation();
	void setLocation(Point2D newLocation);
	const char* getName();
	void setName(const char* newName);

	void print();

	~GameObject() {}
};