#include "PlayerController.h"

PlayerController::PlayerController(std::weak_ptr<GameObject> player) :
	player(player), playerPhysics(std::weak_ptr<PhysicsComponent>())
{
	// IMPORTANT (if we want keypress info from GLib): Set a callback for notification of key presses
	//TODO: Understand std::bind better. 
	GLib::SetKeyStateChangeCallback(std::bind(&PlayerController::handleKeyPress, this, std::placeholders::_1, std::placeholders::_2));
}

PlayerController::PlayerController(const PlayerController& other)
{
	player = other.player;
	playerPhysics = other.playerPhysics;
}

void PlayerController::update(GameObject& controlledObject)
{
	//Empty for now
}

void PlayerController::createPlayerController(std::shared_ptr<GameObject> gameObject, nlohmann::json& initializerSection)
{
	//The gameObject should hold a shared_ptr to the controller, but the controller should hold a weak_ptr to the gameObject
	std::shared_ptr<PlayerController> newController = std::make_shared<PlayerController>(PlayerController(gameObject));
	gameObject->setCurrentController(newController);

	//The physics component relies on the rigidbody. Do I need to call some function to also ensure the rigidbody?
	std::shared_ptr<PhysicsComponent> physics = std::static_pointer_cast<PhysicsComponent>(gameObject->ensureComponent("physicsComponent", []()
		{
			return std::make_shared<PhysicsComponent>(PhysicsComponent());
		}));
	gameObject->addComponent("physicsComponent", physics);
	//TODO: fix this!!
	newController->playerPhysics = physics;
}

void PlayerController::handleKeyPress(unsigned int i_VKeyID, bool bWentDown)
{
	if (auto pPhysics = playerPhysics.lock())
	{
		unsigned int w = 0x57;
		unsigned int a = 0x41;
		unsigned int s = 0x53;
		unsigned int d = 0x44;

		if (i_VKeyID == w)
		{
			if (bWentDown)
			{
				pPhysics->forces.linear += Vector2::Up * 100;
			}
			else {
				pPhysics->forces.linear -= Vector2::Up * 100;
			}
		}
		if (i_VKeyID == a)
		{
			if (bWentDown)
			{
				pPhysics->forces.linear += Vector2::Left * 100;
			}
			else {
				pPhysics->forces.linear -= Vector2::Left * 100;
			}
		}
		if (i_VKeyID == s)
		{
			if (bWentDown)
			{
				pPhysics->forces.linear += Vector2::Down * 100;
			}
			else {
				pPhysics->forces.linear -= Vector2::Down * 100;
			}
		}
		if (i_VKeyID == d)
		{
			if (bWentDown)
			{
				pPhysics->forces.linear += Vector2::Right * 100;
			}
			else {
				pPhysics->forces.linear -= Vector2::Right * 100;
			}
		}
	}
}
