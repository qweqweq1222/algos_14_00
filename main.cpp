#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include<iostream>
#include "list.h"
#include <random>

// генерация рандомных строк
std::string random_string(std::string::size_type length)
{
    static auto& chrs = "0123456789"
                        "abcdefghijklmnopqrstuvwxyz"
                        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    thread_local static std::mt19937 rg{std::random_device{}()};
    thread_local static std::uniform_int_distribution<std::string::size_type> pick(0, sizeof(chrs) - 2);

    std::string s;

    s.reserve(length);

    while(length--)
        s += chrs[pick(rg)];

    return s;
}

struct my_struct{
    std::string  str = "abc";
    int val = 0;
};


//выводит список через итератор
void print(lab618::CSingleLinkedList<my_struct> &List)
{
    std::cout << " Your list:\n";
    for(lab618::CSingleLinkedList<my_struct>::CIterator it = List.begin(); it.isValid(); ++it)
        std::cout << "(" << (*it).str << ", " << it.getData().val << ")\n";
    std::cout << std::endl;
}
void print_dual(lab618::CDualLinkedList<my_struct>  &List)
{
    std::cout << " Your list:\n";
    for(lab618::CDualLinkedList<my_struct>::CIterator it = List.begin(); it.isValid(); ++it)
        std::cout << "(" << (*it).str << ", " << it.getData().val << ")\n";
    std::cout << std::endl;
}
void full(lab618::CSingleLinkedList<my_struct> &List, int N)
{
    my_struct s;
    for(int i =0; i< N; ++i) {
        s.str = random_string(i % 10);
        s.val = i % 255;
        List.pushBack(s);
    }
}
void full_dual(lab618::CDualLinkedList<my_struct>  &List, int N)
{
    my_struct s;
    for(int i =0; i< N; ++i) {
        s.str = random_string(i % 10);
        s.val = i % 255;
        List.pushBack(s);
    }
}

TEST_SUITE("dual_erase")
{
    lab618::CDualLinkedList<my_struct> list;
    TEST_CASE("erase_begin_end")
    {
        full_dual(list,5);
        for(lab618::CDualLinkedList<my_struct>::CIterator it = list.begin(); it.isValid(); ++it) {
            std::cout << list.getSize() << ' ';
            list.erase(it);
        }
        std::cout << std::endl;
        REQUIRE(list.getSize() == 0);
    }
    TEST_CASE("erase_end_begin")
    {
        full_dual(list,5);
        for(lab618::CDualLinkedList<my_struct>::CIterator it = list.end(); it.isValid(); --it) {
            std::cout << list.getSize() << ' ';
            list.erase(it);
        }
        std::cout << std::endl;
        REQUIRE(list.getSize() == 0);
    }
    TEST_CASE("eraseAndNext_begin_end")
    {
        full_dual(list,5);
        for(lab618::CDualLinkedList<my_struct>::CIterator it = list.begin(); it.isValid(); ++it) {
            std::cout << list.getSize() << ' ';
            list.eraseAndNext(it);
        }
        std::cout << std::endl;
        REQUIRE(list.getSize() == 0);
    }
    TEST_CASE("eraseAndNext_end_begin")
    {
        full_dual(list,5);
        for(lab618::CDualLinkedList<my_struct>::CIterator it = list.end(); it.isValid(); --it) {
            std::cout << list.getSize() << ' ';
            list.eraseAndNext(it);
        }
        std::cout << std::endl;
        REQUIRE(list.getSize() == 0);
    }
    TEST_CASE("erase_middle")
    {
        full_dual(list,5);
        lab618::CDualLinkedList<my_struct>::CIterator it = list.begin();
        ++it;
        ++it;
        for(it; it.isValid(); ++it) {
            std::cout << list.getSize() << ' ';
            list.erase(it);
        }
        std::cout << std::endl;
        REQUIRE(list.getSize() == 2);
    }
    TEST_CASE("eraseAndNext_middle")
    {
        full_dual(list,5);
        lab618::CDualLinkedList<my_struct>::CIterator it = list.begin();
        ++it;
        ++it;
        int size = list.getSize();
        for(it; it.isValid(); ++it) {
            list.eraseAndNext(it);
            size = list.getSize();
        }
        std::cout << std::endl;
        REQUIRE(list.getSize() == 4);
    }
}

/*int main(void)
{
    lab618::CDualLinkedList<my_struct> list;
    full_dual(list,5);
    lab618::CDualLinkedList<my_struct>::CIterator it = list.begin();
    ++it;
    ++it;
    for(it; it.isValid(); ++it) {
        list.eraseAndNext(it);
    }
    std::cout << list.getSize() << ' ';
    std::cout << std::endl;
}*/