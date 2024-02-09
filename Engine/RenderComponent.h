#pragma once
#include "GameObject.h"
#include "GLib.h"


struct RenderComponent {
	GameObject* gameObject;
	GLib::Sprite* sprite;

	RenderComponent(GameObject* gameObject, GLib::Sprite* sprite) :
		gameObject(gameObject), sprite(sprite) {}

	~RenderComponent()
	{
		GLib::Release(sprite);
	}

	inline void draw() {
		//TODO: convert orientation to radians and figure out why passing player orientation breaks it

		GLib::Render(*sprite, { gameObject->position.x, gameObject->position.y }, 0.0f, gameObject->orientation);
	}
};