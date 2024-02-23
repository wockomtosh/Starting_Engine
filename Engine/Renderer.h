#pragma once
#include <vector>
#include <DirectXColors.h>
#include "GLib.h"
#include "GameObject.h"
#include "Renderable.h"
#include "nlohmann/json.hpp"

namespace Renderer {
	bool initialize(HINSTANCE i_hInstance, int i_CmdShow);
	void render();
	void shutdown();

	void createRenderable(GameObject& gameObject, nlohmann::json& renderSection);

	GLib::Sprite* CreateSprite(const char* i_Filename);
}