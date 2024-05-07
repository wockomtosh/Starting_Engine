#pragma once
#include "GameObjectController.h"
#include "GameObject.h"
#include "nlohmann/json.hpp"
#include "PhysicsComponent.h"
#include "AxisAlignedBoundingBox.h"
#include "GLib.h"
#include <functional>

class BallController : public GameObjectController
{
	std::weak_ptr<GameObject> ball;

public:
	BallController(std::weak_ptr<GameObject> ball);
	BallController(const BallController& other);

	void update(float dt, GameObject& controlledObject);

	static void createBallController(std::shared_ptr<GameObject> gameObject, nlohmann::json& physicsSection);

private:
	//std::weak_ptr<PhysicsComponent> playerPhysics;
};