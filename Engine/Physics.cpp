#include "Physics.h"
#include <malloc.h>

namespace Physics
{
	std::vector<PhysicsComponent*> physicsObjects;

	void initialize()
	{
		GameObjectFactory::RegisterComponentCreator("rigidbody", addRigidbody);
		GameObjectFactory::RegisterComponentCreator("physicsComponent", addPhysicsObject);
	}

	void update(float dt)
	{
		for (int i = 0; i < physicsObjects.size(); i++)
		{
			physicsObjects[i]->update(dt);
		}
	}

	void addRigidbody(GameObject& gameObject, nlohmann::json& rbSection)
	{
		//TODO: How do I ensure that the PhysicsComponent points to this Rigidbody when I can't guarantee that it'll be made first?
		Rigidbody* newRb = new Rigidbody(&gameObject);

		float maxSpeed = newRb->maxSpeed;
		float maxRotation = newRb->maxRotation;
		if (rbSection["maxSpeed"].is_number())
		{
			maxSpeed = rbSection["maxSpeed"];
		}
		if (rbSection["maxRotation"].is_number())
		{
			maxSpeed = rbSection["maxRotation"];
		}

		gameObject.addComponent("rigidbody", newRb);
	}

	void addPhysicsObject(GameObject& gameObject, nlohmann::json& physicsSection)
	{
		//TODO: Find a better way to handle this.
		Rigidbody* rb = static_cast<Rigidbody*>(gameObject.ensureComponent("rigidbody", []()
			{
				return new Rigidbody();
			}));
		rb->gameObject = &gameObject;

		float dragFactor = 0;
		if (physicsSection["dragFactor"].is_number())
		{
			dragFactor = physicsSection["dragFactor"];
		}

		PhysicsComponent* physicsObject = new PhysicsComponent(rb, dragFactor, Acceleration());

		physicsObjects.push_back(physicsObject);
		gameObject.addComponent("physicsComponent", physicsObject);
	}
}

