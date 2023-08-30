#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include "Point2D.h"
#include "Monster.h"


char* playerName;
Point2D playerPos = Point2D();
int numMonsters;
int totalMonsters;
Monster* monsters;

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

void initMonsters()
{
    monsters = (Monster*)malloc(numMonsters * sizeof(Monster));
    for (int i = 0; i < numMonsters; i++) 
    {
        monsters[i] = Monster("Monster", i);
    }
}

void startGame()
{
    playerName = getInputString("Enter Player Name: ");
    numMonsters = getInputInt("Enter Number of initial Monsters: ");
    totalMonsters = numMonsters;
    initMonsters();
}

template <typename T>
T* appendToArray(T newValue, T* i_array, int i_arrayLength)
{
    //copy i_array over to new array with additional space
    T* o_array = (T*)malloc(i_arrayLength + 1 * sizeof(T));
    for (int i = 0; i < i_arrayLength; i++)
    {
        o_array[i] = i_array[i];
    }
    o_array[i_arrayLength] = newValue; //add the new value

    //free(i_array);

    return o_array;
}

template <typename T>
T* removeFromArray(int removeIndex, T* i_array, int i_arrayLength) 
{
    //copy i_array over to new array with less space, don't copy over the removeValue
    T* o_array = (T*)malloc((i_arrayLength - 1) * sizeof(T));
    int o_location = 0; //We lose a space when we remove so we need to track the location in our new array
    for (int i = 0; i < i_arrayLength; i++)
    {
        if (i != removeIndex) 
        {
            o_array[o_location] = i_array[i];
            o_location++;
        }
    }

    //free(i_array);

    return o_array;
}

void removeMonster(int index)
{
    Monster* toDelete = &monsters[index];
    monsters = removeFromArray(index, monsters, numMonsters);
    numMonsters -= 1;
    //delete toDelete;
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