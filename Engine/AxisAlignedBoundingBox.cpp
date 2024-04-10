#include "AxisAlignedBoundingBox.h"

AABB::AABB(std::shared_ptr<Rigidbody> i_rb) :
	body(i_rb)
{
	gameObject = i_rb->gameObject;
}

bool AABB::isPointWithinBox(float x, float y)
{
	return !(x > center.x + extents.x || x < center.x - extents.x
		|| y > center.y + extents.y || y < center.y - extents.y);
}

bool AABB::isPointWithinBox(Vector2 point)
{
	return isPointWithinBox(point.x, point.y);
}

//If neither are rotated then we can just do the simple check
bool simpleCollisionCheck(AABB* first, Vector2 firstPos, AABB* second, Vector2 secondPos)
{
	bool xSeparation = fabs((firstPos.x + first->center.x) - (secondPos.x + second->center.x))
		> first->extents.x + second->extents.x;
	if (xSeparation)
	{
		//We don't need to calculate y if x is already separated.
		return false;
	}

	bool ySeparation = fabs((firstPos.y + first->center.y) - (secondPos.y + second->center.y))
		> first->extents.y + second->extents.y;
	return !ySeparation;
}

Matrix4 getAToBMatrix(AABB* a, AABB* b)
{
	//Remember that this assumes column vectors
	return b->getToLocalMatrix() * a->getToWorldMatrix();
}

//For a collision check we need to check A in B and vice versa. Any separation lets you stop early.
//We can use this one if both objects are stationary.
bool checkAInBStationary(AABB* a, AABB* b)
{
	Matrix4 aToB = getAToBMatrix(a, b);

	//Transform our center, x and y extents into b's coordinate system
	Vector4 aCenterInB = aToB * Vector4(a->center);
	Vector4 aExtentsXInB = aToB * Vector4(a->extents.x, 0, 0, 0);
	Vector4 aExtentsYInB = aToB * Vector4(a->extents.y, 0, 0, 0);

	//Take just the x values of the new x and y extents
	float xAExtentsProjectedOntoB = fabs(aExtentsXInB.x) + fabs(aExtentsYInB.x);
	float xCenterDistance = fabs(aCenterInB.x - b->center.x);
	bool xSeparation = xCenterDistance > xAExtentsProjectedOntoB + b->extents.x;
	
	if (xSeparation)
	{
		return false;
	}

	//Now the same for the y values
	float yAExtentsProjectedOntoB = fabs(aExtentsXInB.y) + fabs(aExtentsYInB.y);
	float yCenterDistance = fabs(aCenterInB.y - b->center.y);
	bool ySeparation = yCenterDistance > yAExtentsProjectedOntoB + b->extents.y;
	
	return !ySeparation;
}

//Because we need to check all the open/close times at the end, this returns the latest close time followed by the earliest open time
bool checkAInBMoving(float dt, AABB* a, Vector2 aVel, AABB* b, Vector2 bVel, float& o_closeTime, float& o_openTime)
{
	Matrix4 aToB = getAToBMatrix(a, b);

	//Get velocity, center, extents in B
	//Treat B as stationary (and this also combines the velocities nicely)
	Vector2 aVelRelToB = aVel - bVel;
	Vector4 aVelInB = aToB * Vector4(aVelRelToB);
	Vector4 aCenterInB = aToB * Vector4(a->center, 0, 1);
	Vector4 aExtentsXInB = aToB * Vector4(a->extents.x, 0, 0, 0);
	Vector4 aExtentsYInB = aToB * Vector4(0, a->extents.y, 0, 0);

	//X-axis check
	//Get the extents and combine them for simplification
	float aExtentsProjectedOntoB = fabs(aExtentsXInB.x) + fabs(aExtentsYInB.x);
	float combinedExtents = aExtentsProjectedOntoB + b->extents.x;

	//Get the left and right side of B
	float bLeft = b->center.x - combinedExtents;
	float bRight = b->center.x + combinedExtents;

	//If velocity is 0 just check if we're between bLeft and bRight
	if (aVelInB.x == 0)
	{
		//If we're less than left or greater than right we're outside, which means we have separation
		if (aCenterInB.x < bLeft || aCenterInB.x > bRight)
		{
			return false;
		}
		//If we're inside then we still need to continue to check other axes
	}
	//If we're moving
	else 
	{
		//Calculate the distance until the separation axis closes/opens
		float closeDistance = bLeft - aCenterInB.x;
		float openDistance = bRight - aCenterInB.x;

		//Get the time that the axis will close/open
		float xCloseTime = closeDistance / aVelInB.x;
		float xOpenTime = openDistance / aVelInB.x;

		//Instead of doing this should we just check if aCenterInB is to the left/right? IDK how much the division matters in this situation, but division can be expensive.
		if (xOpenTime < xCloseTime)
		{
			//Our default is to have A on the left side of B and this means that it's on the other side so we swap.
			std::swap(xOpenTime, xCloseTime);
		}

		if (xCloseTime > dt)
		{
			return false;
		}

		if (xOpenTime < 0)
		{
			return false;
		}

		//Check if these are the latest close time or earliest open time, and if so then replace
		if (xCloseTime > o_closeTime)
		{
			o_closeTime = xCloseTime;
		}
		if (xOpenTime < o_openTime)
		{
			o_openTime = xOpenTime;
		}
	}

	//Y-axis check
	aExtentsProjectedOntoB = fabs(aExtentsXInB.y) + fabs(aExtentsYInB.y);
	combinedExtents = aExtentsProjectedOntoB + b->extents.y;

	bLeft = b->center.y - combinedExtents;
	bRight = b->center.y + combinedExtents;

	if (aVelInB.y == 0)
	{
		if (aCenterInB.y < bLeft || aCenterInB.y > bRight)
		{
			return false;
		}
	}
	else
	{
		float closeDistance = bLeft - aCenterInB.y;
		float openDistance = bRight - aCenterInB.y;

		float yCloseTime = closeDistance / aVelInB.y;
		float yOpenTime = openDistance / aVelInB.y;

		if (yOpenTime < yCloseTime)
		{
			std::swap(yOpenTime, yCloseTime);
		}

		if (yCloseTime > dt)
		{
			return false;
		}

		if (yOpenTime < 0)
		{
			return false;
		}

		if (yCloseTime > o_closeTime)
		{
			o_closeTime = yCloseTime;
		}
		if (yOpenTime < o_openTime)
		{
			o_openTime = yOpenTime;
		}
	}
}

bool AABB::isColliding(float dt, std::shared_ptr<AABB> other)
{
	//Make sure both gameObjects still exist (otherwise we can just return false)
	//Because weak_ptr.lock() is relatively expensive I should think about finding a way to
	//only use it once (per object) later. One option is to update internal position/orientation values whenever we get a lock?
	if (auto selfRB = body.lock())
	{
		if (auto otherRB = other->body.lock())
		{
			//If one of them is rotated and the other isn't then we can avoid a matrix inversion
			//by doing things in world space. I'm not going to do that right now.

			float latestCloseTime = 0;
			float earliestOpenTime = 1;

			//If either return a separation then we're good
			if (!checkAInBMoving(dt, this, selfRB->velocity, other.get(), otherRB->velocity, latestCloseTime, earliestOpenTime))
			{
				return false;
			}
			if (!checkAInBMoving(dt, other.get(), otherRB->velocity, this, selfRB->velocity, latestCloseTime, earliestOpenTime))
			{
				return false;
			}

			//If we couldn't find any separation we have one more check
			//We need to track close and open time for each axis. 
			//If the latest close time was after the earliest open time then a new separation opened before the last one closed, which means no collision
			//If the latest close time was before the earliest open time then all the separations closed before any other separations could open - collision
			//By default this is true, and that's by design. If both have 0 velocity and neither check returns false then it must be true.
			return latestCloseTime < earliestOpenTime;
		}
	}
	return false;
}

Matrix4 AABB::getToWorldMatrix()
{
	if (auto selfObject = gameObject.lock())
	{
		//This assumes column vectors
		Matrix4 translation = Matrix4::CreateTranslation(selfObject->position.x, selfObject->position.y);
		Matrix4 rotation = Matrix4::CreateZRotation(selfObject->orientation);
		return translation * rotation;
	}
	return Matrix4::CreateIdentity();
}

Matrix4 AABB::getToLocalMatrix()
{
	return getToWorldMatrix().GetInverse();
}
