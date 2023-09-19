#include <stdlib.h>
#include "ArrayUtils.h"

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