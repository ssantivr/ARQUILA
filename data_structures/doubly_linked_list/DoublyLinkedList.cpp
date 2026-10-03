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

    bool isEmpty() const {
        return head == nullptr;
    }

    size_t size() const {
        return count;
    }

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

    bool popFront(int& removedValue) {
        if (head == nullptr) {
            return false;
        }

        removedValue = head->data;
        unlink(head);

        return true;
    }

    bool popBack(int& removedValue) {
        if (tail == nullptr) {
            return false;
        }

        removedValue = tail->data;
        unlink(tail);

        return true;
    }

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

    void printForward() const {
        Node* current = head;

        while (current != nullptr) {
            cout << current->data << " ";
            current = current->next;
        }

        cout << endl;
    }

    void printBackward() const {
        Node* current = tail;

        while (current != nullptr) {
            cout << current->data << " ";
            current = current->previous;
        }

        cout << endl;
    }

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
