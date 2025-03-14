#include "Physics.h"

namespace Physics
{
	std::vector<std::weak_ptr<PhysicsComponent>> physicsObjects;

	void initialize()
	{
		GameObjectFactory::RegisterComponentCreator("rigidbody", addRigidbody);
		GameObjectFactory::RegisterComponentCreator("physicsComponent", addPhysicsObject);
	}

	void update(float dt)
	{
		for (int i = 0; i < physicsObjects.size(); i++)
		{
			if (std::shared_ptr<PhysicsComponent> pObject = physicsObjects[i].lock())
			{
				pObject->update(dt);
			}
		}
	}

	void addRigidbody(std::shared_ptr<GameObject> gameObject, nlohmann::json& rbSection)
	{
		std::shared_ptr<Rigidbody> rb = std::static_pointer_cast<Rigidbody>(gameObject->getComponent("rigidbody"));
		if (rb == nullptr)
		{
			rb = std::make_shared<Rigidbody>(gameObject);
			gameObject->addComponent("rigidbody", rb);
		}

		float maxSpeed = rb->maxSpeed;
		float maxRotation = rb->maxRotation;
		float mass = rb->mass;
		if (rbSection["maxSpeed"].is_number())
		{
			maxSpeed = rbSection["maxSpeed"];
		}
		if (rbSection["maxRotation"].is_number())
		{
			maxRotation = rbSection["maxRotation"];
		}
		if (rbSection["mass"].is_number())
		{
			mass = rbSection["mass"];
		}
		rb->maxSpeed = maxSpeed;
		rb->maxRotation = maxRotation;
		rb->mass = mass;
	}

	void addPhysicsObject(std::shared_ptr<GameObject> gameObject, nlohmann::json& physicsSection)
	{
		std::shared_ptr<Rigidbody> rb = std::static_pointer_cast<Rigidbody>(gameObject->ensureComponent("rigidbody", 
			[gameObject]()
			{
				return std::make_shared<Rigidbody>(gameObject);
			}));

		float dragFactor = 0;
		if (physicsSection["dragFactor"].is_number())
		{
			dragFactor = physicsSection["dragFactor"];
		}

		std::shared_ptr<PhysicsComponent> physicsObject = std::static_pointer_cast<PhysicsComponent>(gameObject->getComponent("physicsComponent"));
		if (physicsObject == nullptr)
		{
			physicsObject = std::make_shared<PhysicsComponent>();
		}
		physicsObject->rigidbody = rb;
		physicsObject->dragFactor = dragFactor;
		physicsObject->forces = Acceleration();

		physicsObjects.push_back(physicsObject);
		gameObject->addComponent("physicsComponent", physicsObject);
	}
}

