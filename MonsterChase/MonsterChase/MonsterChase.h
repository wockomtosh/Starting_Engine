#pragma once
#include "Vector2.h"
#include "Monster.h"
#include "GLib.h"

class MonsterChase {

private:
	const char* playerName = nullptr;
	Vector2 playerPos = Vector2();
	int numMonsters = 0;
	int totalMonsters = 0;
	Monster* monsters = nullptr;

	template <typename T>
	T* appendToArray(T newValue, T* i_array, int i_arrayLength);
	template <typename T>
	T* removeFromArray(int removeIndex, T* i_array, int i_arrayLength);
	void initMonsters();
	void removeMonster(int index);
	void addMonster();

public:
	void Point2DUnitTest();
	void startGame();
	void addRemoveMonsters();
	void moveMonsters();
	void displayMonsters();
	void displayPlayer();
	void queryPlayer();
	void RenderMonsters(GLib::Sprite* monsterSprite);
};