#include <iostream>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <Windows.h>
#include <vector>

#include <DirectXColors.h>

#if defined _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif // _DEBUG

#include "MonsterChase.h"
#include "GLib.h"

#include "GameObject.h"
#include "PhysicsSystem.h"
#include "Renderer.h"

std::vector<GameObject*> gameObjects;

MonsterChase* game;
GameObject* player;
PhysicsSystem* physicsSystem;
float prevTime = 0;
float dt = 0;


void* LoadFile(const char* i_pFilename, size_t& o_sizeFile);
GLib::Sprite* CreateSprite(const char* i_pFilename);

void startTick()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&prevTime);
}

void getTick()
{
	uint64_t countsPerSecond = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSecond);

	uint64_t curTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&curTime);

	//Divide the number of counts that have passed by counts per second to get seconds
	dt = (curTime - prevTime) / (float)countsPerSecond;

	prevTime = static_cast<float>(curTime);
}

void KeyCallback(unsigned int i_VKeyID, bool bWentDown)
{
	unsigned int w = 0x57;
	unsigned int a = 0x41;
	unsigned int s = 0x53;
	unsigned int d = 0x44;

	if (i_VKeyID == w)
	{
		if (bWentDown)
		{
			static_cast<PhysicsComponent*>(player->getComponent("PhysicsComponent"))->forces = { Vector2::Up * 100, 0 };
		}
		else {
			static_cast<PhysicsComponent*>(player->getComponent("PhysicsComponent"))->forces = { Vector2(), 0 };
		}
	}
	if (i_VKeyID == a)
	{
		if (bWentDown)
		{
			static_cast<PhysicsComponent*>(player->getComponent("PhysicsComponent"))->forces = { Vector2::Left * 100, 0 };
		}
		else {
			static_cast<PhysicsComponent*>(player->getComponent("PhysicsComponent"))->forces = { Vector2(), 0 };
		}
	}
	if (i_VKeyID == s)
	{
		if (bWentDown)
		{
			static_cast<PhysicsComponent*>(player->getComponent("PhysicsComponent"))->forces = { Vector2::Down * 100, 0 };
		}
		else {
			static_cast<PhysicsComponent*>(player->getComponent("PhysicsComponent"))->forces = { Vector2(), 0 };
		}
	}
	if (i_VKeyID == d)
	{
		if (bWentDown)
		{
			static_cast<PhysicsComponent*>(player->getComponent("PhysicsComponent"))->forces = { Vector2::Right * 100, 0 };
		}
		else {
			static_cast<PhysicsComponent*>(player->getComponent("PhysicsComponent"))->forces = { Vector2(), 0 };
		}
	}
#ifdef _DEBUG
	const size_t	lenBuffer = 129;
	char			Buffer[lenBuffer];

	sprintf_s(Buffer, lenBuffer, "VKey 0x%04x went %s\n", i_VKeyID, bWentDown ? "down" : "up");
	OutputDebugStringA(Buffer);
#endif // __DEBUG
}

void setupPlayer()
{
	//Maybe this could be used for any object? Just swap out components as needed?
	//This should maybe be inside the GameObject constructor because it's absolutely required for all GameObjects?

	Rigidbody* playerBody = new Rigidbody();
	PhysicsComponent* playerPhysics = new PhysicsComponent(playerBody, 50, { Vector2(), 0 });

	std::map<std::string, void*> components = { {"Rigidbody", playerBody},
												{"PhysicsComponent", playerPhysics} };

	player = new GameObject(Vector2(), components);
	player->orientation = 0;

	playerBody->gameObject = player;

	GLib::Sprite* pGoodGuy = CreateSprite("sprites\\GoodGuy.dds");
	RenderComponent* playerRender = new RenderComponent(player, pGoodGuy);
	player->addComponent("RenderComponent", playerRender);

	gameObjects.push_back(player);
}

void setupPhysics()
{
	//Right now I'm just having the player do physics, I'll need to figure out how to set up the rest later
	std::vector<PhysicsComponent*> physicsObjects = std::vector<PhysicsComponent*>();
	//TODO Here I would normally iterate through all my game objects and find whichever ones have this and add them
	physicsObjects.push_back(static_cast<PhysicsComponent*>(player->getComponent("PhysicsComponent")));
	physicsSystem = new PhysicsSystem(physicsObjects);
}

void setup()
{
	gameObjects = std::vector<GameObject*>();

	setupPlayer();

	setupPhysics();

	startTick();
}

int wWinMain(HINSTANCE i_hInstance, HINSTANCE i_hPrevInstance, LPWSTR i_lpCmdLine, int i_nCmdShow)
{
	bool bSuccess = Renderer::initialize(i_hInstance, i_nCmdShow);

	if (bSuccess)
	{
		// IMPORTANT (if we want keypress info from GLib): Set a callback for notification of key presses
		GLib::SetKeyStateChangeCallback(KeyCallback);

		setup();

		bool bQuit = false;

		//Main Loop
		do
		{
			// IMPORTANT: We need to let GLib do it's thing. 
			GLib::Service(bQuit);

			if (!bQuit)
			{
				getTick();

				physicsSystem->update(dt);
				
				Renderer::drawGameObjects(gameObjects);
			}
		} while (bQuit == false);

		//shutdown and free sprites
		Renderer::shutdown(gameObjects);
	}

	_CrtDumpMemoryLeaks();
#if defined _DEBUG
	_CrtDumpMemoryLeaks();
#endif // _DEBUG
}

GLib::Sprite* CreateSprite(const char* i_pFilename)
{
	assert(i_pFilename);

	size_t sizeTextureFile = 0;

	// Load the source file (texture data)
	void* pTextureFile = LoadFile(i_pFilename, sizeTextureFile);

	// Ask GLib to create a texture out of the data (assuming it was loaded successfully)
	GLib::Texture* pTexture = pTextureFile ? GLib::CreateTexture(pTextureFile, sizeTextureFile) : nullptr;

	// exit if something didn't work
	// probably need some debug logging in here!!!!
	if (pTextureFile)
		delete[] pTextureFile;

	if (pTexture == nullptr)
		return nullptr;

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
	GLib::Sprite* pSprite = GLib::CreateSprite(Edges, 0.1f, Color, UVs, pTexture);

	// release our reference on the Texture
	GLib::Release(pTexture);

	return pSprite;
}

void* LoadFile(const char* i_pFilename, size_t& o_sizeFile)
{
	assert(i_pFilename != NULL);

	FILE* pFile = NULL;

	errno_t fopenError = fopen_s(&pFile, i_pFilename, "rb");
	if (fopenError != 0)
		return NULL;

	assert(pFile != NULL);

	int FileIOError = fseek(pFile, 0, SEEK_END);
	assert(FileIOError == 0);

	long FileSize = ftell(pFile);
	assert(FileSize >= 0);

	FileIOError = fseek(pFile, 0, SEEK_SET);
	assert(FileIOError == 0);

	uint8_t* pBuffer = new uint8_t[FileSize];
	assert(pBuffer);

	size_t FileRead = fread(pBuffer, 1, FileSize, pFile);
	assert(FileRead == FileSize);

	fclose(pFile);

	o_sizeFile = FileSize;

	return pBuffer;
}
