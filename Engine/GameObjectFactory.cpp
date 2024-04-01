#include "GameObjectFactory.h"
#include <fstream>

using json = nlohmann::json;

namespace GameObjectFactory
{
	std::map<std::string, std::function<void(std::shared_ptr<GameObject>, nlohmann::json&)>> controllerCreators;
	std::map<std::string, std::function<void(std::shared_ptr<GameObject>, nlohmann::json&)>> componentCreators;

	std::shared_ptr<GameObject> createBaseGameObject(json jsonData)
	{
		std::shared_ptr<GameObject> newObject = std::make_shared<GameObject>();

		if (jsonData["name"].is_string())
		{
			newObject->name = jsonData["name"].get<std::string>();
		}
		if (jsonData["position"].is_array())
		{
			newObject->position = Vector2(jsonData["position"][0], jsonData["position"][1]);
		}
		if (jsonData["orientation"].is_number())
		{
			newObject->orientation = jsonData["orientation"];
		}

		return newObject;
	}

	void createComponents(std::shared_ptr<GameObject> newObject, json jsonData)
	{
		if (jsonData.contains("components"))
		{
			for (json::iterator iter = jsonData["components"].begin(); iter != jsonData["components"].end(); ++iter)
			{
				std::string componentName = iter.key();

				auto componentCreator = componentCreators.find(componentName);
				if (componentCreator != componentCreators.end())
				{
					componentCreator->second(newObject, iter.value());
				}
			}
		}
	}

	void createControllers(std::shared_ptr<GameObject> newObject, json jsonData)
	{
		if (jsonData.contains("controller"))
		{
			if (jsonData["controller"].contains("type"))
			{
				auto controllerCreator = controllerCreators.find(jsonData["controller"]["type"]);
				if (controllerCreator != controllerCreators.end())
				{
					controllerCreator->second(newObject, jsonData["controller"]["initializer"]);
				}
			}
		}
	}

	std::shared_ptr<GameObject> CreateGameObject(const char* i_jsonFile)
	{
		std::ifstream jsonFile(i_jsonFile); //It sounds like using C functions like fopen_s and fread are a bit faster, but this will do for now.
		json jsonData = json::parse(jsonFile);
		//TODO: Error handling for incorrect file names or missing files

		if (!jsonData.empty())
		{
			std::shared_ptr<GameObject> newObject = createBaseGameObject(jsonData);

			createComponents(newObject, jsonData);
			createControllers(newObject, jsonData);
			
			return newObject;
		}
		return nullptr;
	}

	void RegisterControllerCreator(const std::string& i_ControllerName, std::function<void(std::shared_ptr<GameObject>, nlohmann::json&)> i_ControllerCreator)
	{
		controllerCreators.insert({ i_ControllerName, i_ControllerCreator });
	}

	void RegisterComponentCreator(const std::string& i_ComponentName, std::function<void(std::shared_ptr<GameObject>, nlohmann::json&)> i_ComponentCreator)
	{
		componentCreators.insert({ i_ComponentName, i_ComponentCreator });
	}
}


