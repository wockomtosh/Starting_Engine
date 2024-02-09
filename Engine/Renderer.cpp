#include "Renderer.h"

bool Renderer::initialize(HINSTANCE i_hInstance, int i_CmdShow)
{
	return GLib::Initialize(i_hInstance, i_CmdShow, "Jake's Engine", -1, 1024, 768, true);
}

//TODO: Later change this to RenderComponents or something along those lines
void Renderer::drawGameObjects(std::vector<GameObject*> objects)
{
	GLib::BeginRendering(DirectX::Colors::Blue);
	GLib::Sprites::BeginRendering();

	for (int i = 0; i < objects.size(); i++)
	{
		RenderComponent* render = static_cast<RenderComponent*>(objects[i]->getComponent("RenderComponent"));
		if (render != nullptr)
		{
			render->draw();
		}
	}

	GLib::Sprites::EndRendering();
	GLib::EndRendering();
}

void Renderer::shutdown(std::vector<GameObject*> objects)
{
	//TODO: Free all sprites here?
	for (int i = 0; i < objects.size(); i++)
	{
		RenderComponent* render = static_cast<RenderComponent*>(objects[i]->getComponent("RenderComponent"));
		GLib::Release(render->sprite);
	}
	
	// IMPORTANT:  Tell GLib to shutdown, releasing resources.
	GLib::Shutdown();
}
