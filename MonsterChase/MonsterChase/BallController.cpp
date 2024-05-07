#include "BallController.h"

BallController::BallController(std::weak_ptr<GameObject> ball) :
	ball(ball)
{
	int posX = rand() % 2;
	int posY = rand() % 2;
	Vector2 startingVel = Vector2(100, 100);
	if (posX == 0)
	{
		startingVel.x *= -1;
	}
	if (posY == 0)
	{
		startingVel.y *= -1;
	}
	std::static_pointer_cast<Rigidbody>(ball.lock()->getComponent("rigidbody"))->velocity = startingVel;
}

BallController::BallController(const BallController& other)
{
	ball = other.ball;
}

void BallController::update(float dt, GameObject& controlledObject)
{
	//TODO: Link this to some game manager later to make it so that it tracks score
}

void BallController::createBallController(std::shared_ptr<GameObject> gameObject, nlohmann::json& initializerSection)
{
	//The gameObject should hold a shared_ptr to the controller, but the controller should hold a weak_ptr to the gameObject
	std::shared_ptr<BallController> newController = std::make_shared<BallController>(gameObject);
	gameObject->setCurrentController(newController);
}
