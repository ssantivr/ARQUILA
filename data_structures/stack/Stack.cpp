#include <iostream>

using namespace std;

class Stack {
private:
    static const int CAPACITY = 100;

    int items[CAPACITY];
    int top;

public:
    Stack() : top(-1) {}

    // O(1)
    bool isEmpty() const {
        return top == -1;
    }

    // O(1)
    bool isFull() const {
        return top == CAPACITY - 1;
    }

    // O(1)
    int size() const {
        return top + 1;
    }

    // O(1). Returns false on overflow
    bool push(int value) {
        if (isFull()) {
            return false;
        }

        items[++top] = value;
        return true;
    }

    // O(1). Returns false on underflow, so any int can be stored safely
    bool pop(int& removedValue) {
        if (isEmpty()) {
            return false;
        }

        removedValue = items[top--];
        return true;
    }

    // O(1)
    bool peek(int& topValue) const {
        if (isEmpty()) {
            return false;
        }

        topValue = items[top];
        return true;
    }

    // O(1)
    void clear() {
        top = -1;
    }
};

int main() {
    Stack stack;

    stack.push(10);
    stack.push(20);
    stack.push(30);

    int value;

    if (stack.peek(value)) {
        cout << "peek: " << value << endl; // 30
    }

    while (stack.pop(value)) {
        cout << "pop: " << value << endl;  // 30, 20, 10 (LIFO)
    }

    cout << "empty: " << stack.isEmpty() << endl;            // 1
    cout << "pop on empty: " << stack.pop(value) << endl;    // 0

    return 0;
}
