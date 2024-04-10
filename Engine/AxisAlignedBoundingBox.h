#pragma once
#include "GameObject.h"
#include "Vector2.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Rigidbody.h"

struct AABB {
	std::weak_ptr<GameObject> gameObject;
	std::weak_ptr<Rigidbody> body;
	Vector2 center = Vector2();
	Vector2 extents = Vector2();

	AABB() {}
	//I'm not using this everywhere since I wrote some other code before, but this constructor is likely better than the gameObject constructor
	AABB(std::shared_ptr<Rigidbody> i_rb);
	AABB(std::weak_ptr<GameObject> i_gameObject) :
		gameObject(i_gameObject) {}
	AABB(std::weak_ptr<GameObject> i_gameObject, Vector2 center, Vector2 extents) : 
		gameObject(i_gameObject), center(center), extents(extents) {}

	bool isPointWithinBox(float x, float y);
	bool isPointWithinBox(Vector2 point);
	bool isColliding(float dt, std::shared_ptr<AABB> other);

	Matrix4 getToWorldMatrix();
	Matrix4 getToLocalMatrix();
};