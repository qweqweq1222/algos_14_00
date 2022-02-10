#ifndef TEMPLATES_LIST_2022_02_03
#define TEMPLATES_LIST_2022_02_03

#endif //#ifndef TEMPLATES_LIST_2022_02_03
#include<iostream>
#include "list_1.h"

struct my_struct{
    std::string  str = "abc";
    int val = 0;
};
// erase по пустому списку
void erase_empty()
{
    lab618::CSingleLinkedList<my_struct> List;
    lab618::CSingleLinkedList<my_struct>::CIterator it = List.begin();
    List.erase(it);
}

//выводит список через итератор
void print(lab618::CSingleLinkedList<my_struct> &List)
{
    for(lab618::CSingleLinkedList<my_struct>::CIterator it = List.begin(); it.isValid(); ++it)
        std::cout << "string: " << (*it).str << " value: " << (*it).val <<std::endl;
}
//erase четные элементы списка
void erase_even(lab618::CSingleLinkedList<my_struct> &List)
{
    int i = List.getSize();
    for(lab618::CSingleLinkedList<my_struct>::CIterator it = List.begin(); it.isValid(); ++it)
    {
        if( i % 2 == 0)
            List.erase(it);
        i--;
        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
    }
    print(List);
}
int main(void)
{
    lab618::CSingleLinkedList<my_struct> List;
    my_struct s;
    int task =0;
    std::string str;
    while(task != 10) {
        std::cout << "1 - push front " << std::endl << "2 - push back " << std::endl << "3 - clear " << std::endl
                  << "4 - get size " << std::endl << "5 - popFront" << std::endl << "6 - print"<<std::endl
                  << "7 - delete even" << std::endl;
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
                erase_even(List);
                break;
        }
    }
    erase_empty();
    return 0;
}