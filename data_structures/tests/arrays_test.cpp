#include <sstream>
#include <string>

#include <catch2/catch_test_macros.hpp>

#include "arrays/ArrayExamples.hpp"

TEST_CASE("searches return the index or -1", "[arrays]") {
    const int values[5] = {10, 20, 30, 40, 50};

    REQUIRE(linearSearch(values, 5, 30) == 2);
    REQUIRE(linearSearch(values, 5, 35) == -1);
    REQUIRE(binarySearch(values, 5, 10) == 0);
    REQUIRE(binarySearch(values, 5, 40) == 3);
    REQUIRE(binarySearch(values, 5, 50) == 4);
    REQUIRE(binarySearch(values, 5, 35) == -1);
    REQUIRE(binarySearch(values, 0, 10) == -1);
}

TEST_CASE("insertAt shifts values to the right", "[arrays]") {
    int values[4] = {10, 20, 30, 0};
    int size = 3;

    REQUIRE(insertAt(values, size, 4, 1, 15));
    REQUIRE(size == 4);
    REQUIRE(values[0] == 10);
    REQUIRE(values[1] == 15);
    REQUIRE(values[2] == 20);
    REQUIRE(values[3] == 30);

    REQUIRE_FALSE(insertAt(values, size, 4, 0, 5));
    REQUIRE(size == 4);
}

TEST_CASE("insertAt rejects positions out of range", "[arrays]") {
    int values[4] = {10, 20, 0, 0};
    int size = 2;

    REQUIRE_FALSE(insertAt(values, size, 4, -1, 5));
    REQUIRE_FALSE(insertAt(values, size, 4, 3, 5));
    REQUIRE(insertAt(values, size, 4, 2, 30));
    REQUIRE(values[2] == 30);
}

TEST_CASE("removeAt shifts values to the left", "[arrays]") {
    int values[4] = {10, 20, 30, 40};
    int size = 4;

    REQUIRE(removeAt(values, size, 1));
    REQUIRE(size == 3);
    REQUIRE(values[0] == 10);
    REQUIRE(values[1] == 30);
    REQUIRE(values[2] == 40);

    REQUIRE_FALSE(removeAt(values, size, 3));
    REQUIRE_FALSE(removeAt(values, size, -1));
}

TEST_CASE("resize keeps the values in a block of another size", "[arrays]") {
    int capacity = 2;
    int size = 0;
    int* values = new int[capacity];

    insertAt(values, size, capacity, 0, 10);
    insertAt(values, size, capacity, 1, 20);

    REQUIRE_FALSE(insertAt(values, size, capacity, 2, 30));

    capacity *= 2;
    values = resize(values, size, capacity);

    REQUIRE(insertAt(values, size, capacity, 2, 30));
    REQUIRE(values[0] == 10);
    REQUIRE(values[1] == 20);
    REQUIRE(values[2] == 30);

    values = resize(values, size, 2);

    REQUIRE(values[0] == 10);
    REQUIRE(values[1] == 20);

    delete[] values;
}

TEST_CASE("array functions work with types other than int", "[arrays]") {
    const std::string names[3] = {"brick", "glass", "wood"};
    std::ostringstream out;

    REQUIRE(linearSearch(names, 3, std::string("glass")) == 1);
    REQUIRE(binarySearch(names, 3, std::string("wood")) == 2);

    printArray(names, 3, out);

    REQUIRE(out.str() == "brick glass wood \n");
}
