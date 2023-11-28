#include <iostream>
#include <stdio.h>
#include <conio.h>

#define REMOVE_CHAR
//#define REMOVE_CHAR_USER_INPUT
#define ITOA

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
				if (i_String[cleanIndices[0]] != i_CharToRemove)
				{
					i_String[0] = i_String[cleanIndices[0]];
				}
				else 
				{
					i_String[i] = cleanIndices[i];
				}
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
#ifdef REMOVE_CHAR_USER_INPUT
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
#else
	char* abba = _strdup("abba");
	std::cout << "Before string: " << abba << "\n";
	removeChar(abba, 'a');
	std::cout << "After string: " << abba << "\n";

	char* a = _strdup("a");
	std::cout << "Before string: " << a << "\n";
	removeChar(a, 'a');
	std::cout << "After string: " << a << "\n";

	char* b = _strdup("b");
	std::cout << "Before string: " << b << "\n";
	removeChar(b, 'a');
	std::cout << "After string: " << b << "\n";

	char* aabbaabbcca = _strdup("aabbaabbcca");
	std::cout << "Before string: " << aabbaabbcca << "\n";
	removeChar(aabbaabbcca, 'a');
	std::cout << "After string: " << aabbaabbcca << "\n";

	char* aabbaabbcc = _strdup("aabbaabbcc");
	std::cout << "Before string: " << aabbaabbcc << "\n";
	removeChar(aabbaabbcc, 'a');
	std::cout << "After string: " << aabbaabbcc << "\n";

	char* baabbaabbcca = _strdup("baabbaabbcca");
	std::cout << "Before string: " << baabbaabbcca << "\n";
	removeChar(baabbaabbcca, 'a');
	std::cout << "After string: " << baabbaabbcca << "\n";
#endif
}

int getNumDigits(int i_int)
{
	int numDigits = 0;

	if (i_int < 10)
	{
		return 1;
	}

	while (i_int > 0)
	{
		i_int /= 10;
		numDigits++;
	}

	return numDigits;
}

char* itoa(int i_int)
{
	std::cout << "Input is: " << i_int << "\n";

	if (i_int == 0)
	{
		char* output = static_cast<char*>(malloc(1));
		if (output != NULL)
		{
			output[0] = '0';
		}
	}

	char zeroCharOffset = 48;

	int numDigits = getNumDigits(i_int);
	char* outString = static_cast<char*>(malloc(numDigits + 1));
	int outStringLocation = numDigits - 1;
	if (outString != NULL)
	{
		while (outStringLocation >= 0)
		{
			//Get the last digit (num % 10) and convert that to char (add 48 to it and cast to char)
			outString[outStringLocation] = static_cast<char>((i_int % 10) + zeroCharOffset);
			outStringLocation--;
			i_int /= 10;
		}
		outString[numDigits] = NULL;
	}
	
	return outString;
}

void runItoa()
{
	char* bigZeros = itoa(10000);
	std::cout << "Output is: " << bigZeros << "\n";
	char* zero = itoa(0);
	std::cout << "Output is: " << zero << "\n";
	char* randomNum = itoa(1234);
	std::cout << "Output is: " << randomNum << "\n";
	char* randomNum2 = itoa(10203040);
	std::cout << "Output is: " << randomNum2 << "\n";
}

int main()
{
#ifdef REMOVE_CHAR
	std::cout << "Starting RemoveChar...\n";
	runRemoveChar();
	std::cout << "\n";
#endif

#ifdef ITOA
	std::cout << "Starting itoa...\n";
	runItoa();
	std::cout << "\n";
#endif
}