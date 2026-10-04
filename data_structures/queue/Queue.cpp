#include <iostream>

#include "Queue.hpp"

using namespace std;

int main() {
    Queue<int> queue;

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
