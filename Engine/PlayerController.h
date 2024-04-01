#pragma once
#include "GameObjectController.h"
#include "GameObject.h"
#include "nlohmann/json.hpp"
#include "PhysicsComponent.h"
#include "GLib.h"
#include <functional>

class PlayerController : public GameObjectController 
{
	std::weak_ptr<GameObject> player;

public:
	PlayerController(std::weak_ptr<GameObject> player);
	PlayerController(const PlayerController& other);

	void update(GameObject& controlledObject);
	
	static void createPlayerController(std::shared_ptr<GameObject> gameObject, nlohmann::json& physicsSection);

private:
	void handleKeyPress(unsigned int i_VKeyID, bool bWentDown);

	std::weak_ptr<PhysicsComponent> playerPhysics;
};