#include <string>

#include <catch2/catch_test_macros.hpp>

#include "stack/Stack.hpp"

TEST_CASE("stack returns values in LIFO order", "[stack]") {
    Stack<int> stack;
    int value = 0;

    REQUIRE(stack.isEmpty());

    stack.push(10);
    stack.push(20);
    stack.push(30);

    REQUIRE(stack.size() == 3);
    REQUIRE(stack.peek(value));
    REQUIRE(value == 30);

    REQUIRE(stack.pop(value));
    REQUIRE(value == 30);
    REQUIRE(stack.pop(value));
    REQUIRE(value == 20);
    REQUIRE(stack.pop(value));
    REQUIRE(value == 10);
    REQUIRE(stack.isEmpty());
}

TEST_CASE("stack rejects pop and peek when empty", "[stack]") {
    Stack<int> stack;
    int value = 7;

    REQUIRE_FALSE(stack.pop(value));
    REQUIRE_FALSE(stack.peek(value));
    REQUIRE(value == 7);
}

TEST_CASE("stack rejects push when full", "[stack]") {
    Stack<int> stack;

    for (int i = 0; i < 100; i++) {
        REQUIRE(stack.push(i));
    }

    REQUIRE(stack.isFull());
    REQUIRE_FALSE(stack.push(100));
    REQUIRE(stack.size() == 100);

    int value = 0;

    REQUIRE(stack.peek(value));
    REQUIRE(value == 99);

    stack.clear();

    REQUIRE(stack.isEmpty());
    REQUIRE(stack.push(1));
}

TEST_CASE("stack stores types other than int", "[stack]") {
    Stack<std::string> stack;
    std::string value;

    stack.push("plan");
    stack.push("elevation");

    REQUIRE(stack.pop(value));
    REQUIRE(value == "elevation");
    REQUIRE(stack.pop(value));
    REQUIRE(value == "plan");
}
