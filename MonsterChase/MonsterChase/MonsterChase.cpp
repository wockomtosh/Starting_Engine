#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "MonsterChase.h"
#include "Input.h"


template <typename T>
T* MonsterChase::appendToArray(T newValue, T* i_array, int i_arrayLength)
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
        o_array[i_arrayLength] = newValue; //add the new value
    }

    //free(i_array);
    delete i_array;

    return o_array;
}

template <typename T>
T* MonsterChase::removeFromArray(int removeIndex, T* i_array, int i_arrayLength)
{
    //copy i_array over to new array with less space, don't copy over the removeValue
    //If the remove value isn't in the array, this will remove the final value in the array because that gets cut off. There isn't a check for the remove value.
    int newLength = i_arrayLength - 1; //This lets us get rid of a compiler warning.
    T* o_array = (T*)malloc(newLength * sizeof(T));
    int o_location = 0; //We lose a space when we remove so we need to track the position in our new array
    for (int i = 0; i < i_arrayLength; i++)
    {
        if (i != removeIndex && o_location < newLength)
        {
            if (o_array != NULL)
            {
                //This NULL check should remove the warning, but for some reason it isn't....
                o_array[o_location] = i_array[i];
            }
            o_location++;
        }
    }

    //free(i_array);
    delete i_array;

    return o_array;
}

void MonsterChase::initMonsters()
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

void MonsterChase::startGame()
{
    /*playerName = getInputString("Enter Player Name: ");
    numMonsters = getInputInt("Enter Number of initial Monsters: ");*/
    playerName = "Player";
    numMonsters = 4;
    totalMonsters = numMonsters;
    initMonsters();
}

//Old main loop:
//game->addRemoveMonsters();
//game->moveMonsters();
//game->displayMonsters();
//game->displayPlayer();
//game->queryPlayer();

void MonsterChase::removeMonster(int index)
{
    monsters = removeFromArray(index, monsters, numMonsters);
    numMonsters -= 1;
}

void MonsterChase::addMonster()
{
    //monsters = appendToArray(Monster("Monster", totalMonsters), monsters, numMonsters);
    numMonsters += 1;
    totalMonsters += 1;
}

void MonsterChase::addRemoveMonsters()
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

void MonsterChase::moveMonsters()
{
    for (int i = 0; i < numMonsters; i++)
    {
        monsters[i].move();
    }
}

void MonsterChase::displayMonsters()
{
    std::cout << std::endl;
    for (int i = 0; i < numMonsters; i++)
    {
        monsters[i].print();
    }
}

void MonsterChase::displayPlayer()
{
    std::cout << std::endl << playerName << " ";
    playerPos.print();
    std::cout << std::endl;
}

void MonsterChase::queryPlayer()
{
    char input = getInputChar("Use the WASD keys to move or Q to quit");
    switch (input)
    {
    case 'w':
        //move player up
        playerPos += Vector2::Up;
        break;
    case 'a':
        playerPos += Vector2::Left;
        break;
    case 's':
        playerPos += Vector2::Down;
        break;
    case 'd':
        playerPos += Vector2::Right;
        break;
    case 'q':
        exit(0);
        break;
    }
}

void MonsterChase::RenderMonsters(GLib::Sprite* monsterSprite)
{
    static GLib::Point2D	totalOffset = { 180.0f, -100.0f };

    int monsterLocationScale = 1;

    for (int i = 0; i < numMonsters; i++)
    {
        Vector2 newLocation = Vector2(monsters[i].getLocation().getX() * monsterLocationScale + totalOffset.x, monsters[i].getLocation().getY() * monsterLocationScale + totalOffset.y);
        GLib::Render(*monsterSprite, { static_cast<float>(newLocation.getX()), static_cast<float>(newLocation.getY()) }, 0.0f, 0.0f);
    }
}

void MonsterChase::Point2DUnitTest()
{
    Vector2 A(0, 1);
    Vector2 B(2, 3);

    // equality
    bool bEqual = A == B;
    assert(bEqual == false);

    bEqual = B == Vector2(2, 3);
    assert(bEqual == true);

    // Inequality
    bool bNotEqual = A != B;
    assert(bNotEqual == true);

    bNotEqual = B != Vector2(2, 3);
    assert(bNotEqual == false);

    // Vector2 + Vector2
    Vector2 C = A + B;
    assert(C == Vector2(2, 4));

    // Vector2 - Vector2
    C = B - A;
    assert(C == Vector2(2, 2));

    // Vector2 * int
    C = A * 2;
    assert(C == Vector2(0, 2));

    // Vector2 / int
    C = Vector2(6, 4) / 2;
    assert(C == Vector2(3, 2));

    // int * Vector2
    C = 2 * B;
    assert(C == Vector2(4, 6));

    // negate
    C = -B;
    assert(C == Vector2(-2, -3));

    // Vector2 += Vector2
    B += Vector2(2, 1);
    assert(B == Vector2(4, 4));

    // Vector2 -= Point2d
    B -= Vector2(2, 1);
    assert(B == Vector2(2, 3));

    // Vector2 *= int
    B *= 2;
    assert(B == Vector2(4, 6));

    // Vector2 /= int
    B /= 2;
    assert(B == Vector2(2, 3));
}