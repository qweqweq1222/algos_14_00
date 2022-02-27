#include<iostream>
#include "mm.h"
class s{
private:
    int a=1;
    int b=1;
    char c= 'a';
};
void check_true_empty()
{
    lab618::CMemoryManager<s> manager(7, true);
    s** mass = new s* [5];
    for(int i = 0; i < 5; ++i)
    {
        mass[i] = manager.newObject();
        for (int i = 0; i < 6; ++i)
            manager.newObject();
    }
    manager.check();
    manager.check_exp();
    for(int i = 0; i < 5; ++i)
        manager.deleteObject(mass[i] + i);
    for(int i = 0; i < 5; ++i)
        manager.deleteObject(mass[i] + i + 1);
    manager.check();
    manager.check_exp();

}
void check_false_empty()
{
    lab618::CMemoryManager<s> manager(7, false);
    s** mass = new s* [5];
    for(int i = 0; i < 5; ++i)
    {
        mass[i] = manager.newObject();
        for (int i = 0; i < 6; ++i)
            manager.newObject();
    }
    for(int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 7; ++j)
            manager.deleteObject(mass[i] + j);
    }
    manager.check();
    manager.check_exp();
}
int main(void)
{
    check_true_empty();
    check_false_empty();
}