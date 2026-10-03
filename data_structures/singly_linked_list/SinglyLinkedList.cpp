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

    bool isEmpty() const {
        return head == nullptr;
    }

    size_t size() const {
        return count;
    }

    void pushFront(int value) {
        Node* newNode = new Node(value);
        newNode->next = head;
        head = newNode;

        if (tail == nullptr) {
            tail = newNode;
        }

        count++;
    }

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

    void print() const {
        Node* current = head;

        while (current != nullptr) {
            cout << current->data << " ";
            current = current->next;
        }

        cout << endl;
    }

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
