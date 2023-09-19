#pragma once

template <typename T>
T* appendToArray(T newValue, T* i_array, int i_arrayLength);

template <typename T>
T* removeFromArray(int removeIndex, T* i_array, int i_arrayLength);