#pragma once
#include <vector>
#include "PhysicsComponent.h"
#include "GameObjectFactory.h"
#include "nlohmann/json.hpp"

namespace Physics {
	void initialize();
	void update(float dt);
	void addRigidbody(GameObject& gameObject, nlohmann::json& rbSection);
	void addPhysicsObject(GameObject& gameObject, nlohmann::json& physicsSection);
};