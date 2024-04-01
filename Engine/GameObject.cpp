#include "GameObject.h"

GameObject::GameObject() :
	name(""), position(Vector2()), orientation(0), components({ {"", nullptr} }), controller(nullptr)
{
}

GameObject::GameObject(Vector2 startingPosition) :
	name(""), position(startingPosition), orientation(0), components({ {"", nullptr} }), controller(nullptr)
{
}

GameObject::GameObject(Vector2 startingPosition, std::map<std::string, std::shared_ptr<void>> components) :
	name(""), position(startingPosition), orientation(0), components(components), controller(nullptr)
{
}

GameObject::GameObject(Vector2 startingPosition, float startingOrientation) :
	name(""), position(startingPosition), orientation(startingOrientation), components({ {"", nullptr} }), controller(nullptr)
{
}

GameObject::GameObject(const GameObject& oldObject)
{
	name = oldObject.name;
	position = oldObject.position;
	orientation = oldObject.orientation;
	components = oldObject.components;
}

std::shared_ptr<void> GameObject::getComponent(const std::string componentName)
{
	if (!components.empty()) 
	{
		auto componentIter = components.find(componentName);
		if (componentIter == components.end())
		{
			return std::shared_ptr<void>();
		}
		return componentIter->second;
	}
	
}

void GameObject::addComponent(const std::string componentName, std::shared_ptr<void> component)
{
	components.insert({ componentName.c_str(), component });
}

std::shared_ptr<void> GameObject::ensureComponent(std::string componentName, std::function<std::shared_ptr<void> (void)> componentCreator)
{
	auto componentIter = components.find(componentName);
	if (componentIter == components.end())
	{
		std::shared_ptr<void> newComponent = componentCreator();
		components.insert({ componentName, newComponent });
		return newComponent;
	}
	return componentIter->second;
}

void GameObject::print()
{
	position.print();
}

GameObject::~GameObject()
{
	//Delete the components
	for (auto iter = components.begin(); iter != components.end(); iter++)
	{
		iter->second.reset();
	}
}
