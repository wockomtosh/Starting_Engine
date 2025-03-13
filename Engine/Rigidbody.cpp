#include "Rigidbody.h"
#include <corecrt_math.h>
#include <iostream>
#include "Orientation.h"

Rigidbody::Rigidbody(GameObject* i_gameObject)
{
	gameObject = i_gameObject;
}

void Rigidbody::update(float dt, Acceleration acceleration) {

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

	gameObject->position += dt * velocity;
	gameObject->orientation += dt * rotation;

	//Implement wraparound
	//if (position.x > WINDOW_WIDTH)
	//{
	//	position.x = 0;
	//}
	//else if (position.x < 0)
	//{
	//	position.x = WINDOW_WIDTH;
	//}
	//if (position.y > WINDOW_HEIGHT)
	//{
	//	position.y = 0;
	//}
	//else if (position.y < 0)
	//{
	//	position.y = WINDOW_HEIGHT;
	//}

	//ANOTHER OPTION: Just don't let them pass the edges.
	//if (position.x > WINDOW_WIDTH)
	//{
	//	position.x = WINDOW_WIDTH;
	//}
	//else if (position.x < 0)
	//{
	//	position.x = 0;
	//}
	//if (position.y > WINDOW_HEIGHT)
	//{
	//	position.y = WINDOW_HEIGHT;
	//}
	//else if (position.y < 0)
	//{
	//	position.y = 0;
	//}

	gameObject->orientation = Orientation::clampOrientation(gameObject->orientation);
}

float Rigidbody::getOrientationOfMovement()
{
	float oMovement = atan2(velocity.y, velocity.x) * (180 / 3.1415);
	//This function gives 0 orientation as pointing to the right, so we need to offset that properly.
	oMovement += 90;

	oMovement = Orientation::clampOrientation(oMovement);

	return oMovement;
}