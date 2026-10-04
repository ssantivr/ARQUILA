#include <iostream>

#include "DoublyLinkedList.hpp"

using namespace std;

int main() {
    DoublyLinkedList<int> list;

    list.pushBack(10);
    list.pushBack(20);
    list.pushFront(5);
    list.insertAt(2, 15);
    list.printForward();

    list.remove(10);
    list.printForward();
    list.printBackward();

    int removedValue;

    if (list.popBack(removedValue)) {
        cout << "popBack: " << removedValue << endl;
    }

    if (list.popFront(removedValue)) {
        cout << "popFront: " << removedValue << endl;
    }

    cout << "size: " << list.size() << endl;
    cout << "contains 15: " << list.contains(15) << endl;

    return 0;
}
