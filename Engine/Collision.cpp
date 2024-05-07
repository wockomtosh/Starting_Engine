#include "Collision.h"
#include "Physics.h"
#include <iostream>

namespace Collision
{
	std::vector<std::weak_ptr<AABB>> boundingBoxes;

	void initialize()
	{
		GameObjectFactory::RegisterComponentCreator("AABB", addAABB);
	}

	void resolveElasticCollision(std::shared_ptr<AABB> obj1, std::shared_ptr<AABB> obj2, CollisionData collision)
	{
		//If both are elastic then we need to do mass/conservation of momentum stuff
		std::shared_ptr<Rigidbody> rb1 = obj1->body.lock();
		std::shared_ptr<Rigidbody> rb2 = obj2->body.lock();

		Vector2 newV1 = rb1->velocity * ((rb1->mass - rb2->mass) / (rb1->mass + rb2->mass));
		newV1 += rb2->velocity * ((2 * rb2->mass) / (rb1->mass + rb2->mass));

		Vector2 newV2 = rb2->velocity * ((rb2->mass - rb1->mass) / (rb1->mass + rb2->mass));
		newV2 += rb1->velocity * ((2 * rb1->mass) / (rb1->mass + rb2->mass));

		rb1->velocity = newV1;
		rb2->velocity = newV2;
		
		//I also need to roll them back a bit so that they aren't actively colliding
		//We can move them back along the collisionNormal
		std::shared_ptr<GameObject> go1 = obj1->gameObject.lock();
		std::shared_ptr<GameObject> go2 = obj2->gameObject.lock();

		//First determine the correct direction of the normal
		//We do this by getting the vector from one object to the other, then take the dot product. 
		//If the dot product is negative they point in opposite directions (which means the normal is correct)
		//So we negate the collision normal if the dot product is positive
		Vector4 obj1to2Vector = Vector4(go2->position - go1->position, 0, 0);
		Vector4 collisionNormal = collision.collisionNormal;
		if (obj1to2Vector.dot(collisionNormal) > 0)
		{
			collisionNormal *= -1;
		}

		go1->position += .1 * Vector2(collisionNormal.x, collisionNormal.y);
		//It's just opposite for the other normal, so subtract instead
		go2->position -= .1 * Vector2(collisionNormal.x, collisionNormal.y);
	}

	void resolveBlockingCollision(std::shared_ptr<AABB> obj1, std::shared_ptr<AABB> obj2, CollisionData collision)
	{
		//If they're both blocking then we don't change their velocity, but we can't let them overlap
		//Push them back along the collision normal
		//The problem is that my idea of blocking is that blocking objects shouldn't bounce away from each other, they should just stop
		//TODO: find a way to roll the objects back along the collision normal just enough to not collide. For now I'm multiplying by .1
		//TODO: One blocking object will push the other if the other isn't pushing back. Do I want that? I think I might need more types of collision responses depending on the game...
		std::shared_ptr<GameObject> go1 = obj1->gameObject.lock();
		std::shared_ptr<GameObject> go2 = obj2->gameObject.lock();

		Vector4 obj1to2Vector = Vector4(go2->position - go1->position, 0, 0);
		Vector4 collisionNormal = collision.collisionNormal;
		if (obj1to2Vector.dot(collisionNormal) > 0)
		{
			collisionNormal *= -1;
		}

		go1->position += .1 * Vector2(collisionNormal.x, collisionNormal.y);
		go2->position -= .1 * Vector2(collisionNormal.x, collisionNormal.y);
	}

	void resolveElasticBlockingCollision(std::shared_ptr<AABB> elastic, std::shared_ptr<AABB> blocking, CollisionData collision)
	{
		//If one is blocking and the other is elastic then we need to reflect off of the blocking one
		//Don't change the velocity or position of the blocking object, just the elastic.

		std::shared_ptr<Rigidbody> elasticBody = elastic->body.lock();

		Vector4 elasticVel = Vector4(elasticBody->velocity);
		Vector4 newVel = elasticVel - (collision.collisionNormal * (2 * (elasticVel.dot(collision.collisionNormal))));

		elasticBody->velocity = Vector2(newVel.x, newVel.y);

		std::shared_ptr<GameObject> elasticGO = elastic->gameObject.lock();
		std::shared_ptr<GameObject> blockingGO = blocking->gameObject.lock();

		Vector4 elasticToBlockingVector = Vector4(blockingGO->position - elasticGO->position, 0, 0);
		Vector4 collisionNormal = collision.collisionNormal;
		if (elasticToBlockingVector.dot(collisionNormal) > 0)
		{
			collisionNormal *= -1;
		}
		elasticGO->position += .1 * Vector2(collisionNormal.x, collisionNormal.y);
	}

	//I wanted to implement overlap style collisions. I think that this would be tricky under this setup. 
	//Maybe AABBs can have an overlap callback function, where if collision time is > 0 and it's an overlap we do the callback then
	//But if it's == 0 then we ignore it, so it only happens on the initial overlap
	//Could this happen here or does it need to be a combo of here and the AABB?
	//I think this is out of scope for now, I just need to move on.
	void resolveCollision(CollisionData collision)
	{
		std::shared_ptr<AABB> obj1 = collision.collidedObjects[0].lock();
		std::shared_ptr<AABB> obj2 = collision.collidedObjects[1].lock();

		if (obj1->type == CollisionType::blocking)
		{
			if (obj2->type == CollisionType::blocking)
			{
				resolveBlockingCollision(obj1, obj2, collision);
			}
			if (obj2->type == CollisionType::elastic)
			{
				resolveElasticBlockingCollision(obj2, obj1, collision);

			}
		}
		if (obj1->type == CollisionType::elastic)
		{
			if (obj2->type == CollisionType::blocking)
			{
				resolveElasticBlockingCollision(obj1, obj2, collision);

			}
			if (obj2->type == CollisionType::elastic)
			{
				resolveElasticCollision(obj1, obj2, collision);
			}
		}
	}

	bool findEarliestCollision(float dt, CollisionData& o_collisionData)
	{
		o_collisionData.collisionTime = dt;

		bool foundCollision = false;

		for (int i = 0; i < boundingBoxes.size(); i++)
		{
			if (std::shared_ptr<AABB> bb = boundingBoxes[i].lock())
			{
				//Check against all the other bounding boxes (ex: if you start at 0, check everything from 1 onwards)
				for (int j = i + 1; j < boundingBoxes.size(); j++)
				{
					if (auto other = boundingBoxes[j].lock())
					{
						float collisionTime = 10;
						Vector4 collisionNormal = Vector4(1, 0, 0, 0); //For now default to x direction
						if (bb->isColliding(dt, other, collisionTime, collisionNormal))
						{
							if (collisionTime < o_collisionData.collisionTime)
							{
								o_collisionData.collisionTime = collisionTime;
								o_collisionData.collisionNormal = collisionNormal;
								o_collisionData.collidedObjects[0] = boundingBoxes[i];
								o_collisionData.collidedObjects[1] = boundingBoxes[j];

								foundCollision = true;
							}
						}
					}
				}
			}
		}

		return foundCollision;
	}

	//If our world is sparse (not lots of collisions) then this is overkill
	//To optimize this we can generate a list of collision pairs when we find collisions, update the physics, and resolve all those collision pairs
	void update(float dt)
	{
		//If we find a collision, advance the frame up to that point, resolve, decrease dt, and repeat
		//If no collisions then we just advance
		while (dt > 0)
		{
			CollisionData earliestCollision;
			if (findEarliestCollision(dt, earliestCollision))
			{
				Physics::update(earliestCollision.collisionTime);
				resolveCollision(earliestCollision);

				dt -= earliestCollision.collisionTime;
			}
			else
			{
				Physics::update(dt);
				dt = 0;
			}
		}
	}

	void addAABB(std::shared_ptr<GameObject> gameObject, nlohmann::json& bbSection)
	{
		//I don't think I need to check if one already exists here but I'm going to anyways
		std::shared_ptr<AABB> bb = std::static_pointer_cast<AABB>(gameObject->getComponent("AABB"));
		if (bb == nullptr)
		{
			bb = std::make_shared<AABB>(gameObject);

			//We only need to add the component if it's not already there.
			gameObject->addComponent("AABB", bb);

			//Should this be done here or outside this if? It's hard to say and it may need its own check.
			//For now this shouldn't matter.
			boundingBoxes.push_back(bb);
		}

		//Maybe other physics stuff too? Not exactly sure how I want to handle this.
		//TODO: It would probably be cleaner to use the RB constructor for the AABB, so we'd move this to the top
		//and pass it into our constructor? But we also may want to set this afterwards anyways... Idk but it doesn't matter too much.
		std::shared_ptr<Rigidbody> rb = std::static_pointer_cast<Rigidbody>(gameObject->ensureComponent("rigidbody",
			[gameObject]()
			{
				return std::make_shared<Rigidbody>(gameObject);
			}));
		bb->body = rb;
		
		if (bbSection["center"].is_array())
		{
			bb->center = Vector2(bbSection["center"][0], bbSection["center"][1]);
		}
		if (bbSection["extents"].is_array())
		{
			bb->extents = Vector2(bbSection["extents"][0], bbSection["extents"][1]);
		}
		if (bbSection["type"].is_string())
		{
			std::string type = bbSection["type"];
			if (type == "elastic")
			{
				bb->type = CollisionType::elastic;
			}
			else if (type == "blocking")
			{
				bb->type = CollisionType::blocking;
			}
			else 
			{
				bb->type = CollisionType::blocking;
			}
		}
	}
}

