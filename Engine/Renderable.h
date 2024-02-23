#pragma once
#include "GameObject.h"
#include "GLib.h"

struct Renderable {
	GameObject* gameObject;
	GLib::Sprite* sprite;

	Renderable(GameObject* gameObject, GLib::Sprite* sprite) :
		gameObject(gameObject), sprite(sprite) {}

	~Renderable()
	{
		GLib::Release(sprite);
	}

	inline void draw() {
		GLib::Render(*sprite, { gameObject->position.x, gameObject->position.y }, 0.0f, gameObject->orientation);
	}
};