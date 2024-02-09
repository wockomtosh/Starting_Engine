#include "PhysicsSystem.h"
#include <malloc.h>

PhysicsSystem::PhysicsSystem(std::vector<PhysicsComponent*> physicsObjects) :
	physicsObjects(physicsObjects)
{
}

void PhysicsSystem::update(float dt)
{
	for (int i = 0; i < physicsObjects.size(); i++)
	{
		physicsObjects[i]->update(dt);
	}
}

void PhysicsSystem::addPhysicsObject(PhysicsComponent* object)
{
	physicsObjects.push_back(object);
}
