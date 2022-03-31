#include <iostream>
#include "avltree.h"
struct my_struct {
	std::string name;
	int data;
};

std::ostream& operator << (std::ostream& ostrm, my_struct rhs) noexcept
{
	ostrm << rhs.data;
	return ostrm;
}

int Compare(const my_struct* left,const my_struct* right)
{
	if(left->data < right->data)
		return -1;
	else if(left->data == right->data)
		return 0;
	return 1;
}
int main(void)
{
	return 0;
}

