#pragma once
#include <vector>
#include <memory>
#include "PhysicsComponent.h"
#include "GameObjectFactory.h"
#include "nlohmann/json.hpp"

namespace Physics {
	void initialize();
	void update(float dt);
	void addRigidbody(std::shared_ptr<GameObject> gameObject, nlohmann::json& rbSection);
	void addPhysicsObject(std::shared_ptr<GameObject> gameObject, nlohmann::json& physicsSection);
};