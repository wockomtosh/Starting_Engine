#include <iostream>
#include <stdlib.h>
#include "Monster.h"

Monster::Monster(const char* name, int number)
{
    lifeSpan = rand() % 6 + 2;
    duration = 0;
    this->number = number;
    int randx = rand() % 100 - 50;
    int randy = rand() % 100 - 50;
    setLocation(Point2D(randx, randy));
    setName(name);
}


int Monster::getLifeSpan()
{
	return lifeSpan;
}

void Monster::setLifeSpan(int newLifeSpan)
{
	lifeSpan = newLifeSpan;
}

int Monster::getDuration()
{
	return duration;
}

void Monster::setDuration(int newDuration)
{
	duration = newDuration;
}

void Monster::move()
{
    //Choose x or y, pos or neg, add that to monster position
    int xOrY = rand() % 2;
    int posOrNeg = rand() % 2;
    if (xOrY == 0) //this will be x
    {
        if (posOrNeg == 0) //this will be positive
        {
            getLocation().setX(getLocation().getX() + 1);
        }
        else
        {
            getLocation().setX(getLocation().getX() - 1);
        }
    }
    else
    {
        if (posOrNeg == 0)
        {
            getLocation().setY(getLocation().getY() + 1);
        }
        else
        {
            getLocation().setY(getLocation().getY() - 1);
        }
    }
    //Increase the amount of time the monster has been alive
    duration += 1;
}

void Monster::print()
{
    std::cout << getName() << number << " ";
	getLocation().print();
	std::cout << " Lifespan: " << lifeSpan << " Duration: " << duration << std::endl;
}

Monster::~Monster()
{
}

