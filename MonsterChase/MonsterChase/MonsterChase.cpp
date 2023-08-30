#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include "Point2D.h";


char* playerName;
Point2D playerPos = Point2D(0, 0);
int numMonsters;
Point2D* monsterLocations;

char getInputChar(const char* prompt)
{
    std::cout << prompt;
    char ret = _getch();
    std::cout << std::endl;
    return ret;
}

char* getInputString(const char* prompt)
{
    char playerInput[20];
    std::cout << prompt;
    std::cin >> playerInput;
    return playerInput;
}

int getInputInt(const char* prompt)
{
    int playerInput;
    std::cout << prompt;
    std::cin >> playerInput;
    return playerInput;
}

void placeMonsters()
{
    for (int i = 0; i < numMonsters; i++)
    {
        //for monster in list, get rand int and put it somewhere
        int randx = rand() % 100 - 50;
        int randy = rand() % 100 - 50;
        monsterLocations[i] = Point2D(randx, randy);
    }
}

void startGame()
{
    playerName = getInputString("Enter Player Name: ");
    numMonsters = getInputInt("Enter Number of initial Monsters: ");
    monsterLocations = (Point2D*) malloc(numMonsters * sizeof(Point2D));
    placeMonsters();
    //Init Monster Names
}

void moveMonsters()
{
    for (int i = 0; i < numMonsters; i++)
    {
        //Choose x or y, pos or neg, add that to monster position
        int xOrY = rand() % 2;
        int posOrNeg = rand() % 2;
        if (xOrY == 0) //this will be x
        {
            if (posOrNeg == 0) //this will be positive
            {
                monsterLocations[i].setX(monsterLocations[i].getX() + 1);
            }
            else
            {
                monsterLocations[i].setX(monsterLocations[i].getX() - 1);
            }
        }
        else
        {
            if (posOrNeg == 0)
            {
                monsterLocations[i].setY(monsterLocations[i].getY() + 1);
            }
            else
            {
                monsterLocations[i].setY(monsterLocations[i].getY() - 1);
            }
        }
    }
}

void addMonsters()
{
    //Have some counter that whenever it gets to a certain point we add a monster
    //Remove monsters after a certain number of turns? 
}

void displayMonsters()
{
    std::cout << std::endl;
    for (int i = 0; i < numMonsters; i++)
    {
        //Print MonsterNames
        monsterLocations[i].print();
        std::cout << std::endl;
    }
}

void displayPlayer()
{
    std::cout << std::endl << playerName << " ";
    playerPos.print();
    std::cout << std::endl;
}

void queryPlayer(bool &playing)
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
        playing = false;
        break;
    }
}

void mainLoop()
{
    bool playing = true;
    while(playing)
    {
        addMonsters();
        moveMonsters();
        displayMonsters();
        displayPlayer();
        queryPlayer(playing);
    }
}

int main()
{
    std::cout << "Starting Monster Chase" << std::endl;
    srand(time(NULL));

    startGame();
    mainLoop();
}