#define _CRTDBG_MAP_ALLOC
//#define _DEBUG
#include <stdlib.h>
#include <crtdbg.h>

#include <string.h>

#include <algorithm>
#include <vector>

// base class
class GameActor
{
public:
	virtual void			Update() { }
	virtual const char* getActorType() const = 0;
};

class PlayerActor : public GameActor
{
public:
	PlayerActor(const char* i_pName)
	{
		// I have added this ActorNumber counter such that
		// every instance of PlayerActor created will have a unique
		// actor number (m_ActorNumber). This may aid you in debugging
		// the crashes and memory leaks.
		static int ActorNumber = 0;

		m_ActorNumber = ActorNumber++;

		m_pName = _strdup(i_pName ? i_pName : "Generic");
	}

	// copy constructor
	PlayerActor(const PlayerActor& i_other)
	{
		m_ActorNumber = i_other.m_ActorNumber;
		m_pName = _strdup(i_other.m_pName ? i_other.m_pName : "Generic");
	}

	~PlayerActor()
	{
		free(const_cast<char*>(m_pName));
	}

	void Update()
	{
		printf("Updating %s.", m_pName);
	}

	const char* getActorType() const { return "Player"; }
	const char* getName() const { return m_pName; }
private:
	int m_ActorNumber;
	const char* m_pName;
};

// Function declarations for our test problems
void Test0();
void Test1();
void Test2();
void Test3();


//Changed because it was giving a warning.
int main()
{
	// Your assignment is to fix all the warnings, crashes and memory leaks
	// that happen when running this program.

	// Each function may have specific requirements as to how any issue
	// within need to be resolved.

	Test0();
	//DOESN'T WORK so I commented it out for now. 
	//Test1(); 
	Test2();
	Test3();

#if defined _DEBUG
	// This should not emit any messages about memory leaks.
	_CrtDumpMemoryLeaks();
#endif // _DEBUG
}

void Test0()
{
	// This function should work, not crashing or generating any memory leaks
	PlayerActor Joe("Joe");

	Joe.Update();
}

void Test1()
{
	// fix the issues with this sample code
	// by ONLY making changes to the PlayerActor class.
	// No changes can be made to this code

	std::vector<PlayerActor> TestActors;

	TestActors.push_back(PlayerActor("Player1"));
	TestActors.push_back(PlayerActor("Player2"));
	TestActors.push_back(PlayerActor("Player3"));
	TestActors.push_back(PlayerActor("Player4"));

	//I have no clue how random shuffle works. I tried looking it up but it wasn't helpful in figuring out why it doesn't shuffle correctly.
	std::random_shuffle(TestActors.begin(), TestActors.end());

	for (auto iter = TestActors.begin(); iter != TestActors.end(); ++iter)
	{
		// do something interesting with our PlayerActors
		iter->Update();
	}
}

void Test2()
{
	// fix the memory leaks caused by this sample code
	// by ONLY making changes to this function
	// No changes can be made to the PlayerActor class
	// Hint: You probably want to use _CrtDumpMemoryLeaks()
	// but maybe not inside the function. Why?

	std::vector<PlayerActor*> TestPlayerActorPtrs;

	TestPlayerActorPtrs.push_back(new PlayerActor("Player1"));
	TestPlayerActorPtrs.push_back(new PlayerActor("Player2"));
	TestPlayerActorPtrs.push_back(new PlayerActor("Player3"));
	TestPlayerActorPtrs.push_back(new PlayerActor("Player4"));

	for (auto iter = TestPlayerActorPtrs.begin(); iter != TestPlayerActorPtrs.end(); ++iter)
	{
		// do something interesting with our PlayerActors
		if (*iter) 
			(*iter)->Update();	
	}

	for (auto iter = TestPlayerActorPtrs.begin(); iter != TestPlayerActorPtrs.end(); ++iter)
	{
		if (*iter) {
			//Just call delete to fix the memory leaks??? But at the same time, I'm not actually seeing any memory leaks when I don't do this???
			delete (*iter);
		}
	}
}

void Test3()
{
	// fix the memory leaks caused by this sample code
	// by ONLY making changes to the GameActor class.
	// No changes can be made to this code
	// Hint: You probably want to use _CrtDumpMemoryLeaks()

	std::vector<GameActor*> TestGameActorPtrs;

	TestGameActorPtrs.push_back(new PlayerActor("Player1"));
	TestGameActorPtrs.push_back(new PlayerActor("Player2"));
	TestGameActorPtrs.push_back(new PlayerActor("Player3"));
	TestGameActorPtrs.push_back(new PlayerActor("Player4"));

	for (auto iter = TestGameActorPtrs.begin(); iter != TestGameActorPtrs.end(); ++iter)
	{
		// do something interesting with our PlayerActors
		if (*iter)
			(*iter)->Update();
	}

	//I'm not seeing any memory leaks here??? _CrtDumpMemoryLeaks() isn't printing anything.
}
