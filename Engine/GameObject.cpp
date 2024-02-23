#include "GameObject.h"

GameObject::GameObject() :
	name(""), position(Vector2()), orientation(0), components({ {"", nullptr} }), controller(nullptr)
{
}

GameObject::GameObject(Vector2 startingPosition) :
	name(""), position(startingPosition), orientation(0), components({ {"", nullptr} }), controller(nullptr)
{
}

GameObject::GameObject(Vector2 startingPosition, std::map<std::string, void*> components) :
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

void* GameObject::getComponent(const std::string componentName)
{
	if (!components.empty()) 
	{
		auto componentIter = components.find(componentName);
		if (componentIter == components.end())
		{
			return nullptr;
		}
		return componentIter->second;
	}
	
}

void GameObject::addComponent(const std::string componentName, void* component)
{
	components.insert({ componentName.c_str(), component });
}

void* GameObject::ensureComponent(const char* componentName, std::function<void* (void)> componentCreator)
{
	auto componentIter = components.find(componentName);
	if (componentIter == components.end())
	{
		void* newComponent = componentCreator();
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
	//TODO: delete components!!!
}
