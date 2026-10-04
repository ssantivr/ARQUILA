cmake_minimum_required(VERSION 3.25)

project(arquila_data_structures LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ARQUILA_SANITIZE "Build with AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
option(ARQUILA_BUILD_TESTS "Build the Catch2 tests" ON)

if(MINGW)
    add_link_options(-static)
endif()

if(ARQUILA_SANITIZE)
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()

set(ARQUILA_WARNINGS -Wall -Wextra -Wpedantic)

set(ARQUILA_PROGRAMS
    arrays/ArrayExamples
    stack/Stack
    queue/Queue
    singly_linked_list/SinglyLinkedList
    doubly_linked_list/DoublyLinkedList
)

foreach(program IN LISTS ARQUILA_PROGRAMS)
    get_filename_component(name ${program} NAME)
    add_executable(${name} ${program}.cpp)
    target_compile_options(${name} PRIVATE ${ARQUILA_WARNINGS})
endforeach()

if(ARQUILA_BUILD_TESTS)
    include(FetchContent)

    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.8.1
        GIT_SHALLOW ON
    )
    FetchContent_MakeAvailable(Catch2)

    add_executable(structure_tests
        tests/arrays_test.cpp
        tests/stack_test.cpp
        tests/queue_test.cpp
        tests/singly_linked_list_test.cpp
        tests/doubly_linked_list_test.cpp
    )
    target_include_directories(structure_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_compile_options(structure_tests PRIVATE ${ARQUILA_WARNINGS})
    target_link_libraries(structure_tests PRIVATE Catch2::Catch2WithMain)

    enable_testing()
    add_test(NAME structure_tests COMMAND structure_tests)
endif()
