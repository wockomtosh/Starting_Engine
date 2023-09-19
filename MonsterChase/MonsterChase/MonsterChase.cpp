#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "Point2D.h"
#include "Monster.h"
#include "Input.h"
#include "ArrayUtils.h"


char* playerName;
Point2D playerPos = Point2D();
int numMonsters;
int totalMonsters;
Monster* monsters;

void initMonsters()
{
    monsters = (Monster*)malloc(numMonsters * sizeof(Monster));
    for (int i = 0; i < numMonsters; i++) 
    {
        if (monsters != NULL)
        {
            monsters[i] = Monster("Monster", i);
        }
    }
}

void startGame()
{
    playerName = getInputString("Enter Player Name: ");
    numMonsters = getInputInt("Enter Number of initial Monsters: ");
    totalMonsters = numMonsters;
    initMonsters();
}

void removeMonster(int index)
{
    Monster* toDelete = &monsters[index];
    monsters = removeFromArray(index, monsters, numMonsters);
    numMonsters -= 1;
}

void addMonster()
{
    monsters = appendToArray(Monster("Monster", totalMonsters), monsters, numMonsters);
    numMonsters += 1;
    totalMonsters += 1;
}

void addRemoveMonsters()
{
    //Remove monsters after a certain number of turns
    //Start from the end and work backwards to not mess with the order of the arrays by removing during the loop
    for (int i = numMonsters-1; i > -1; i--)
    {
        if (monsters[i].getDuration() >= monsters[i].getLifeSpan())
        {
            removeMonster(i);
            //add a monster for each removed monster
            addMonster();
        }
    }
}

void moveMonsters()
{
    for (int i = 0; i < numMonsters; i++)
    {
        monsters[i].move();
    }
}

void displayMonsters()
{
    std::cout << std::endl;
    for (int i = 0; i < numMonsters; i++)
    {
        monsters[i].print();
    }
}

void displayPlayer()
{
    std::cout << std::endl << playerName << " ";
    playerPos.print();
    std::cout << std::endl;
}

void queryPlayer()
{
    char input = getInputChar("Use the WASD keys to move or Q to quit");
    switch (input)
    {
    case 'w':
        //move player up
        playerPos.setY(playerPos.getY() + 1);
        break;
    case 'a':
        playerPos.setX(playerPos.getX() - 1);
        break;
    case 's':
        playerPos.setY(playerPos.getY() - 1);
        break;
    case 'd':
        playerPos.setX(playerPos.getX() + 1);
        break;
    case 'q':
        exit(0);
        break;
    }
}

void mainLoop()
{
    while(true)
    {
        addRemoveMonsters();
        moveMonsters();
        displayMonsters();
        displayPlayer();
        queryPlayer();
    }
}

int main()
{
    std::cout << "Starting Monster Chase" << std::endl;

    startGame();
    mainLoop();
}