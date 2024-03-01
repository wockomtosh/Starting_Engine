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
		Rigidbody* rb = static_cast<Rigidbody*>(gameObject.getComponent("rigidbody"));
		if (rb == nullptr)
		{
			rb = new Rigidbody(&gameObject);
			gameObject.addComponent("rigidbody", rb);
		}

		float maxSpeed = rb->maxSpeed;
		float maxRotation = rb->maxRotation;
		if (rbSection["maxSpeed"].is_number())
		{
			maxSpeed = rbSection["maxSpeed"];
		}
		if (rbSection["maxRotation"].is_number())
		{
			maxSpeed = rbSection["maxRotation"];
		}
	}

	void addPhysicsObject(GameObject& gameObject, nlohmann::json& physicsSection)
	{
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

		PhysicsComponent* physicsObject = static_cast<PhysicsComponent*>(gameObject.getComponent("physicsComponent"));
		if (physicsObject == nullptr)
		{
			physicsObject = new PhysicsComponent();
		}
		physicsObject->rigidbody = rb;
		physicsObject->dragFactor = dragFactor;
		physicsObject->forces = Acceleration();

		physicsObjects.push_back(physicsObject);
		gameObject.addComponent("physicsComponent", physicsObject);
	}
}

