#include "Renderer.h"
#include "GameObjectFactory.h"
#include "LoadFileToBuffer.h"
#include <iostream>

namespace Renderer 
{
	std::vector<std::weak_ptr<Renderable>> renderables;

	bool initialize(HINSTANCE i_hInstance, int i_CmdShow)
	{
		GameObjectFactory::RegisterComponentCreator("renderable", createRenderable);
		return GLib::Initialize(i_hInstance, i_CmdShow, "Jake's Engine", -1, 1024, 768, true);
	}

	void render()
	{
		GLib::BeginRendering(DirectX::Colors::Blue);
		GLib::Sprites::BeginRendering();

		for (int i = 0; i < renderables.size(); i++)
		{
			//renderables[i]->draw();
			if (std::shared_ptr<Renderable> renderable = renderables[i].lock())
			{
				std::cout << renderable->sprite << std::endl;
				renderable->draw();
			}
		}

		GLib::Sprites::EndRendering();
		GLib::EndRendering();
	}

	void shutdown()
	{
		// IMPORTANT:  Tell GLib to shutdown, releasing resources.
		GLib::Shutdown();
	}

	void createRenderable(std::shared_ptr<GameObject> gameObject, nlohmann::json& renderSection)
	{
		assert(renderSection["sprite_file"].is_string());

		std::string spriteFile = renderSection["sprite_file"];
		GLib::Sprite* sprite = CreateSprite(spriteFile.c_str());

		if (sprite)
		{
			//Nothing depends on renderables so we don't need to worry about checking if one exists here.
			std::shared_ptr<Renderable> newRenderable = std::make_shared<Renderable>(gameObject, sprite);
			Renderer::renderables.push_back(newRenderable);
			gameObject->addComponent("renderable", newRenderable);
		}
	}


	GLib::Sprite* CreateSprite(const char* i_Filename)
	{
		assert(i_Filename);

		size_t sizeTextureFile = 0;

		// Load the source file (texture data)
		std::vector<uint8_t> TextureData = LoadFileToBuffer(i_Filename);

		GLib::Sprite* pSprite = nullptr;

		// Ask GLib to create a texture out of the data (assuming it was loaded successfully)
		if (!TextureData.empty())
		{
			GLib::Texture* pTexture = GLib::CreateTexture(TextureData.data(), TextureData.size());

			unsigned int width = 0;
			unsigned int height = 0;
			unsigned int depth = 0;

			// Get the dimensions of the texture. We'll use this to determine how big it is on screen
			bool result = GLib::GetDimensions(*pTexture, width, height, depth);
			assert(result == true);
			assert((width > 0) && (height > 0));

			// Define the sprite edges
			GLib::SpriteEdges	Edges = { -float(width / 2.0f), float(height), float(width / 2.0f), 0.0f };
			GLib::SpriteUVs	UVs = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f } };
			GLib::RGBA							Color = { 255, 255, 255, 255 };

			// Create the sprite
			pSprite = GLib::CreateSprite(Edges, 0.1f, Color, UVs, pTexture);

			// release our reference on the Texture
			GLib::Release(pTexture);
		}

		return pSprite;
	}
}

