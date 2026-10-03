#include <cstddef>
#include <iostream>

using namespace std;

class DoublyLinkedList {
private:
    struct Node {
        int data;
        Node* previous;
        Node* next;

        Node(int value)
            : data(value), previous(nullptr), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    size_t count;

    // O(1). Detaches node from the list and frees it
    void unlink(Node* node) {
        if (node->previous != nullptr) {
            node->previous->next = node->next;
        } else {
            head = node->next;
        }

        if (node->next != nullptr) {
            node->next->previous = node->previous;
        } else {
            tail = node->previous;
        }

        delete node;
        count--;
    }

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), count(0) {}

    DoublyLinkedList(const DoublyLinkedList&) = delete;
    DoublyLinkedList& operator=(const DoublyLinkedList&) = delete;

    ~DoublyLinkedList() {
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
    void pushBack(int value) {
        Node* newNode = new Node(value);

        if (tail == nullptr) {
            head = tail = newNode;
        } else {
            newNode->previous = tail;
            tail->next = newNode;
            tail = newNode;
        }

        count++;
    }

    // O(1)
    void pushFront(int value) {
        Node* newNode = new Node(value);

        if (head == nullptr) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->previous = newNode;
            head = newNode;
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

        for (size_t i = 0; i < index; i++) {
            current = current->next;
        }

        Node* newNode = new Node(value);
        newNode->previous = current->previous;
        newNode->next = current;
        current->previous->next = newNode;
        current->previous = newNode;
        count++;

        return true;
    }

    // O(1)
    bool popFront(int& removedValue) {
        if (head == nullptr) {
            return false;
        }

        removedValue = head->data;
        unlink(head);

        return true;
    }

    // O(1)
    bool popBack(int& removedValue) {
        if (tail == nullptr) {
            return false;
        }

        removedValue = tail->data;
        unlink(tail);

        return true;
    }

    // O(n). Removes the first occurrence of value
    bool remove(int value) {
        Node* current = head;

        while (current != nullptr) {
            if (current->data == value) {
                unlink(current);
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

    // O(n)
    void printForward() const {
        Node* current = head;

        while (current != nullptr) {
            cout << current->data << " ";
            current = current->next;
        }

        cout << endl;
    }

    // O(n)
    void printBackward() const {
        Node* current = tail;

        while (current != nullptr) {
            cout << current->data << " ";
            current = current->previous;
        }

        cout << endl;
    }

    // O(n)
    void clear() {
        Node* current = head;

        while (current != nullptr) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }

        head = nullptr;
        tail = nullptr;
        count = 0;
    }
};

int main() {
    DoublyLinkedList list;

    list.pushBack(10);
    list.pushBack(20);
    list.pushFront(5);
    list.insertAt(2, 15);
    list.printForward();  // 5 10 15 20

    list.remove(10);
    list.printForward();  // 5 15 20
    list.printBackward(); // 20 15 5

    int removedValue;

    if (list.popBack(removedValue)) {
        cout << "popBack: " << removedValue << endl;   // 20
    }

    if (list.popFront(removedValue)) {
        cout << "popFront: " << removedValue << endl;  // 5
    }

    cout << "size: " << list.size() << endl;                // 1
    cout << "contains 15: " << list.contains(15) << endl;   // 1

    return 0;
}
