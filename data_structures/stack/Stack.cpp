#include <iostream>

#include "Stack.hpp"

using namespace std;

int main() {
    Stack<int> stack;

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
