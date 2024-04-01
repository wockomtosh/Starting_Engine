#pragma once

#include "GameObject.h"
#include "nlohmann/json.hpp"
#include <functional>
#include <memory>

namespace GameObjectFactory
{
	std::shared_ptr<GameObject> CreateGameObject(const char* i_JSONConfig);

	std::shared_ptr<GameObject> CreateGameObject(const char* i_jsonFile);

	void RegisterControllerCreator(const std::string& i_ControllerName, std::function<void(std::shared_ptr<GameObject>, nlohmann::json&)> i_ControllerCreator);
	void RegisterComponentCreator(const std::string& i_ComponentName, std::function<void(std::shared_ptr<GameObject>, nlohmann::json&)> i_ComponentCreator);
}