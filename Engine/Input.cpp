#include <iostream>
#include <conio.h>
#include "Input.h"

char getInputChar(const char* prompt)
{
    std::cout << prompt;
    char ret = _getch();
    std::cout << std::endl;
    return ret;
}

char* getInputString(const char* prompt)
{
    char* playerInput = (char*)calloc(100, sizeof(char));
    std::cout << prompt;
    std::cin >> playerInput;
    //Check for the length of the player input and reduce the size accordingly
    //Get the length
    char curChar = NULL;
    if (playerInput != NULL) //To get rid of warning
    {
        curChar = playerInput[0];
    }
    int length = 0;
    while (curChar != NULL)
    {
        length += 1;
        curChar = playerInput[length];
    }
    //We need to have an extra space for a terminating null character
    length += 1;
    //Allocate less space for the input
    char* trimmedPlayerInput = (char*)calloc(length, sizeof(char));
    //copy over the input. Because of calloc the final char will be null
    for (int i = 0; i < length - 1; i++)
    {
        if (trimmedPlayerInput != NULL) //To get rid of warning
        {
            trimmedPlayerInput[i] = playerInput[i];
        }
    }
    return playerInput;
}

int getInputInt(const char* prompt)
{
    int playerInput;
    std::cout << prompt;
    std::cin >> playerInput;
    return playerInput;
}