#pragma once
#include "Rigidbody.h"

struct PhysicsComponent {
	std::weak_ptr<Rigidbody> rigidbody;
	float dragFactor = 0;
	Acceleration forces = { Vector2(), 0 };
	
	PhysicsComponent() {}

	PhysicsComponent(std::weak_ptr<Rigidbody> rigidbody, float dragFactor, Acceleration forces) :
		rigidbody(rigidbody), dragFactor(dragFactor), forces(forces) {}

	PhysicsComponent(const PhysicsComponent& otherComponent)
	{
		rigidbody = otherComponent.rigidbody;
		dragFactor = otherComponent.dragFactor;
		forces = otherComponent.forces;
	}

	inline void update(float dt) {
		//Implementing some janky drag. Does this work as expected?
		if (auto rb = rigidbody.lock())
		{
			rb->velocity -= rb->velocity.normalize() * dragFactor * dt;
			rb->rotation -= rb->rotation * dragFactor * dt;
			rb->update(dt, forces);
		}
	}
};