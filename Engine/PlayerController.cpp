#include "PlayerController.h"

PlayerController::PlayerController(std::weak_ptr<GameObject> player) :
	player(player), playerPhysics(std::weak_ptr<PhysicsComponent>())
{
	playerIndex = numPlayers;
	numPlayers++;
}

PlayerController::PlayerController(const PlayerController& other)
{
	player = other.player;
	playerPhysics = other.playerPhysics;
	playerIndex = other.playerIndex;
}

void PlayerController::update(float dt, GameObject& controlledObject)
{

}

void PlayerController::createPlayerController(std::shared_ptr<GameObject> gameObject, nlohmann::json& initializerSection)
{
	//The gameObject should hold a shared_ptr to the controller, but the controller should hold a weak_ptr to the gameObject
	std::shared_ptr<PlayerController> newController = std::make_shared<PlayerController>(gameObject);
	gameObject->setCurrentController(newController);

	//The physics component relies on the rigidbody. Do I need to call some function to also ensure the rigidbody?
	std::shared_ptr<PhysicsComponent> physics = std::static_pointer_cast<PhysicsComponent>(gameObject->ensureComponent("physicsComponent", []()
		{
			return std::make_shared<PhysicsComponent>();
		}));
	gameObject->addComponent("physicsComponent", physics);
	//TODO: fix this!!
	newController->playerPhysics = physics;
}
