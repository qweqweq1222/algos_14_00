#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include<iostream>
#include "list.h"
#include <random>
#include <cstdlib>
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
    TEST_CASE("erase_begin_end")
    {
        lab618::CDualLinkedList<my_struct> list;
        full_dual(list,5);
        lab618::CDualLinkedList<my_struct>::CIterator it = list.begin();
        for(it; it.isValid(); ++it) {
            std::cout << list.getSize() << ' ';
            list.erase(it);
        }
        std::cout << std::endl;
        REQUIRE(list.getSize() == 0);
    }
    TEST_CASE("erase_end_begin")
    {
        lab618::CDualLinkedList<my_struct> list;
        full_dual(list,5);
        lab618::CDualLinkedList<my_struct>::CIterator it = list.end();
        for(it; it.isValid(); --it) {
            list.erase(it);
            print_dual(list);
        }
        std::cout << std::endl;
        REQUIRE(list.getSize() == 2);
    }
    TEST_CASE("erase_end_begin____")
    {
        lab618::CDualLinkedList<my_struct> list;
        full_dual(list,5);
        lab618::CDualLinkedList<my_struct>::CIterator it = list.end();
        for(it; it.isValid(); ++it) {
            list.erase(it);
            print_dual(list);
        }
        std::cout << std::endl;
        REQUIRE(list.getSize() == 4);
    }
    TEST_CASE("eraseAndNext_begin_end")
    {
        lab618::CDualLinkedList<my_struct> list;
        full_dual(list,5);
        lab618::CDualLinkedList<my_struct>::CIterator it = list.begin();
        for(it; it.isValid(); ++it) {
            std::cout << list.getSize() << ' ';
            list.eraseAndNext(it);
        }
        std::cout << std::endl;
        REQUIRE(list.getSize() == 2);
    }
    TEST_CASE("eraseAndNext_end_begin")
    {
        lab618::CDualLinkedList<my_struct> list;
        full_dual(list,5);
        lab618::CDualLinkedList<my_struct>::CIterator it = list.end();
        for(it; it.isValid(); --it) {
            list.eraseAndNext(it);
        }
        std::cout << std::endl;
        REQUIRE(list.getSize() == 0);
    }
    TEST_CASE("erase_middle")
    {
        lab618::CDualLinkedList<my_struct> list;
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
        lab618::CDualLinkedList<my_struct> list;
        lab618::CDualLinkedList<my_struct> list_;
        full_dual(list,5);
        full_dual(list_,5);
        lab618::CDualLinkedList<my_struct>::CIterator it = list.begin();
        ++it;
        ++it;
        int size = list.getSize();
        list_.erase(it);
        std::cout << std::endl;
    //REQUIRE(list.getSize() == 3);
    }
    TEST_CASE("erase_randomly_from_begin")
    {
        int random = 0;
        lab618::CDualLinkedList<my_struct> list;
        full_dual(list,5);
        int size = list.getSize();
        lab618::CDualLinkedList<my_struct>::CIterator it = list.begin();
        for(it; it.isValid(); ++it)
        {
            random = rand() % 2;
            if(random == 1) {
                list.erase(it);
                size -=1;
            }
        }
        print_dual(list);
        REQUIRE(size == list.getSize());
    }
    TEST_CASE("erase_randomly_from_end")
    {
        int random = 0;
        lab618::CDualLinkedList<my_struct> list;
        full_dual(list,5);
        int size = list.getSize();
        lab618::CDualLinkedList<my_struct>::CIterator it = list.end();
        for(it; it.isValid(); --it)
        {
            random = rand() % 2;
            if(random == 1)
            {
                list.erase(it);
                size-=1;
            }
        }
        print_dual(list);
        REQUIRE(size == list.getSize());
    }
    TEST_CASE("eraseAndNeXT_randomly_from_end")
    {
        int random = 0;
        lab618::CDualLinkedList<my_struct> list;
        full_dual(list,5);
        int size = list.getSize();
        lab618::CDualLinkedList<my_struct>::CIterator it = list.end();
        for(it; it.isValid(); ++it)
        {
            random = rand() % 2;
            if(random == 1)
            {
                size -= 1;
                list.eraseAndNext(it);
            }
        }
        print_dual(list);
        REQUIRE(size == list.getSize());
    }
    TEST_CASE("eraseAndNeXT_randomly_from_begin")
    {
        int random = 0;
        lab618::CDualLinkedList<my_struct> list;
        full_dual(list,5);
        int size = list.getSize();
        lab618::CDualLinkedList<my_struct>::CIterator it = list.begin();
        for(it; it.isValid(); ++it)
        {
            random = rand() % 2;
            if(random == 1)
            {
                size -= 1;
                list.eraseAndNext(it);
            }
        }
        print_dual(list);
        REQUIRE(size == list.getSize());
    }
    TEST_CASE("eraseAndNeXT_from_random_place_++")
    {
        int random = 0;
        lab618::CDualLinkedList<my_struct> list;
        full_dual(list,5);
        int size = list.getSize();
        lab618::CDualLinkedList<my_struct>::CIterator it = list.begin();

        random = rand() % size;
        for(int i =0; i<rand(); ++i)
            ++it;
        for(it; it.isValid(); ++it)
        {
            random = rand() % 2;
            if(random == 1)
            {
                size -= 1;
                list.eraseAndNext(it);
            }
        }
        print_dual(list);
        REQUIRE(size == list.getSize());
    }
    TEST_CASE("eraseAndNeXT_from_random_place_--")
    {
        int random = 0;
        lab618::CDualLinkedList<my_struct> list;
        full_dual(list,5);
        int size = list.getSize();
        lab618::CDualLinkedList<my_struct>::CIterator it = list.end();

        random = rand() % size;
        for(int i =0; i<rand(); ++i)
            --it;
        for(it; it.isValid(); --it)
        {
            random = rand() % 2;
            if(random == 1)
            {
                size -= 1;
                list.eraseAndNext(it);
            }
        }
        print_dual(list);
        REQUIRE(size == list.getSize());
    }
}

    TEST_CASE("2 lists ")
    {
        lab618::CDualLinkedList<my_struct> list;
        lab618::CDualLinkedList<my_struct> list_;
        full_dual(list,5);
        full_dual(list_,5);
        lab618::CDualLinkedList<my_struct>::CIterator it = list.begin();
        for(it; it.isValid(); ++it)
            list_.erase(it);
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