#pragma once
#include <vector>
#include <memory>
#include "AxisAlignedBoundingBox.h"
#include "GameObjectFactory.h"
#include "nlohmann/json.hpp"
#include "Rigidbody.h"

namespace Collision {
	void initialize();
	void update(float dt);
	void addAABB(std::shared_ptr<GameObject> gameObject, nlohmann::json& rbSection);
};