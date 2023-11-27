#include <iostream>
#include <stdio.h>
#include <conio.h>

#define REMOVE_CHAR

void removeChar(char* i_String, char i_CharToRemove)
{
	size_t stringLength = strlen(i_String);
	//Store all the chars that WON'T be removed. We will overwrite the string with these chars
	int* cleanIndices = static_cast<int*>(calloc(stringLength, sizeof(int)));
	int curCleanIndex = 0;
	if (cleanIndices != NULL)
	{
		for (int i = 0; i < stringLength; i++)
		{
			if (i_String[i] != i_CharToRemove) {
				cleanIndices[curCleanIndex] = i;
				curCleanIndex++;
			}
		}
	}

	//Now we overwrite the string with the clean indices, and write NULL to the end.
	if (cleanIndices != NULL)
	{
		for (int i = 0; i < stringLength; i++) {
			//if i == 0 and cleanIndices[i] == 0 then we run into issues with the null comparison
			if (i == 0)
			{
				i_String[i] = i_String[cleanIndices[i]];
			}
			else
			{
				if (cleanIndices[i] != NULL)
				{
					i_String[i] = i_String[cleanIndices[i]];
				}
				else
				{
					i_String[i] = cleanIndices[i];
				}
			}
		}
	}
}

void runRemoveChar()
{
	std::cout << "Enter a string (max length is 64): ";
	size_t maxLen = 64;
	char* inputString = static_cast<char*>(malloc(maxLen));
	if (inputString != NULL) //To get rid of warning
	{
		gets_s(inputString, maxLen);
	}
	std::cout << "Enter a char to remove: ";
	char charToRemove = _getch();
	std::cout << charToRemove << '\n';

	removeChar(inputString, charToRemove);
	std::cout << "After RemoveChar: " << inputString << '\n';
}

int main()
{
#ifdef REMOVE_CHAR
	std::cout << "Starting RemoveChar...\n";
	runRemoveChar();	
#endif
}