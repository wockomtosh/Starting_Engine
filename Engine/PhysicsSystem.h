#pragma once
#include <vector>
#include "PhysicsComponent.h"

class PhysicsSystem {
	//Maybe later we'll want a dedicated physics component to handle forces differently, but for now I'm just using the rigidbody

	std::vector<PhysicsComponent*> physicsObjects;

public:
	PhysicsSystem(std::vector<PhysicsComponent*> physicsObjects);

	void update(float dt);

	void addPhysicsObject(PhysicsComponent* object);
};