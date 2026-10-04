#ifndef ARQUILA_DOUBLY_LINKED_LIST_HPP
#define ARQUILA_DOUBLY_LINKED_LIST_HPP

#include <cstddef>
#include <iostream>

template <typename T>
class DoublyLinkedList {
private:
    struct Node {
        T data;
        Node* previous;
        Node* next;

        Node(const T& value) : data(value), previous(nullptr), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    std::size_t count;

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

    std::size_t size() const {
        return count;
    }

    void pushBack(const T& value) {
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

    void pushFront(const T& value) {
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

        for (std::size_t i = 0; i < index; i++) {
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

    bool popFront(T& removedValue) {
        if (head == nullptr) {
            return false;
        }

        removedValue = head->data;
        unlink(head);

        return true;
    }

    bool popBack(T& removedValue) {
        if (tail == nullptr) {
            return false;
        }

        removedValue = tail->data;
        unlink(tail);

        return true;
    }

    bool remove(const T& value) {
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

    void printForward(std::ostream& out = std::cout) const {
        Node* current = head;

        while (current != nullptr) {
            out << current->data << " ";
            current = current->next;
        }

        out << std::endl;
    }

    void printBackward(std::ostream& out = std::cout) const {
        Node* current = tail;

        while (current != nullptr) {
            out << current->data << " ";
            current = current->previous;
        }

        out << std::endl;
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

#endif
