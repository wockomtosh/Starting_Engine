#include "Rigidbody.h"
#include <corecrt_math.h>
#include <iostream>
#include "Orientation.h"

Rigidbody::Rigidbody(std::weak_ptr<GameObject> i_gameObject)
{
	gameObject = i_gameObject;
}

void Rigidbody::update(float dt, Acceleration acceleration) 
{
	if (auto object = gameObject.lock())
	{
		velocity += dt * acceleration.linear;
		rotation += dt * acceleration.angular;

		//Clamp to max speed and rotation
		if (rotation > maxRotation)
		{
			rotation = maxRotation;
		}
		if (rotation < -maxRotation)
		{
			rotation = -maxRotation;
		}
		if (velocity.getMagnitude() > maxSpeed)
		{
			velocity = velocity.getVectorWithMagnitude(maxSpeed);
		}

		object->position += dt * velocity;
		object->orientation += dt * rotation;

		object->orientation = Orientation::clampOrientation(object->orientation);
	}
}

float Rigidbody::getOrientationOfMovement()
{
	float oMovement = Orientation::radToDeg(atan2(velocity.y, velocity.x));
	//This function gives 0 orientation as pointing to the right, so we need to offset that properly.
	oMovement += 90;

	oMovement = Orientation::clampOrientation(oMovement);

	return oMovement;
}