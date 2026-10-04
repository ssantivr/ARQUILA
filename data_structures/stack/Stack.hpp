#ifndef ARQUILA_STACK_HPP
#define ARQUILA_STACK_HPP

template <typename T>
class Stack {
private:
    static const int CAPACITY = 100;

    T items[CAPACITY];
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

    bool push(const T& value) {
        if (isFull()) {
            return false;
        }

        items[++top] = value;
        return true;
    }

    bool pop(T& removedValue) {
        if (isEmpty()) {
            return false;
        }

        removedValue = items[top--];
        return true;
    }

    bool peek(T& topValue) const {
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

#endif
