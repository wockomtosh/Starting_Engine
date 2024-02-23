#pragma once
#include "GameObjectController.h"
#include "GameObject.h"
#include "nlohmann/json.hpp"
#include "PhysicsComponent.h"
#include "GLib.h"
#include <functional>

class PlayerController : public GameObjectController 
{
	std::shared_ptr<GameObject> player;

public:
	PlayerController(GameObject* player);

	virtual void update(GameObject& controlledObject) override;
	
	static void createPlayerController(GameObject& gameObject, nlohmann::json& physicsSection);

private:
	void handleKeyPress(unsigned int i_VKeyID, bool bWentDown);
};