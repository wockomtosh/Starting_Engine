#include "PlayerController.h"

PlayerController::PlayerController(GameObject* player) :
	player(player), playerPhysics(nullptr)
{
	// IMPORTANT (if we want keypress info from GLib): Set a callback for notification of key presses
	//TODO: Understand std::bind better. 
	GLib::SetKeyStateChangeCallback(std::bind(&PlayerController::handleKeyPress, this, std::placeholders::_1, std::placeholders::_2));
}

void PlayerController::update(GameObject& controlledObject)
{
	//Empty for now
}

void PlayerController::createPlayerController(GameObject& gameObject, nlohmann::json& initializerSection)
{
	//TODO: currently the controller and the gameObject hold refs to each other, that seems like it could be a problem?
	//TODO: fix this!!
	PlayerController* newController = new PlayerController(&gameObject);
	gameObject.setCurrentController(newController);

	//The physics component relies on the rigidbody. Do I need to call some function to also ensure the rigidbody?
	PhysicsComponent* physics = static_cast<PhysicsComponent*>(gameObject.ensureComponent("physicsComponent", []()
		{
			return new PhysicsComponent();
		}));
	gameObject.addComponent("physicsComponent", physics);
	//TODO: fix this!!
	newController->playerPhysics = physics;
}

void PlayerController::handleKeyPress(unsigned int i_VKeyID, bool bWentDown)
{
	unsigned int w = 0x57;
	unsigned int a = 0x41;
	unsigned int s = 0x53;
	unsigned int d = 0x44;

	if (i_VKeyID == w)
	{
		if (bWentDown)
		{
			playerPhysics->forces.linear += Vector2::Up * 100;
		}
		else {
			playerPhysics->forces.linear -= Vector2::Up * 100;
		}
	}
	if (i_VKeyID == a)
	{
		if (bWentDown)
		{
			playerPhysics->forces.linear += Vector2::Left * 100;
		}
		else {
			playerPhysics->forces.linear -= Vector2::Left * 100;
		}
	}
	if (i_VKeyID == s)
	{
		if (bWentDown)
		{
			playerPhysics->forces.linear += Vector2::Down * 100;
		}
		else {
			playerPhysics->forces.linear -= Vector2::Down * 100;
		}
	}
	if (i_VKeyID == d)
	{
		if (bWentDown)
		{
			playerPhysics->forces.linear += Vector2::Right * 100;
		}
		else {
			playerPhysics->forces.linear -= Vector2::Right * 100;
		}
	}
}
