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
    lab618::CMemoryManager<s> manager(3, true);
    for(int i = 0; i < 4; ++i)
        manager.newObject();
    s* elem = manager.newObject();
    manager.check_exp();
    manager.deleteObject(elem);
    manager.check_exp();
}