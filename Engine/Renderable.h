#pragma once
#include <memory>
#include "GameObject.h"
#include "GLib.h"
#include "Orientation.h"

struct Renderable {
	std::weak_ptr<GameObject> gameObject;
	GLib::Sprite* sprite;

	Renderable(std::weak_ptr<GameObject> gameObject, GLib::Sprite* sprite) :
		gameObject(gameObject), sprite(sprite) {}

	Renderable(const Renderable& other)
	{
		gameObject = other.gameObject;
		//TODO: This is not a deep copy, it's a shallow copy. So this will get destroyed.
		//Either don't use this copy constructor or create a Sprite struct that holds a sprite pointer and releases it
		//and then we do a shared pointer to that Sprite struct that will only get released when there are no refs to it.
		//Shared_ptr to a GLib::Sprite is probably cleaner but tricky because we don't do a destructor on that, we do GLib::Release.
		//We could do that if we pass the GLib::Release into shared_ptr instead of the destructor. We need to create a custom deleter for that
		sprite = other.sprite;
	}

	~Renderable()
	{
		GLib::Release(sprite);
	}

	//TODO: There's something weird with how the sprites get rotated compared to the AABBs
	//I think sprites get rotated around their base and AABBs are rotated around their center
	inline void draw() {
		if (auto object = gameObject.lock())
		{
			//GLib uses radians and also rotates counterclockwise -_-
			//It also rotates around things weirdly, idk it's just janky -_-
			GLib::Render(*sprite, { object->position.x, object->position.y }, 0.0f, -Orientation::degToRad(object->orientation));
		}
	}
};