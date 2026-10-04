#include <sstream>
#include <string>

#include <catch2/catch_test_macros.hpp>

#include "doubly_linked_list/DoublyLinkedList.hpp"

namespace {

template <typename T>
std::string text(const DoublyLinkedList<T>& list) {
    std::ostringstream out;
    list.printForward(out);
    return out.str();
}

struct Tracked {
    static int live;

    int id;

    Tracked(int value) : id(value) {
        live++;
    }

    Tracked(const Tracked& other) : id(other.id) {
        live++;
    }

    Tracked& operator=(const Tracked&) = default;

    ~Tracked() {
        live--;
    }

    bool operator==(const Tracked& other) const {
        return id == other.id;
    }
};

int Tracked::live = 0;

template <typename T>
std::string backward(const DoublyLinkedList<T>& list) {
    std::ostringstream out;
    list.printBackward(out);
    return out.str();
}

}

TEST_CASE("doubly linked list inserts at both ends and in the middle", "[doubly]") {
    DoublyLinkedList<int> list;

    REQUIRE(list.isEmpty());

    list.pushBack(10);
    list.pushBack(20);
    list.pushFront(5);

    REQUIRE(list.insertAt(1, 7));
    REQUIRE(list.insertAt(0, 1));
    REQUIRE(list.insertAt(list.size(), 99));
    REQUIRE(list.insertAt(3, 8));
    REQUIRE_FALSE(list.insertAt(list.size() + 1, 0));

    REQUIRE(list.size() == 7);
    REQUIRE(text(list) == "1 5 7 8 10 20 99 \n");
    REQUIRE(list.contains(8));
    REQUIRE_FALSE(list.contains(9));
}

TEST_CASE("doubly linked list removes the first match and fixes both ends", "[doubly]") {
    DoublyLinkedList<int> list;

    for (int value : {10, 20, 30, 20, 40}) {
        list.pushBack(value);
    }

    REQUIRE(list.remove(20));
    REQUIRE(text(list) == "10 30 20 40 \n");
    REQUIRE(list.remove(10));
    REQUIRE(list.remove(40));
    REQUIRE_FALSE(list.remove(99));
    REQUIRE(text(list) == "30 20 \n");

    list.pushBack(50);
    list.pushFront(1);

    REQUIRE(text(list) == "1 30 20 50 \n");
    REQUIRE(list.size() == 4);
}

TEST_CASE("doubly linked list stays usable after it becomes empty", "[doubly]") {
    DoublyLinkedList<int> list;
    int value = 0;

    REQUIRE_FALSE(list.popFront(value));
    REQUIRE_FALSE(list.remove(1));

    list.pushBack(10);

    REQUIRE(list.remove(10));
    REQUIRE(list.isEmpty());

    list.pushBack(20);
    list.pushBack(30);

    REQUIRE(list.popFront(value));
    REQUIRE(value == 20);
    REQUIRE(list.popFront(value));
    REQUIRE(value == 30);
    REQUIRE(list.isEmpty());

    list.pushFront(40);
    list.clear();
    list.clear();

    REQUIRE(list.isEmpty());
    REQUIRE(list.size() == 0);

    list.pushBack(50);

    REQUIRE(text(list) == "50 \n");
}

TEST_CASE("doubly linked list releases every node of a long list", "[doubly]") {
    DoublyLinkedList<std::string> list;

    for (int i = 0; i < 1000; i++) {
        list.pushBack("node " + std::to_string(i));
    }

    for (int i = 0; i < 1000; i += 2) {
        REQUIRE(list.remove("node " + std::to_string(i)));
    }

    REQUIRE(list.size() == 500);
    REQUIRE(list.contains("node 999"));
    REQUIRE_FALSE(list.contains("node 998"));
}

TEST_CASE("doubly linked list keeps both directions consistent", "[doubly]") {
    DoublyLinkedList<int> list;
    int value = 0;

    REQUIRE_FALSE(list.popBack(value));

    list.pushBack(10);
    list.pushBack(20);
    list.pushFront(5);
    list.insertAt(2, 15);

    REQUIRE(text(list) == "5 10 15 20 \n");
    REQUIRE(backward(list) == "20 15 10 5 \n");

    REQUIRE(list.remove(10));
    REQUIRE(backward(list) == "20 15 5 \n");

    REQUIRE(list.popBack(value));
    REQUIRE(value == 20);
    REQUIRE(list.popFront(value));
    REQUIRE(value == 5);
    REQUIRE(text(list) == "15 \n");
    REQUIRE(backward(list) == "15 \n");

    REQUIRE(list.popBack(value));
    REQUIRE(list.isEmpty());
    REQUIRE(backward(list) == "\n");
}

TEST_CASE("doubly linked list destroys each value exactly once", "[doubly]") {
    {
        DoublyLinkedList<Tracked> list;

        for (int i = 1; i <= 50; i++) {
            list.pushBack(Tracked(i));
            list.pushFront(Tracked(-i));
        }

        REQUIRE(Tracked::live == 100);

        list.insertAt(10, Tracked(500));

        REQUIRE(list.remove(Tracked(500)));
        REQUIRE(list.remove(Tracked(50)));
        REQUIRE(list.remove(Tracked(-50)));
        REQUIRE_FALSE(list.remove(Tracked(500)));
        REQUIRE(Tracked::live == 98);

        {
            Tracked removed(0);

            REQUIRE(list.popFront(removed));
            REQUIRE(removed.id == -49);
        }

        REQUIRE(Tracked::live == 97);

        list.clear();

        REQUIRE(Tracked::live == 0);

        for (int i = 0; i < 10; i++) {
            list.pushBack(Tracked(i));
        }

        REQUIRE(Tracked::live == 10);
    }

    REQUIRE(Tracked::live == 0);
}
