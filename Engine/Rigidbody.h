#pragma once
#include "Vector2.h"
#include "GameObject.h"

//Where do these variables belong? It makes sense to have them in ofApp.h,
//but it makes sense to clamp position in the rigidbody
static const int WINDOW_WIDTH = 1024;
static const int WINDOW_HEIGHT = 768;

struct Acceleration {
	Vector2 linear;
	float angular;
	Acceleration(Vector2 linear, float angular) : linear(linear), angular(angular) {}
	Acceleration()
	{
		linear = Vector2();
		angular = 0;
	}
};

struct Rigidbody {
	std::weak_ptr<GameObject> gameObject;
	Vector2 velocity = Vector2();
	float rotation = 0;
	float maxSpeed = 100;
	float maxRotation = 100;
	float mass = 1;

	Rigidbody() {}
	Rigidbody(std::weak_ptr<GameObject> i_gameObject);
	~Rigidbody() {}

	void update(float dt, Acceleration acceleration);
	float getOrientationOfMovement();
};