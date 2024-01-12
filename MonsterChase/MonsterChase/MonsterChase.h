#pragma once
#include "Point2D.h"
#include "Monster.h"

class MonsterChase {

private:
	char* playerName = nullptr;
	Point2D playerPos = Point2D();
	int numMonsters = 0;
	int totalMonsters = 0;
	Monster* monsters = nullptr;

	template <typename T>
	T* appendToArray(T newValue, T* i_array, int i_arrayLength);
	template <typename T>
	T* removeFromArray(int removeIndex, T* i_array, int i_arrayLength);
	void initMonsters();
	void startGame();
	void removeMonster(int index);
	void addMonster();
	void addRemoveMonsters();
	void moveMonsters();
	void displayMonsters();
	void displayPlayer();
	void queryPlayer();
	void mainLoop();
	void Point2DUnitTest();

public:
	void run();
};