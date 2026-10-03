#include <iostream>

using namespace std;

void printArray(const int* values, int size) {
    for (int i = 0; i < size; i++) {
        cout << values[i] << " ";
    }

    cout << endl;
}

int linearSearch(const int* values, int size, int target) {
    for (int i = 0; i < size; i++) {
        if (values[i] == target) {
            return i;
        }
    }

    return -1;
}

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

int* resize(int* values, int size, int newCapacity) {
    int* resized = new int[newCapacity];

    for (int i = 0; i < size && i < newCapacity; i++) {
        resized[i] = values[i];
    }

    delete[] values;
    return resized;
}

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
