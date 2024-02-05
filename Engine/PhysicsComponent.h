#pragma once
#include "Rigidbody.h"

struct PhysicsComponent {
	Rigidbody* rigidbody = nullptr;
	float dragFactor = 0;
	Acceleration forces = { Vector2(), 0 };
	
	PhysicsComponent(Rigidbody* rigidbody, float dragFactor, Acceleration forces) :
		rigidbody(rigidbody), dragFactor(dragFactor), forces(forces) {}

	inline void update(float dt) {
		//Implementing some janky drag. Does this work as expected?
		rigidbody->velocity -= rigidbody->velocity.normalize() * dragFactor * dt;
		rigidbody->rotation -= dragFactor * dt;
		rigidbody->update(dt, forces);
	}
};