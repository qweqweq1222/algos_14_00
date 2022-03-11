#include<iostream>
#include "mm.h"
#include <vector>
#include <typeinfo>
class s{
public:
    int a=1;
    int b=1;
    char c= 'a';
};
std::ostream& operator << (std::ostream& ostrm, s& r) noexcept
{
    ostrm << r.a << r.b << r.c;
}

void test_(bool var)
{
    lab618::CMemoryManager<s> manager(7, var);
    std::vector<s*> vec;
    for(int i = 0; i < 14; ++i)
        vec.push_back(manager.newObject());
    for(int i = 0; i < 14; ++i)
        if (i % 2 == 0)
            manager.deleteObject(vec[i]);
    for(int i = 0; i < 14; ++i)
        std::cout << *vec[i] << " ";
    std::cout << std::endl;
}
void test_complete_deleted(bool var)
{
    lab618::CMemoryManager<s> manager(7, var);
    std::vector<s *> vec;
    for(int i = 0; i < 14; ++i)
        vec.push_back(manager.newObject());
    for(int i = 0; i < 14; ++i)
        manager.deleteObject(vec[i]);
    for(int i = 0; i < 14; ++i)
        std::cout << *vec[i] << " ";
    std::cout << std::endl;
}


int main(void)
{
    test_(true);
    test_complete_deleted(true);
    test_complete_deleted(false);
    test_(false); // падаем
}