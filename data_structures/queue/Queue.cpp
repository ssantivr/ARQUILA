#include <iostream>

using namespace std;

class Queue {
private:
    static const int CAPACITY = 100;

    int items[CAPACITY];
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

    bool enqueue(int value) {
        if (isFull()) {
            return false;
        }

        items[(front + count) % CAPACITY] = value;
        count++;
        return true;
    }

    bool dequeue(int& removedValue) {
        if (isEmpty()) {
            return false;
        }

        removedValue = items[front];
        front = (front + 1) % CAPACITY;
        count--;
        return true;
    }

    bool peek(int& frontValue) const {
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

int main() {
    Queue queue;

    queue.enqueue(10);
    queue.enqueue(20);
    queue.enqueue(30);

    int value;

    if (queue.peek(value)) {
        cout << "peek: " << value << endl;
    }

    while (queue.dequeue(value)) {
        cout << "dequeue: " << value << endl;
    }

    for (int i = 0; i < 250; i++) {
        queue.enqueue(i);
        queue.enqueue(i + 1);
        queue.dequeue(value);
        queue.dequeue(value);
    }

    cout << "size after wrap-around: " << queue.size() << endl;
    cout << "last dequeued: " << value << endl;

    return 0;
}
