#include "PlayerController.h"

PlayerController::PlayerController(GameObject* player) : 
	player(player)
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
	//For now I don't actually need to initialize anything in the controller, I just need to get a controller that points to the game object
	//TODO: currently the controller and the gameObject hold refs to each other, that seems like it could be a problem?
	PlayerController* newController = new PlayerController(&gameObject);

	gameObject.setCurrentController(newController);
	//I do need to make sure the player has a physics component though, which will run into the same issue that I'm facing with the Rigidbody
	/*PhysicsComponent* physics = static_cast<PhysicsComponent*>(gameObject.ensureComponent("physicsComponent", []()
		{
			return new PhysicsComponent();
		}));*/
	//I can't do it the same way that I did it with the rigidbody, because it also requires the rigidbody on top of the physics component.
	//I think for this and the rigidbody, I need to search the top level json for the component and throw an error if it isn't there.
	//Or we'll need to enforce an order for creating objects and components.
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
			static_cast<PhysicsComponent*>(player->getComponent("physicsComponent"))->forces.linear += Vector2::Up * 100;
		}
		else {
			static_cast<PhysicsComponent*>(player->getComponent("physicsComponent"))->forces.linear -= Vector2::Up * 100;
		}
	}
	if (i_VKeyID == a)
	{
		if (bWentDown)
		{
			static_cast<PhysicsComponent*>(player->getComponent("physicsComponent"))->forces.linear += Vector2::Left * 100;
		}
		else {
			static_cast<PhysicsComponent*>(player->getComponent("physicsComponent"))->forces.linear -= Vector2::Left * 100;
		}
	}
	if (i_VKeyID == s)
	{
		if (bWentDown)
		{
			static_cast<PhysicsComponent*>(player->getComponent("physicsComponent"))->forces.linear += Vector2::Down * 100;
		}
		else {
			static_cast<PhysicsComponent*>(player->getComponent("physicsComponent"))->forces.linear -= Vector2::Down * 100;
		}
	}
	if (i_VKeyID == d)
	{
		if (bWentDown)
		{
			PhysicsComponent* comp = static_cast<PhysicsComponent*>(player->getComponent("physicsComponent"));
			comp->forces.linear += Vector2::Right * 100;
		}
		else {
			static_cast<PhysicsComponent*>(player->getComponent("physicsComponent"))->forces.linear -= Vector2::Right * 100;
		}
	}
}
