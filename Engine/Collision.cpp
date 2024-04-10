#include "Collision.h"
#include <iostream>

namespace Collision
{
	std::vector<std::weak_ptr<AABB>> boundingBoxes;

	void initialize()
	{
		GameObjectFactory::RegisterComponentCreator("AABB", addAABB);
	}

	void update(float dt)
	{
		for (int i = 0; i < boundingBoxes.size(); i++)
		{
			if (std::shared_ptr<AABB> bb = boundingBoxes[i].lock())
			{
				//Check against all the other bounding boxes (ex: if you start at 0, check everything from 1 onwards)
				for (int j = i + 1; j < boundingBoxes.size(); j++)
				{
					if (auto other = boundingBoxes[j].lock())
					{
						if (bb->isColliding(dt, other))
						{
							//Respond to the collision
							std::cout << "COLLIDED" << std::endl;
						}
					}
				}
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
	}
}

