#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include<iostream>
#include<vector>
#include "sort.h"
#include "doctest.h"

struct my_struct {
	std::string name;
	int data;
};

int counter = 100;
int Compare(const my_struct* left,const my_struct* right)
{
	if(left->data <= right->data)
		return -1;
	else if(left->data == right->data)
		return 0;
	return 1;
}

std::string GenerateString(const int length)
{
	char symbols[27]  = "abcdefghijklmnopqrstuvwxyz";
	char str[length];
	for(int i = 0; i < length; ++i)
		str[i] = symbols[rand()%27];
	std::string str_(str);
	return str_;
}

my_struct** generate_data(const int N)
{
	my_struct** mass = new my_struct* [N];
	for(int i = 0; i < N; ++i)
		mass[i] = new my_struct{ GenerateString(i),rand()% 100};
	return mass;
}

TEST_CASE("checker")
{
	my_struct** mass = generate_data(counter);
	templates::mergeSort(mass, counter, Compare);
	bool check_;
	for(int i = 0; i < counter - 1; ++i)
	{
		check_ = Compare(mass[i], mass[i+1]) == -1 || Compare(mass[i], mass[i+1]) == 0;
		CHECK(check_ == true);
	}
}