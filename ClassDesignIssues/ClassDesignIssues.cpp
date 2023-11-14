#define _CRTDBG_MAP_ALLOC
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
		m_pName = _strdup(i_pName ? i_pName : "Generic");
	}

	// copy constructor
	PlayerActor(const PlayerActor& i_other)
	{
		m_pName = _strdup(i_other.m_pName ? i_other.m_pName : "Generic");
	}

	~PlayerActor()
	{
		delete(m_pName);
	}

	const char* getActorType() const { return "Player"; }
	const char* getName() const { return m_pName; }
	void Update() {}
private:
	const char* m_pName;
};

void Test1();
void Test2();

void main(int i_argc, char** i_argl)
{
	Test1();
	Test2();

#if defined _DEBUG
	_CrtDumpMemoryLeaks();
#endif // _DEBUG
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

	std::random_shuffle(TestActors.begin(), TestActors.end());

	// pretend we call PlayGame() here.
}

void Test2()
{
	// fix the memory leaks caused by this sample code
	// by ONLY making changes to the GameActor class.
	// No changes can be made to this code

	std::vector<GameActor*> TestActorPtrs;

	TestActorPtrs.push_back(new PlayerActor("Player1"));
	TestActorPtrs.push_back(new PlayerActor("Player2"));
	TestActorPtrs.push_back(new PlayerActor("Player3"));
	TestActorPtrs.push_back(new PlayerActor("Player4"));

	// pretend we call PlayGame() here.

	// clean up
	for (auto iter = TestActorPtrs.begin(); iter != TestActorPtrs.end(); ++iter)
	{
		if (*iter)
			delete* iter;
	}
}
