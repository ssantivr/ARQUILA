#include <iostream>

#include "SinglyLinkedList.hpp"

using namespace std;

int main() {
    SinglyLinkedList<int> list;

    list.pushBack(10);
    list.pushBack(20);
    list.pushFront(5);
    list.insertAt(1, 7);
    list.print();

    list.remove(20);
    list.pushBack(30);
    list.print();

    list.reverse();
    list.print();

    cout << "size: " << list.size() << endl;
    cout << "contains 10: " << list.contains(10) << endl;
    cout << "contains 20: " << list.contains(20) << endl;

    int removedValue;

    if (list.popFront(removedValue)) {
        cout << "popFront: " << removedValue << endl;
    }

    list.print();

    return 0;
}
