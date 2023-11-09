#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#else
#include <stdlib.h>
#endif // _DEBUG
#include <cassert>
#include <iostream>

//#define SAMPLE_STRINGS

int stringLength(const char* string) {
	char curChar = NULL;
	if (string != NULL)
	{
		curChar = string[0];
	}
	int length = 0;
	while (curChar != NULL)
	{
		length += 1;
		curChar = string[length];
	}

	return length;
}

char* getInputString(const char* prompt)
{
	const size_t maxLen = 128;
	char* playerInput = (char*)malloc(maxLen * sizeof(char));
	std::cout << prompt;
	//std::cin >> playerInput;
	assert(playerInput != NULL);
	gets_s(playerInput, maxLen);

	int length = stringLength(playerInput);
	//Add space for the terminator
	length += 1;

	//Allocate less space for the input
	char* trimmedPlayerInput = (char*)malloc(length * sizeof(char));
	//Copy over the input. Add null as a terminator at the end.
	for (int i = 0; i < length - 1; i++)
	{
		if (trimmedPlayerInput != NULL) //To get rid of warning
		{
			trimmedPlayerInput[i] = playerInput[i];
		}
	}
	trimmedPlayerInput[length - 1] = NULL;
	free(playerInput);
	return trimmedPlayerInput;
}

char** appendToStringArray(char* newValue, char** i_array, int i_arrayLength)
{
	//copy i_array over to new array with additional space
	size_t numStrings = static_cast<size_t>(i_arrayLength) + static_cast<size_t>(1);
	char** o_array = static_cast<char**>(malloc(numStrings * sizeof(char*)));

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

int arrayLength(char** strings) {
	char* curPointer = NULL;
	if (strings != NULL)
	{
		curPointer = strings[0];
	}
	int length = 0;
	while (curPointer != NULL)
	{
		length += 1;
		curPointer = strings[length];
	}

	return length;
}

char* MakeSentence(char** strings) {
	const size_t maxStringLength = 128;
	int length = arrayLength(strings);

	size_t totalChars = 0;

	//Get the length of the strings in the list. Add 1 to each of them for the spaces/period
	for (int i = 0; i < length; i++) {
		char* string = strings[i];
		totalChars += (static_cast<size_t>(stringLength(string)) + static_cast<size_t>(1));
	}
	//add 1 for the string terminator
	totalChars++;

	//allocate the final sentence
	char* sentence = static_cast<char*>(malloc(totalChars * sizeof(char)));

	//build the sentence with the strings, adding spaces and the period.
	char* sentenceLocation = sentence;
	for (int i = 0; i < length; i++) {
		//We have the sentence location, copy over each string into that place while moving along the sentence
		char* string = strings[i];

		char curChar = NULL;
		if (string != NULL)
		{
			curChar = string[0];
			sentenceLocation[0] = curChar;
			sentenceLocation++;
		}
		int stringLocation = 1;
		curChar = string[stringLocation];
		while (curChar != NULL)
		{
			sentenceLocation[0] = curChar;
			stringLocation++;
			sentenceLocation++;
			curChar = string[stringLocation];
		}

		if (i == length - 1) 
		{
			sentenceLocation[0] = '.';
			sentenceLocation++;
		}
		else
		{
			sentenceLocation[0] = ' ';
			sentenceLocation++;
		}
	}
	sentenceLocation[0] = NULL;

	free(strings);

	return sentence;
}

int main()
{

#ifdef SAMPLE_STRINGS	
	// a sample set of words
	const char* strings[] = {
		"This",
		"is",
		"a",
		"test",
		NULL
	};
#else
	char** strings = NULL;
	int stringsLength = 0;

	do
	{
		// ask the user for a word.
		char* input = getInputString("Enter a word for the sentence, empty to end: ");

		if (stringLength(input) > 0)
		{
			// if the user entered a word add it to the list.

			strings = appendToStringArray(input, strings, stringsLength);
			stringsLength++;

			// you need to duplicate the string the user input (Input)
			// store it in a linked list or array until the user
			// enters an empty input
		}
		else
		{
			// if not break out of the do / while loop
			
			//Adding a null to the end of the array to give the array a terminator
			strings = appendToStringArray(NULL, strings, stringsLength);

			break;
		}

	} while (1);

	// once all of the strings have been input then need to be put
	// into a const char * [] as above so they can be passed to MakeSentence()
#endif

	// MakeSentence() is the function we need to implement.
	// it should take in an array of words and create a sentence,
	// allocating EXACTLY enough memory for the resultant string.
	// it should use malloc() to allocate the memory.
	// it can use no other CRT or STL functions.
	char* pSentence = MakeSentence(strings);

	// print out the sentence
	printf("The Sentence is: %s", pSentence);

	// free the memory it allocated.
	free(pSentence);

	// in a Debug build check for memory leaks.
#if defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif // _DEBUG
}