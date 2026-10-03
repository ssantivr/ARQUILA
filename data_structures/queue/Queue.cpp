#include <iostream>

using namespace std;

// Circular queue: slots freed by dequeue are reused, so the queue only
// reports full when it really holds CAPACITY elements.
class Queue {
private:
    static const int CAPACITY = 100;

    int items[CAPACITY];
    int front;
    int count;

public:
    Queue() : front(0), count(0) {}

    // O(1)
    bool isEmpty() const {
        return count == 0;
    }

    // O(1)
    bool isFull() const {
        return count == CAPACITY;
    }

    // O(1)
    int size() const {
        return count;
    }

    // O(1). Returns false on overflow
    bool enqueue(int value) {
        if (isFull()) {
            return false;
        }

        items[(front + count) % CAPACITY] = value;
        count++;
        return true;
    }

    // O(1). Returns false on underflow, so any int can be stored safely
    bool dequeue(int& removedValue) {
        if (isEmpty()) {
            return false;
        }

        removedValue = items[front];
        front = (front + 1) % CAPACITY;
        count--;
        return true;
    }

    // O(1)
    bool peek(int& frontValue) const {
        if (isEmpty()) {
            return false;
        }

        frontValue = items[front];
        return true;
    }

    // O(1)
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
        cout << "peek: " << value << endl;    // 10
    }

    while (queue.dequeue(value)) {
        cout << "dequeue: " << value << endl; // 10, 20, 30 (FIFO)
    }

    // Wrap-around: more enqueues than CAPACITY in total, never more than 2 stored
    for (int i = 0; i < 250; i++) {
        queue.enqueue(i);
        queue.enqueue(i + 1);
        queue.dequeue(value);
        queue.dequeue(value);
    }

    cout << "size after wrap-around: " << queue.size() << endl; // 0
    cout << "last dequeued: " << value << endl;                 // 250

    return 0;
}
