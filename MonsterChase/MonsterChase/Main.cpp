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
#include "Collision.h"
#include "PlayerController.h"
#include "BallController.h"
#include "Matrix4.h"
#include "Vector4.h"

std::shared_ptr<GameObject> player;
std::shared_ptr<GameObject> player2;
std::shared_ptr<GameObject> ball;
std::shared_ptr<GameObject> topBarrier;
std::shared_ptr<GameObject> bottomBarrier;
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

void setupObjects()
{
	player = GameObjectFactory::CreateGameObject("data\\Player.json");
	player2 = GameObjectFactory::CreateGameObject("data\\Player2.json");
	ball = GameObjectFactory::CreateGameObject("data\\Ball.json");
	topBarrier = GameObjectFactory::CreateGameObject("data\\TopBarrier.json");
	bottomBarrier = GameObjectFactory::CreateGameObject("data\\BottomBarrier.json");
}

void setup()
{
	startTick();
	getTick();
}

void handleKeyPress(unsigned int i_VKeyID, bool bWentDown)
{
	//Check player 1
	if (auto pPhysics = std::static_pointer_cast<PhysicsComponent>(player->getComponent("physicsComponent")))
	{
		unsigned int w = 0x57;
		unsigned int s = 0x53;

		if (i_VKeyID == w)
		{
			if (bWentDown)
			{
				pPhysics->forces.linear += Vector2::Up * 1000;
			}
			else {
				pPhysics->forces.linear -= Vector2::Up * 1000;
			}
		}
		if (i_VKeyID == s)
		{
			if (bWentDown)
			{
				pPhysics->forces.linear += Vector2::Down * 1000;
			}
			else {
				pPhysics->forces.linear -= Vector2::Down * 1000;
			}
		}
	}
	//Check player 2
	if (auto pPhysics = std::static_pointer_cast<PhysicsComponent>(player2->getComponent("physicsComponent")))
	{
		unsigned int i = 0x49;
		unsigned int k = 0x4B;

		if (i_VKeyID == i)
		{
			if (bWentDown)
			{
				pPhysics->forces.linear += Vector2::Up * 1000;
			}
			else {
				pPhysics->forces.linear -= Vector2::Up * 1000;
			}
		}
		if (i_VKeyID == k)
		{
			if (bWentDown)
			{
				pPhysics->forces.linear += Vector2::Down * 1000;
			}
			else {
				pPhysics->forces.linear -= Vector2::Down * 1000;
			}
		}
	}
}

int wWinMain(HINSTANCE i_hInstance, HINSTANCE i_hPrevInstance, LPWSTR i_lpCmdLine, int i_nCmdShow)
{
	bool bSuccess = Renderer::initialize(i_hInstance, i_nCmdShow);
	Physics::initialize();
	Collision::initialize();

	GameObjectFactory::RegisterControllerCreator("player", &PlayerController::createPlayerController);
	GameObjectFactory::RegisterControllerCreator("ball", &BallController::createBallController);

	setupObjects();

	// IMPORTANT (if we want keypress info from GLib): Set a callback for notification of key presses
	//TODO: Understand std::bind better. 
	GLib::SetKeyStateChangeCallback(std::bind(&handleKeyPress, std::placeholders::_1, std::placeholders::_2));

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

				player->update(dt);
				player2->update(dt);

				if (ball->position.x > 512 || ball->position.x < -512)
				{
					ball.reset();
					ball = GameObjectFactory::CreateGameObject("data\\Ball.json");
				}

				//Because collisions need to work closely with physics, Physics::update will happen inside of Collision::update
				Collision::update(dt);
				Renderer::render();
			}
		} while (bQuit == false);

		//shutdown and free sprites
		Renderer::shutdown();
	}

	//TODO: Use the profiler to fix memory leaks.
	//I was mixed up on how the output of this shows up and thought I'd been doing fine, but there's several
	_CrtDumpMemoryLeaks();
#if defined _DEBUG
	_CrtDumpMemoryLeaks();
#endif // _DEBUG
}




