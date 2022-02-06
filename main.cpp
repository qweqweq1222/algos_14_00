#ifndef TEMPLATES_LIST_2022_02_03
#define TEMPLATES_LIST_2022_02_03

#endif //#ifndef TEMPLATES_LIST_2022_02_03
#include<iostream>
#include "list_1.h"
int main(void)
{
/* qweqwe */
    int task = 0;
    lab618::CSingleLinkedList<int> List;
    while(task != 10)
    {
        std::cout <<"1 - push front " << std::endl <<  "2 - push back " << std::endl << "3 - clear " << std::endl <<  "4 - print " <<std::endl <<
        "5 - get size " <<std::endl << "6 - popFront" <<std::endl;
        std::cout << "Your choice:";
        std::cin >> task;
        std::cin.get();
        int data = 0;
        switch (task) {
            case 1:
                std::cout << "input data:";
                std::cin >> data;
                List.pushFront(data);
                break;
            case 2:
                std::cout << "input data:";
                std::cin >> data;
                List.pushBack(data);
                break;
            case 3:
                List.clear();
                break;
            case 4:
                std::cout << "Your list:";
                List.print_list();
                break;
            case 5:
                std::cout << "SIZE:" << List.getSize() << std::endl;
                break;
            case 6:
                std::cout << List.popFront() << std::endl;
                break;
        }
    }
    return 0;
}