#ifndef ARQUILA_SINGLY_LINKED_LIST_HPP
#define ARQUILA_SINGLY_LINKED_LIST_HPP

#include <cstddef>
#include <iostream>

template <typename T>
class SinglyLinkedList {
private:
    struct Node {
        T data;
        Node* next;

        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    std::size_t count;

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

    std::size_t size() const {
        return count;
    }

    void pushFront(const T& value) {
        Node* newNode = new Node(value);
        newNode->next = head;
        head = newNode;

        if (tail == nullptr) {
            tail = newNode;
        }

        count++;
    }

    void pushBack(const T& value) {
        Node* newNode = new Node(value);

        if (tail == nullptr) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }

        count++;
    }

    bool insertAt(std::size_t index, const T& value) {
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

        for (std::size_t i = 0; i < index - 1; i++) {
            current = current->next;
        }

        Node* newNode = new Node(value);
        newNode->next = current->next;
        current->next = newNode;
        count++;

        return true;
    }

    bool popFront(T& removedValue) {
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

    bool remove(const T& value) {
        if (head == nullptr) {
            return false;
        }

        if (head->data == value) {
            Node* oldHead = head;
            head = head->next;

            if (head == nullptr) {
                tail = nullptr;
            }

            delete oldHead;
            count--;
            return true;
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

    bool contains(const T& value) const {
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

    void print(std::ostream& out = std::cout) const {
        Node* current = head;

        while (current != nullptr) {
            out << current->data << " ";
            current = current->next;
        }

        out << std::endl;
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

#endif
