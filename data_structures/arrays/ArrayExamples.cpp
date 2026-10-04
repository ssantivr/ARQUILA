#include <iostream>

#include "ArrayExamples.hpp"

using namespace std;

int main() {
    int values[5] = {10, 20, 30, 40, 50};

    printArray(values, 5);

    cout << "linearSearch(30): " << linearSearch(values, 5, 30) << endl;
    cout << "binarySearch(40): " << binarySearch(values, 5, 40) << endl;
    cout << "binarySearch(35): " << binarySearch(values, 5, 35) << endl;

    int capacity = 3;
    int size = 0;
    int* dynamicValues = new int[capacity];

    for (int i = 0; i < capacity; i++) {
        insertAt(dynamicValues, size, capacity, size, (i + 1) * 10);
    }

    printArray(dynamicValues, size);

    if (!insertAt(dynamicValues, size, capacity, 1, 15)) {
        capacity *= 2;
        dynamicValues = resize(dynamicValues, size, capacity);
        insertAt(dynamicValues, size, capacity, 1, 15);
    }

    printArray(dynamicValues, size);

    removeAt(dynamicValues, size, 0);
    printArray(dynamicValues, size);

    delete[] dynamicValues;

    return 0;
}
