#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "Point2D.h"
#include "Monster.h"
#include "Input.h"
#include <assert.h>


char* playerName;
Point2D playerPos = Point2D();
int numMonsters;
int totalMonsters;
Monster* monsters;

template <typename T>
T* appendToArray(T newValue, T* i_array, int i_arrayLength)
{
    //copy i_array over to new array with additional space
    T* o_array = (T*)malloc((i_arrayLength + 1) * sizeof(T));
    for (int i = 0; i < i_arrayLength; i++)
    {
        if (o_array != NULL)
        {
            o_array[i] = i_array[i];
        }
    }
    if (o_array != NULL)
    {
        //NULL check removes a warning
        o_array[i_arrayLength] = newValue; //add the new value
    }

    //free(i_array);
    delete i_array;

    return o_array;
}

template <typename T>
T* removeFromArray(int removeIndex, T* i_array, int i_arrayLength)
{
    //copy i_array over to new array with less space, don't copy over the removeValue
    //If the remove value isn't in the array, this will remove the final value in the array because that gets cut off. There isn't a check for the remove value.
    int newLength = i_arrayLength - 1; //This lets us get rid of a compiler warning.
    T* o_array = (T*)malloc(newLength * sizeof(T));
    int o_location = 0; //We lose a space when we remove so we need to track the location in our new array
    for (int i = 0; i < i_arrayLength; i++)
    {
        if (i != removeIndex && o_location < newLength)
        {
            if (o_array != NULL)
            {
                //NULL check removes a warnings
                o_array[o_location] = i_array[i];
            }
            o_location++;
        }
    }

    //free(i_array);
    delete i_array;

    return o_array;
}

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
        playerPos += Point2D::Up;
        break;
    case 'a':
        playerPos += Point2D::Left;
        break;
    case 's':
        playerPos += Point2D::Down;
        break;
    case 'd':
        playerPos += Point2D::Right;
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

void Point2DUnitTest()
{
    Point2D A(0, 1);
    Point2D B(2, 3);

    // equality
    bool bEqual = A == B;
    assert(bEqual == false);

    bEqual = B == Point2D(2, 3);
    assert(bEqual == true);

    // Inequality
    bool bNotEqual = A != B;
    assert(bNotEqual == true);

    bNotEqual = B != Point2D(2, 3);
    assert(bNotEqual == false);

    // Point2D + Point2D
    Point2D C = A + B;
    assert(C == Point2D(2, 4));

    // Point2D - Point2D
    C = B - A;
    assert(C == Point2D(2, 2));

    // Point2D * int
    C = A * 2;
    assert(C == Point2D(0, 2));

    // Point2D / int
    C = Point2D(6, 4) / 2;
    assert(C == Point2D(3, 2));

    // int * Point2D
    C = 2 * B;
    assert(C == Point2D(4, 6));

    // negate
    C = -B;
    assert(C == Point2D(-2, -3));

    // Point2D += Point2D
    B += Point2D(2, 1);
    assert(B == Point2D(4, 4));

    // Point2D -= Point2d
    B -= Point2D(2, 1);
    assert(B == Point2D(2, 3));

    // Point2D *= int
    B *= 2;
    assert(B == Point2D(4, 6));

    // Point2D /= int
    B /= 2;
    assert(B == Point2D(2, 3));
}

int main()
{
    Point2DUnitTest();  
    std::cout << "Starting Monster Chase" << std::endl;

    startGame();
    mainLoop();
}