#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include<iostream>
#include<vector>
#include "sort.h"
#include "doctest.h"

void templates::compareSort(void** ppArray, int length, CompareSortType* pCompareFunc)
{
	void *buffer;
	for(int i = 0; i < length - 1; ++i)
	{
		for (int j = 0; j < length - i -1; ++j)
			if (pCompareFunc(ppArray[j], ppArray[j + 1]) == -1)
			{

				buffer = ppArray[j];
				ppArray[j] = ppArray[j+1];
				ppArray[j+1] = buffer;
			}
	}
}

void templates::mergeSortCardinal(void **ppArray, void **buffer, int length, CompareSortType pCompareFunc)
{
	// добавить функцию выделения памяти одним большим куском
	if(length > 1)
	{
		int left_size = length / 2;
		int right_size = length - left_size;

		mergeSortCardinal(ppArray,buffer, left_size, pCompareFunc);
		mergeSortCardinal(ppArray + left_size,buffer + left_size, right_size, pCompareFunc);

		int lidx = 0;
		int ridx = left_size;
		int idx = 0;

		while(lidx < left_size || ridx < length)
		{

			if (pCompareFunc(ppArray[lidx], ppArray[ridx]) == -1)
			{
				buffer[idx++] = ppArray[lidx];
				lidx++;
			}
			else
			{
				buffer[idx++] = ppArray[ridx];
				ridx++;
			}
			if (lidx == left_size)
			{
				for (int i = ridx; i < length; ++i)
					buffer[idx++] = ppArray[i];
				break;
			}
			if (ridx == length)
			{
				for (int i = lidx; i < left_size; ++i)
					buffer[idx++] = ppArray[i];
				break;
			}
		}
		for(int i = 0; i < length; ++i)
			ppArray[i] = buffer[i];
	}
}

void templates::mergeSort(void **ppArray, int length, CompareSortType pCompareFunc)
{
	// добавить функцию выделения памяти одним большим куском
	void **buffer = new void *[length];
	mergeSortCardinal(ppArray, buffer, length, pCompareFunc);
}
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
void Print(my_struct** mass, const int counter)
{
	for(int i = 0; i < counter; ++i)
		std::cout << "name: " << mass[i]->name << " " << "data: " << mass[i]->data << std::endl;
	std::cout << "_______________________________________________________________________________________" << std::endl;
}

TEST_CASE("checker")
{
	my_struct** mass = generate_data(counter);
	Print(mass,counter);
	templates::mergeSort(mass, counter, Compare);
	bool check_;
	for(int i = 0; i < counter - 1; ++i)
	{
		check_ = Compare(mass[i], mass[i+1]) == -1 || Compare(mass[i], mass[i+1]) == 0;
		CHECK(check_ == true);
	}
	Print(mass,counter);
}