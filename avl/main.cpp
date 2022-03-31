#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <iostream>
#include "avltree.h"
#include "doctest.h"
struct my_struct {
	int data;
};

std::ostream& operator << (std::ostream& ostrm, my_struct rhs) noexcept
{
	ostrm << rhs.data;
	return ostrm;
}
std::string GenerateString(const int length)
{
	char symbols[27]  = "abcdefghijklmnopqrstuvwxyz";
	char str[length];
	for(int i = 0; i < length; ++i)
		str[i] = symbols[rand()%26];
	std::string str_(str);
	return str_;
}
int Compare(const my_struct* left,const my_struct* right)
{
	if(left->data < right->data)
		return -1;
	else if(left->data == right->data)
		return 0;
	return 1;
}
int CompareInt(const my_struct* left,const my_struct* right)
{
	if(left->data < right->data)
		return -1;
	else if(left->data == right->data)
		return 0;
	return 1;
}
TEST_CASE("add + find")
{
	int N = 100;
	std::vector<my_struct> data;
	lab618::CAVLTree<my_struct, CompareInt> Tree;
	for(int i = 0; i < N; ++i) data.push_back(my_struct{rand()%20});
	for(int i = 0; i < N; ++i) Tree.add(&data[i]);
	for(int i = 0; i < N; ++i)
	{
		my_struct* leaf = Tree.find(data[i]);
		CHECK(Tree.find(data[i]) != nullptr);
	}
}
TEST_CASE("remove")
{
	int N = 10;
	std::vector<my_struct> data;
	lab618::CAVLTree<my_struct, CompareInt> Tree;
	for(int i = 0; i < N; ++i) data.push_back(my_struct{i});
	for(int i = 0; i < N; ++i) Tree.add(&data[i]);
	for(int i = 0; i < N; ++i)
	{
		CHECK(Tree.find(my_struct{i}) != nullptr);
		CHECK(Tree.remove(my_struct{i}) == true);
		CHECK(Tree.find(my_struct{i}) == nullptr);
	}
}
