#include <string>

#include <catch2/catch_test_macros.hpp>

#include "queue/Queue.hpp"

TEST_CASE("queue returns values in FIFO order", "[queue]") {
    Queue<int> queue;
    int value = 0;

    REQUIRE(queue.isEmpty());

    queue.enqueue(10);
    queue.enqueue(20);
    queue.enqueue(30);

    REQUIRE(queue.size() == 3);
    REQUIRE(queue.peek(value));
    REQUIRE(value == 10);

    REQUIRE(queue.dequeue(value));
    REQUIRE(value == 10);
    REQUIRE(queue.dequeue(value));
    REQUIRE(value == 20);
    REQUIRE(queue.dequeue(value));
    REQUIRE(value == 30);
    REQUIRE(queue.isEmpty());
}

TEST_CASE("queue rejects dequeue and peek when empty", "[queue]") {
    Queue<int> queue;
    int value = 7;

    REQUIRE_FALSE(queue.dequeue(value));
    REQUIRE_FALSE(queue.peek(value));
    REQUIRE(value == 7);
}

TEST_CASE("queue rejects enqueue when full", "[queue]") {
    Queue<int> queue;

    for (int i = 0; i < 100; i++) {
        REQUIRE(queue.enqueue(i));
    }

    REQUIRE(queue.isFull());
    REQUIRE_FALSE(queue.enqueue(100));
    REQUIRE(queue.size() == 100);
}

TEST_CASE("queue reuses the slots freed by dequeue", "[queue]") {
    Queue<int> queue;
    int value = 0;

    for (int i = 0; i < 250; i++) {
        REQUIRE(queue.enqueue(i));
        REQUIRE(queue.enqueue(i + 1000));
        REQUIRE(queue.dequeue(value));
        REQUIRE(value == i);
        REQUIRE(queue.dequeue(value));
        REQUIRE(value == i + 1000);
    }

    REQUIRE(queue.isEmpty());
}

TEST_CASE("queue keeps the order after wrapping around", "[queue]") {
    Queue<int> queue;
    int value = 0;

    for (int i = 0; i < 100; i++) {
        queue.enqueue(i);
    }

    for (int i = 0; i < 60; i++) {
        queue.dequeue(value);
    }

    for (int i = 100; i < 160; i++) {
        REQUIRE(queue.enqueue(i));
    }

    REQUIRE(queue.isFull());

    for (int i = 60; i < 160; i++) {
        REQUIRE(queue.dequeue(value));
        REQUIRE(value == i);
    }

    queue.enqueue(5);
    queue.clear();

    REQUIRE(queue.isEmpty());
}

TEST_CASE("queue stores types other than int", "[queue]") {
    Queue<std::string> queue;
    std::string value;

    queue.enqueue("first");
    queue.enqueue("second");

    REQUIRE(queue.dequeue(value));
    REQUIRE(value == "first");
}
