#pragma once
#include "GameObject.h"

class Monster : public GameObject
{
	int lifeSpan;
	int duration;
	int number;

public:
	Monster(const char* name, int number);

	int getLifeSpan();
	void setLifeSpan(int newLifeSpan);
	int getDuration();
	void setDuration(int newDuration);

	void move();

	void print();

	~Monster();
};