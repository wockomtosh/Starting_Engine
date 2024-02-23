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

#include "GLib.h"
#include "GameObject.h"
#include "Physics.h"
#include "Renderer.h"
#include "PlayerController.h"

std::shared_ptr<GameObject> player;
float prevTime = 0;
float dt = 0.0167;

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

void setupPlayer()
{
	player = GameObjectFactory::CreateGameObject("data\\Player.json");
}

void setup()
{
	startTick();
}

int wWinMain(HINSTANCE i_hInstance, HINSTANCE i_hPrevInstance, LPWSTR i_lpCmdLine, int i_nCmdShow)
{
	bool bSuccess = Renderer::initialize(i_hInstance, i_nCmdShow);
	Physics::initialize();

	GameObjectFactory::RegisterControllerCreator("player", &PlayerController::createPlayerController);

	setupPlayer();

	if (bSuccess)
	{
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

				Physics::update(dt);
				
				Renderer::render();
			}
		} while (bQuit == false);

		//shutdown and free sprites
		Renderer::shutdown();
	}

	_CrtDumpMemoryLeaks();
#if defined _DEBUG
	_CrtDumpMemoryLeaks();
#endif // _DEBUG
}




