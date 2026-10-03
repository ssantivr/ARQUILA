#include <iostream>

using namespace std;

class Stack {
private:
    static const int CAPACITY = 100;

    int items[CAPACITY];
    int top;

public:
    Stack() : top(-1) {}

    bool isEmpty() const {
        return top == -1;
    }

    bool isFull() const {
        return top == CAPACITY - 1;
    }

    int size() const {
        return top + 1;
    }

    bool push(int value) {
        if (isFull()) {
            return false;
        }

        items[++top] = value;
        return true;
    }

    bool pop(int& removedValue) {
        if (isEmpty()) {
            return false;
        }

        removedValue = items[top--];
        return true;
    }

    bool peek(int& topValue) const {
        if (isEmpty()) {
            return false;
        }

        topValue = items[top];
        return true;
    }

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
        cout << "peek: " << value << endl;
    }

    while (stack.pop(value)) {
        cout << "pop: " << value << endl;
    }

    cout << "empty: " << stack.isEmpty() << endl;
    cout << "pop on empty: " << stack.pop(value) << endl;

    return 0;
}
