#ifndef ARQUILA_QUEUE_HPP
#define ARQUILA_QUEUE_HPP

template <typename T>
class Queue {
private:
    static const int CAPACITY = 100;

    T items[CAPACITY];
    int front;
    int count;

public:
    Queue() : front(0), count(0) {}

    bool isEmpty() const {
        return count == 0;
    }

    bool isFull() const {
        return count == CAPACITY;
    }

    int size() const {
        return count;
    }

    bool enqueue(const T& value) {
        if (isFull()) {
            return false;
        }

        items[(front + count) % CAPACITY] = value;
        count++;
        return true;
    }

    bool dequeue(T& removedValue) {
        if (isEmpty()) {
            return false;
        }

        removedValue = items[front];
        front = (front + 1) % CAPACITY;
        count--;
        return true;
    }

    bool peek(T& frontValue) const {
        if (isEmpty()) {
            return false;
        }

        frontValue = items[front];
        return true;
    }

    void clear() {
        front = 0;
        count = 0;
    }
};

#endif
