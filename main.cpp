#ifndef TEMPLATES_LIST_2022_02_03
#define TEMPLATES_LIST_2022_02_03

#endif //#ifndef TEMPLATES_LIST_2022_02_03
#include<iostream>
#include "list_1.h"
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
int main(void)
{
    lab618::CSingleLinkedList<my_struct> List;
    my_struct s = my_struct();
    int task =1;
    std::string str = "abc";
    while(task != 0) {
        std::cout << "1 - push front " << std::endl << "2 - push back " << std::endl << "3 - clear " << std::endl
                  << "4 - get size " << std::endl << "5 - popFront" << std::endl << "6 - print"<<std::endl << "7 - fill list\n";
        std::cout << "Your choice:";
        std::cin >> task;
        std::cin.get();
        int data = 0;
        switch (task) {
            case 1:
                std::cout << "input data:" << "string:" ;
                getline(std::cin, s.str);
                std::cout << " val:";
                std::cin >> s.val;
                List.pushFront(s);
                break;
            case 2:
                std::cout << "input data:" << "string:" ;
                getline(std::cin, s.str);
                std::cout << "\n val:";
                std::cin >> s.val;
                List.pushBack(s);
                break;
            case 3:
                List.clear();
                break;
            case 4:
                std::cout << "SIZE:" << List.getSize() << std::endl;
                break;
            case 5:
                List.popFront();
                break;
            case 6:
                print(List);
                break;
            case 7:
                int N = 0;
                std::cout << "How many nodes you want to add? N:";
                std::cin >> N;
                full(List, N);
                break;
        }
    }
    erase_something(List);
    std::cout << "SIZE:" << List.getSize() << std::endl;
    print(List);
    pop_nothing();
    return 0;
}