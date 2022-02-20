#include<iostream>
#include "manager.h"
class s{
private:
    int a=1;
    int b=1;
    char c= 'a';
};
int main(void)
{
    lab618::CMemoryManager<s> manager(2, true);
    int sa = sizeof(s);
    manager.newObject();
    manager.newObject();
    manager.newObject();
    manager.newObject();
    manager.newObject();
    manager.clear();
}