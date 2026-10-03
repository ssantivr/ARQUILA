#include <cstddef>
#include <iostream>

using namespace std;

class SinglyLinkedList {
private:
    struct Node {
        int data;
        Node* next;

        Node(int value) : data(value), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    size_t count;

public:
    SinglyLinkedList() : head(nullptr), tail(nullptr), count(0) {}

    SinglyLinkedList(const SinglyLinkedList&) = delete;
    SinglyLinkedList& operator=(const SinglyLinkedList&) = delete;

    ~SinglyLinkedList() {
        clear();
    }

    // O(1)
    bool isEmpty() const {
        return head == nullptr;
    }

    // O(1)
    size_t size() const {
        return count;
    }

    // O(1)
    void pushFront(int value) {
        Node* newNode = new Node(value);
        newNode->next = head;
        head = newNode;

        if (tail == nullptr) {
            tail = newNode;
        }

        count++;
    }

    // O(1) thanks to the tail pointer
    void pushBack(int value) {
        Node* newNode = new Node(value);

        if (tail == nullptr) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }

        count++;
    }

    // O(n). Valid positions: 0..size()
    bool insertAt(size_t index, int value) {
        if (index > count) {
            return false;
        }

        if (index == 0) {
            pushFront(value);
            return true;
        }

        if (index == count) {
            pushBack(value);
            return true;
        }

        Node* current = head;

        for (size_t i = 0; i < index - 1; i++) {
            current = current->next;
        }

        Node* newNode = new Node(value);
        newNode->next = current->next;
        current->next = newNode;
        count++;

        return true;
    }

    // O(1)
    bool popFront(int& removedValue) {
        if (head == nullptr) {
            return false;
        }

        Node* oldHead = head;
        removedValue = oldHead->data;
        head = head->next;

        if (head == nullptr) {
            tail = nullptr;
        }

        delete oldHead;
        count--;

        return true;
    }

    // O(n). Removes the first occurrence of value
    bool remove(int value) {
        if (head == nullptr) {
            return false;
        }

        if (head->data == value) {
            int discarded;
            return popFront(discarded);
        }

        Node* current = head;

        while (current->next != nullptr) {
            if (current->next->data == value) {
                Node* removed = current->next;
                current->next = removed->next;

                if (removed == tail) {
                    tail = current;
                }

                delete removed;
                count--;
                return true;
            }

            current = current->next;
        }

        return false;
    }

    // O(n)
    bool contains(int value) const {
        Node* current = head;

        while (current != nullptr) {
            if (current->data == value) {
                return true;
            }

            current = current->next;
        }

        return false;
    }

    // O(n) time, O(1) extra space
    void reverse() {
        Node* previous = nullptr;
        Node* current = head;
        tail = head;

        while (current != nullptr) {
            Node* nextNode = current->next;
            current->next = previous;
            previous = current;
            current = nextNode;
        }

        head = previous;
    }

    // O(n)
    void print() const {
        Node* current = head;

        while (current != nullptr) {
            cout << current->data << " ";
            current = current->next;
        }

        cout << endl;
    }

    // O(n)
    void clear() {
        while (head != nullptr) {
            Node* current = head;
            head = head->next;
            delete current;
        }

        tail = nullptr;
        count = 0;
    }
};

int main() {
    SinglyLinkedList list;

    list.pushBack(10);
    list.pushBack(20);
    list.pushFront(5);
    list.insertAt(1, 7);
    list.print(); // 5 7 10 20

    list.remove(20);
    list.pushBack(30);
    list.print(); // 5 7 10 30

    list.reverse();
    list.print(); // 30 10 7 5

    cout << "size: " << list.size() << endl;                   // 4
    cout << "contains 10: " << list.contains(10) << endl;      // 1
    cout << "contains 20: " << list.contains(20) << endl;      // 0

    int removedValue;

    if (list.popFront(removedValue)) {
        cout << "popFront: " << removedValue << endl;          // 30
    }

    list.print(); // 10 7 5

    return 0;
}
