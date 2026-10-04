#ifndef ARQUILA_ARRAY_EXAMPLES_HPP
#define ARQUILA_ARRAY_EXAMPLES_HPP

#include <iostream>

template <typename T>
void printArray(const T* values, int size, std::ostream& out = std::cout) {
    for (int i = 0; i < size; i++) {
        out << values[i] << " ";
    }

    out << std::endl;
}

template <typename T>
int linearSearch(const T* values, int size, const T& target) {
    for (int i = 0; i < size; i++) {
        if (values[i] == target) {
            return i;
        }
    }

    return -1;
}

template <typename T>
int binarySearch(const T* values, int size, const T& target) {
    int low = 0;
    int high = size - 1;

    while (low <= high) {
        int middle = low + (high - low) / 2;

        if (values[middle] == target) {
            return middle;
        }

        if (values[middle] < target) {
            low = middle + 1;
        } else {
            high = middle - 1;
        }
    }

    return -1;
}

template <typename T>
bool insertAt(T* values, int& size, int capacity, int index, const T& value) {
    if (size >= capacity || index < 0 || index > size) {
        return false;
    }

    for (int i = size; i > index; i--) {
        values[i] = values[i - 1];
    }

    values[index] = value;
    size++;
    return true;
}

template <typename T>
bool removeAt(T* values, int& size, int index) {
    if (index < 0 || index >= size) {
        return false;
    }

    for (int i = index; i < size - 1; i++) {
        values[i] = values[i + 1];
    }

    size--;
    return true;
}

template <typename T>
T* resize(T* values, int size, int newCapacity) {
    T* resized = new T[newCapacity];

    for (int i = 0; i < size && i < newCapacity; i++) {
        resized[i] = values[i];
    }

    delete[] values;
    return resized;
}

#endif
