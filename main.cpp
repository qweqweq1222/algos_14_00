#ifndef TEMPLATES_LIST_2022_02_03
#define TEMPLATES_LIST_2022_02_03

#endif //#ifndef TEMPLATES_LIST_2022_02_03
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
//функция заполнения
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
/* удаление четных элементов списка (проверка работы erase - из-за четности проверяется случай удаления головы)*/
void erase_something(lab618::CSingleLinkedList<my_struct> &List)
{
    int i =0;
    for(lab618::CSingleLinkedList<my_struct>::CIterator it = List.begin(); it.isValid(); ++it)
    {
        if(i % 2 == 0)
            List.erase(it);
        ++i;
    }
}
// удаление четных элементов, но уже с реверс-итератором
void erase_something_back(lab618::CDualLinkedList<my_struct> &List)
{
    int i =0;
    for(lab618::CDualLinkedList<my_struct>::CIterator it = List.end(); it.isValid(); --it)
    {
        if(i % 2 == 0)
            List.eraseAndNext(it);
        ++i;
    }
}

// достать из пустого списка
void pop_nothing()
{
    lab618::CSingleLinkedList<my_struct> List;
    full(List, 10);
    List.clear();
    List.popFront();
}

// erase по пустому списку
void erase_empty()
{
    lab618::CSingleLinkedList<my_struct> List;
    lab618::CSingleLinkedList<my_struct>::CIterator it = List.begin();
    List.erase(it);
}
void pop_end(lab618::CDualLinkedList<my_struct> &list)
{
    for(lab618::CDualLinkedList<my_struct>::CIterator it = list.begin(); it.isValid(); ++it)
        list.popFront();
}
int main(void)
{
    //comment
    my_struct s = my_struct();
    erase_empty();
    lab618::CDualLinkedList<my_struct> list;
    full_dual(list, 3);
    for(lab618::CDualLinkedList<my_struct>::CIterator it = list.end(); it.isValid(); --it)
        list.erase(it);
    list.pushFront(s);
    print_dual(list);
    return 0;
}