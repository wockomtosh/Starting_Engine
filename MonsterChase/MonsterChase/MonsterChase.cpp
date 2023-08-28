#include <iostream>
#include <stdlib.h>
#include <conio.h>

using namespace std;

string playerName;
int playerPos[2] = { 0, 0 };
string monsterNames[5];
int monsterPos[5][2];

void printPos(int pos[2])
{
    cout << "[" << pos[0] << ", " << pos[1] << "]";
}

void getPlayerName()
{
    cout << "Enter Player Name: ";
    cin >> playerName;
}

void getNumMonsters()
{

}

void placeMonsters()
{

}

void startGame()
{
    getPlayerName();
    getNumMonsters();
    placeMonsters();
}

void moveMonsters()
{
    //For each monster
    //Choose x or y, pos or neg, add that to monster position
}

void addMonsters()
{

}

void displayMonsters()
{

}

void displayPlayer()
{
    cout << playerName;
    printPos(playerPos);
    cout << endl;
}

void queryPlayer()
{

}

void mainLoop()
{
    addMonsters();
    moveMonsters();
    displayMonsters();
    displayPlayer();
    queryPlayer();
}

int main()
{
    cout << "Starting Monster Chase" << endl;

    startGame();
    mainLoop();
}