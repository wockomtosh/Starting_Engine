#include "PhysicsSystem.h"
#include <malloc.h>

PhysicsSystem::PhysicsSystem(PhysicsComponent** physicsObjects, int numPhysicsObjects) :
	physicsObjects(physicsObjects), numPhysicsObjects(numPhysicsObjects)
{
}

void PhysicsSystem::update()
{
	for (int i = 0; i < numPhysicsObjects; i++)
	{
		physicsObjects[i]->update(.0167);
	}
}

void PhysicsSystem::addPhysicsObject(PhysicsComponent* object)
{
	numPhysicsObjects++;
	PhysicsComponent** newPhysicsObjects = static_cast<PhysicsComponent**>(malloc(sizeof(PhysicsComponent) * (numPhysicsObjects)));
	for (int i = 0; i < numPhysicsObjects - 1; i++)
	{
		//copy over old physics objects to our new list
		newPhysicsObjects[i] = physicsObjects[i];
	}

	newPhysicsObjects[numPhysicsObjects - 1] = object;
		

	free(physicsObjects);

	physicsObjects = newPhysicsObjects;
}
