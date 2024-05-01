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
#include "Matrix4.h"
#include "Vector4.h"

std::shared_ptr<GameObject> player;
std::shared_ptr<GameObject> player2;
float prevTime = 0;
float dt = 0.0167;

void matrixUnitTest()
{
	//Test matrix creation: visual check only
	Matrix4::CreateIdentity().print();
	std::cout << std::endl;
	Matrix4::CreateScale(2, 2, 2).print();
	std::cout << std::endl;
	Matrix4::CreateTranslation(1, 2, 3).print();
	std::cout << std::endl;
	Matrix4::CreateXRotation(3.14).print();
	std::cout << std::endl;
	Matrix4::CreateYRotation(3.14).print();
	std::cout << std::endl;
	Matrix4::CreateZRotation(3.14).print();
	std::cout << std::endl;

	//Test matrix multiplication
	Matrix4 rotTransComp = Matrix4::CreateTranslation(1, 2, 3) * Matrix4::CreateXRotation(3.1415 / 4);
	assert(rotTransComp.Row(2) == Vector4(0, 0.707123, 0.70709, 2));
	rotTransComp.print();
	std::cout << std::endl;

	//Test inverse and transpose
	Matrix4 tempMat = rotTransComp.GetInverse();
	assert(tempMat.Row(3) == Vector4(0, 0.70709, 0.707123, -3.53555));
	tempMat.print();
	std::cout << std::endl;
	tempMat = rotTransComp.GetInverseRotTrans();
	assert(tempMat.Row(3) == Vector4(0, 0.70709, 0.707123, -3.53555));
	tempMat.print();
	std::cout << std::endl;

	Matrix4 testingMatrix = Matrix4(
		1, 3, 3, 4,
		5, 2, 7, 8,
		9, 5, 6, 8,
		4, 7, 7, 1);
	testingMatrix.print();
	std::cout << std::endl;
	testingMatrix = testingMatrix.GetInverse();
	testingMatrix.print();
	assert(testingMatrix.Row(2) == Vector4(0.27362, -0.214724, 0.0736196, 0.0343558));
	std::cout << std::endl;
	//Answer to ^ checked using https://www.emathhelp.net/en/calculators/linear-algebra/inverse-of-matrix-calculator/?i=%5B%5B1%2C3%2C3%2C4%5D%2C%5B5%2C2%2C7%2C8%5D%2C%5B9%2C5%2C6%2C8%5D%2C%5B4%2C7%2C7%2C1%5D%5D&m=a

	rotTransComp.GetTranspose().print();
	std::cout << std::endl;

	//Test matrix-vector multiplication
	Vector4 testCoordinate = Vector4(0, 0, 0, 1);
	Vector4 testCoordinate2 = Vector4(1, 1, 1, 1);

	Vector4 temp = rotTransComp * testCoordinate;
	assert(temp == Vector4(1, 2, 3, 1));
	temp.print();
	std::cout << std::endl;
	std::cout << std::endl;
	temp = testCoordinate * rotTransComp;
	assert(temp == Vector4(0, 0, 0, 1));
	temp.print();
	std::cout << std::endl;
	std::cout << std::endl;

	temp = rotTransComp * testCoordinate2;
	assert(temp == Vector4(2, 3.41421, 3.00003, 1));
	temp.print();
	std::cout << std::endl;
	std::cout << std::endl;
	temp = testCoordinate2 * rotTransComp;
	assert(temp == Vector4(1, 3.27826e-05, 1.41421, 7));
	temp.print();
	std::cout << std::endl;
	std::cout << std::endl;
}

void collisionUnitTest()
{
	std::shared_ptr<GameObject> test1 = GameObjectFactory::CreateGameObject("data\\TestAABB1.json");
	std::shared_ptr<GameObject> test2 = GameObjectFactory::CreateGameObject("data\\TestAABB2.json");
	std::shared_ptr<GameObject> test3 = GameObjectFactory::CreateGameObject("data\\TestAABB3.json");
	std::shared_ptr<GameObject> test4 = GameObjectFactory::CreateGameObject("data\\TestAABB4.json");
	std::shared_ptr<GameObject> test5 = GameObjectFactory::CreateGameObject("data\\TestAABB5.json");
	std::shared_ptr<GameObject> test6 = GameObjectFactory::CreateGameObject("data\\TestAABB6.json");

	std::shared_ptr<Rigidbody> rb1 = std::static_pointer_cast<Rigidbody>(test1->getComponent("rigidbody"));
	std::shared_ptr<Rigidbody> rb2 = std::static_pointer_cast<Rigidbody>(test2->getComponent("rigidbody"));
	std::shared_ptr<Rigidbody> rb3 = std::static_pointer_cast<Rigidbody>(test3->getComponent("rigidbody"));
	std::shared_ptr<Rigidbody> rb4 = std::static_pointer_cast<Rigidbody>(test4->getComponent("rigidbody"));
	std::shared_ptr<Rigidbody> rb5 = std::static_pointer_cast<Rigidbody>(test5->getComponent("rigidbody"));
	std::shared_ptr<Rigidbody> rb6 = std::static_pointer_cast<Rigidbody>(test6->getComponent("rigidbody"));

	std::shared_ptr<AABB> aabb1 = std::static_pointer_cast<AABB>(test1->getComponent("AABB"));
	std::shared_ptr<AABB> aabb2 = std::static_pointer_cast<AABB>(test2->getComponent("AABB"));
	std::shared_ptr<AABB> aabb3 = std::static_pointer_cast<AABB>(test3->getComponent("AABB"));
	std::shared_ptr<AABB> aabb4 = std::static_pointer_cast<AABB>(test4->getComponent("AABB"));
	std::shared_ptr<AABB> aabb5 = std::static_pointer_cast<AABB>(test5->getComponent("AABB"));
	std::shared_ptr<AABB> aabb6 = std::static_pointer_cast<AABB>(test6->getComponent("AABB"));

	//I'll be testing with dt=1 to make it easier to design test cases
	
	rb1->velocity = Vector2(1, 0);
	rb2->velocity = Vector2(-1, 0);
	//Super simple check, both moving and they're colliding
	assert(aabb1->isColliding(1, aabb2));
	//They won't collide yet in this one
	assert(!aabb1->isColliding(1, aabb3));
	//They won't end the frame colliding, but they start by colliding so it's true
	assert(aabb1->isColliding(1, aabb4));

	rb5->velocity = Vector2(-1, -1);
	//Collide in the middle of the frame (corners hitting)
	assert(aabb5->isColliding(1, aabb6));
	//Rotate by roughly pi/4 radians or 45 degrees so that it will miss instead of tapping corners
	//This doesn't figure out until the last Y axis check that it isn't colliding. 
	test5->orientation = .785;
	assert(!aabb5->isColliding(1, aabb6));
}

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
	player2 = GameObjectFactory::CreateGameObject("data\\Player2.json");
}

void setup()
{
	startTick();
	getTick();
}

int wWinMain(HINSTANCE i_hInstance, HINSTANCE i_hPrevInstance, LPWSTR i_lpCmdLine, int i_nCmdShow)
{
	

	bool bSuccess = Renderer::initialize(i_hInstance, i_nCmdShow);
	Physics::initialize();
	Collision::initialize();

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

				player->update(dt);
				player2->update(dt);

				Physics::update(dt);
				Collision::update(dt);
				
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




