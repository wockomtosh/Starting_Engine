#pragma once
#include <vector>
#include <memory>
#include "AxisAlignedBoundingBox.h"
#include "GameObjectFactory.h"
#include "nlohmann/json.hpp"
#include "Rigidbody.h"

namespace Collision {
	struct CollisionData
	{
		float collisionTime;
		Vector4 collisionNormal;
		std::weak_ptr<AABB> collidedObjects[2];
	};

	void initialize();
	bool findEarliestCollision(float dt, CollisionData& o_collisionData);
	void update(float dt);
	void addAABB(std::shared_ptr<GameObject> gameObject, nlohmann::json& rbSection);
};