#include<iostream>
#include "mm.h"
class s{
private:
    int a=1;
    int b=1;
    char c= 'a';
};
void check_true()
{
    lab618::CMemoryManager<s> manager(3, true);
    manager.newObject();
    s* elem_1 = manager.newObject();
    manager.newObject();
    manager.newObject();
    s* elem_2 = manager.newObject();
    manager.newObject();
    manager.check();
    manager.check_exp();
    manager.deleteObject(elem_1);
    manager.deleteObject(elem_2);
    manager.check();
    manager.check_exp();
}
void check_delete()
{
    lab618::CMemoryManager<s> manager(3, false);
    s* elem_1 = manager.newObject();
    s* elem_2 = manager.newObject();
    s* elem_3 = manager.newObject();
    s* elem_4 = manager.newObject();
    manager.deleteObject(elem_4);
    manager.deleteObject(elem_3);
    manager.deleteObject(elem_2);
    manager.deleteObject(elem_1);
    manager.check();
}
int main(void)
{
    check_delete();
    check_true();
}