#include <iostream>

using namespace std;

// O(n)
void printArray(const int* values, int size) {
    for (int i = 0; i < size; i++) {
        cout << values[i] << " ";
    }

    cout << endl;
}

// O(n). Returns the index of target or -1 if it is not present
int linearSearch(const int* values, int size, int target) {
    for (int i = 0; i < size; i++) {
        if (values[i] == target) {
            return i;
        }
    }

    return -1;
}

// O(log n). Requires values sorted in ascending order
int binarySearch(const int* values, int size, int target) {
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

// O(n). Shifts elements right; fails when the array is full or index is invalid
bool insertAt(int* values, int& size, int capacity, int index, int value) {
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

// O(n). Shifts elements left
bool removeAt(int* values, int& size, int index) {
    if (index < 0 || index >= size) {
        return false;
    }

    for (int i = index; i < size - 1; i++) {
        values[i] = values[i + 1];
    }

    size--;
    return true;
}

// O(n). Allocates a larger block, copies the elements and frees the old one
int* resize(int* values, int size, int newCapacity) {
    int* resized = new int[newCapacity];

    for (int i = 0; i < size && i < newCapacity; i++) {
        resized[i] = values[i];
    }

    delete[] values;
    return resized;
}

int main() {
    // Static array: fixed size, O(1) access by index
    int values[5] = {10, 20, 30, 40, 50};

    printArray(values, 5); // 10 20 30 40 50

    cout << "linearSearch(30): " << linearSearch(values, 5, 30) << endl; // 2
    cout << "binarySearch(40): " << binarySearch(values, 5, 40) << endl; // 3
    cout << "binarySearch(35): " << binarySearch(values, 5, 35) << endl; // -1

    // Dynamic array: memory reserved at runtime and released manually
    int capacity = 3;
    int size = 0;
    int* dynamicValues = new int[capacity];

    for (int i = 0; i < capacity; i++) {
        insertAt(dynamicValues, size, capacity, size, (i + 1) * 10);
    }

    printArray(dynamicValues, size); // 10 20 30

    // Full: grow before inserting again
    if (!insertAt(dynamicValues, size, capacity, 1, 15)) {
        capacity *= 2;
        dynamicValues = resize(dynamicValues, size, capacity);
        insertAt(dynamicValues, size, capacity, 1, 15);
    }

    printArray(dynamicValues, size); // 10 15 20 30

    removeAt(dynamicValues, size, 0);
    printArray(dynamicValues, size); // 15 20 30

    delete[] dynamicValues;

    return 0;
}
