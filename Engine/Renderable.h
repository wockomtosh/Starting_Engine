#pragma once
#include <memory>
#include "GameObject.h"
#include "GLib.h"

struct Renderable {
	std::weak_ptr<GameObject> gameObject;
	GLib::Sprite* sprite;

	Renderable(std::weak_ptr<GameObject> gameObject, GLib::Sprite* sprite) :
		gameObject(gameObject), sprite(sprite) {}

	Renderable(const Renderable& other)
	{
		gameObject = other.gameObject;
		sprite = other.sprite;
	}

	~Renderable()
	{
		GLib::Release(sprite);
	}

	inline void draw() {
		if (auto object = gameObject.lock())
		{
			GLib::Render(*sprite, { object->position.x, object->position.y }, 0.0f, object->orientation);
		}
	}
};