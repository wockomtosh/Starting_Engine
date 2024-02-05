#pragma once
#include "PhysicsComponent.h"

class PhysicsSystem {
	//Maybe later we'll want a dedicated physics component to handle forces differently, but for now I'm just using the rigidbody

	PhysicsComponent** physicsObjects;
	int numPhysicsObjects;

public:
	PhysicsSystem(PhysicsComponent** physicsObjects, int numPhysicsObjects);

	void update();

	void addPhysicsObject(PhysicsComponent* object);
};