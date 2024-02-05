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
};

struct Rigidbody {
	GameObject* gameObject;
	Vector2 velocity = Vector2();
	float rotation = 0;
	float maxSpeed = 100;
	float maxRotation = 100;

	void update(float dt, Acceleration acceleration) {
		//Should I be clamping acceleration here or elsewhere?
		//Or do I need to clamp it at all if I'm clamping speed?

		velocity += dt * acceleration.linear;
		rotation += dt * acceleration.angular;

		//Clamp to max speed and rotation
		if (rotation > maxRotation)
		{
			rotation = maxRotation;
		}
		if (velocity.getMagnitude() > maxSpeed)
		{
			velocity = velocity.getVectorWithMagnitude(maxSpeed);
		}

		gameObject->position += dt * velocity;
		gameObject->orientation += dt * rotation;

		//Implement wraparound
		//if (gameObject->position.x > WINDOW_WIDTH)
		//{
		//	gameObject->position.x = 0;
		//}
		//else if (gameObject->position.x < 0)
		//{
		//	gameObject->position.x = WINDOW_WIDTH;
		//}
		//if (gameObject->position.y > WINDOW_HEIGHT)
		//{
		//	gameObject->position.y = 0;
		//}
		//else if (gameObject->position.y < 0)
		//{
		//	gameObject->position.y = WINDOW_HEIGHT;
		//}

		//Clamp orientation. I'm using degrees
		if (gameObject->orientation < 0)
		{
			gameObject->orientation += 360;
		}
		else if (gameObject->orientation > 360)
		{
			gameObject->orientation -= 360;
		}
	}

	float getOrientationOfMovement() {

	}
};