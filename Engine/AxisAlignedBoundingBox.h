#pragma once
#include "GameObject.h"
#include "Vector2.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Rigidbody.h"
#include "Orientation.h"

//Elastic collisions follow conservation of momentum (mass/velocity calcs).
//Blocking colliders just can't pass other blocking colliders/elastic colliders.
//Overlap colliders don't respond to collisions physically, I need to add some sort onOverlap function for that.
//Could also include things like static/dynamic. If we have a static collider, we only need to check it against other static colliders once
//And the collision response for a static collider is to do nothing. 
//In this case blocking takes the role that I would need for static, but static could give more optimization later.
enum CollisionType { blocking, elastic, overlap };

struct AABB {
	std::weak_ptr<GameObject> gameObject;
	std::weak_ptr<Rigidbody> body;
	Vector2 center = Vector2();
	Vector2 extents = Vector2();
	CollisionType type = CollisionType::blocking;

	AABB() {}
	//I'm not using this everywhere since I wrote some other code before, but this constructor is likely better than the gameObject constructor
	AABB(std::shared_ptr<Rigidbody> i_rb);
	AABB(std::weak_ptr<GameObject> i_gameObject) :
		gameObject(i_gameObject) {}
	AABB(std::weak_ptr<GameObject> i_gameObject, Vector2 center, Vector2 extents, CollisionType type = CollisionType::elastic) : 
		gameObject(i_gameObject), center(center), extents(extents), type(type) {}

	bool isPointWithinBox(float x, float y);
	bool isPointWithinBox(Vector2 point);
	bool isColliding(float dt, std::shared_ptr<AABB> other, float& collisionTime, Vector4& collisionNormal);

	//TODO: There's something weird with how the sprites get rotated compared to the AABBs
	inline Matrix4 getToWorldMatrix()
	{
		if (auto selfObject = gameObject.lock())
		{
			//This assumes column vectors
			Matrix4 translation = Matrix4::CreateTranslation(selfObject->position.x, selfObject->position.y);
			//TODO: For some reason I need to switch the rotation here to get the right normal
			//This has gotta be related to the other issue with sprite/AABB rotation
			Matrix4 rotation = Matrix4::CreateZRotation(Orientation::degToRad(-selfObject->orientation));
			return translation * rotation;
		}
		return Matrix4::CreateIdentity();
	}
	inline Matrix4 getToLocalMatrix()
	{
		return getToWorldMatrix().GetInverse();
	}
};