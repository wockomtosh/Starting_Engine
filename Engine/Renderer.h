#pragma once
#include <vector>
#include <DirectXColors.h>
#include "GLib.h"
#include "GameObject.h"
#include "RenderComponent.h"

namespace Renderer {
	bool initialize(HINSTANCE i_hInstance, int i_CmdShow);
	void drawGameObjects(std::vector<GameObject*> objects);
	void shutdown(std::vector<GameObject*> objects);
}