#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include<iostream>
#include<vector>
#include <chrono>
#include "sort.h"
#include "mm.h"
#include "hash.h"
#include "avltree.h"


int HASH_SIZE = 1000;
int BLOCK_SIZE = 1000;
void templates::compareSort(void** ppArray, int length, CompareSortType* pCompareFunc)
{
	void *buffer;
	for(int i = 0; i < length - 1; ++i)
	{
		for (int j = 0; j < length - i -1; ++j)
		{
			if (pCompareFunc(ppArray[j], ppArray[j + 1]) < 0)
			{
				buffer = ppArray[j];
				ppArray[j] = ppArray[j + 1];
				ppArray[j + 1] = buffer;
			}
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

		while(lidx < left_size && ridx < length)
		{

			if (pCompareFunc(ppArray[lidx], ppArray[ridx]) < 0)
			{
				buffer[idx++] = ppArray[lidx];
				lidx++;
			}
			else
			{
				buffer[idx++] = ppArray[ridx];
				ridx++;
			}
		}
		for (ridx ;ridx < length; ++ridx)
		{
			buffer[idx++] = ppArray[ridx];
		}
		for (lidx; lidx < left_size; ++lidx)
		{
			buffer[idx++] = ppArray[lidx];
		}
		for(int i = 0; i < length; ++i)
		{
			ppArray[i] = buffer[i];
		}
	}
}

void templates::mergeSort(void **ppArray, int length, CompareSortType pCompareFunc)
{
	// добавить функцию выделения памяти одним большим куском
	void **buffer = new void *[length];
	mergeSortCardinal(ppArray, buffer, length, pCompareFunc);
	delete [] buffer;
}
struct my_struct {
	std::string name;
	std::string last_name;
	int data;
	int operator ==(const my_struct& left) const
	{
		if(this->last_name == left.last_name && this->name == left.name && this->data == left.data)
			return 0;
		else if (this->data <= left.data )
			return -1;
		return 1;
	}
};

unsigned int HashFunc (const my_struct* element)
{
	const std::string* str = &(element->name);
	unsigned int p = 1;
	unsigned int sum = 0;
	for (int i = 0; i < str->size(); ++i) {
		sum += p * (str->at(i) - '0');
		p *= 17;
	}
	str = &(element->last_name);
	for (int i = 0; i < str->size(); ++i) {
		sum += p * (str->at(i) - '0');
		p *= 17;
	}
	return sum;
}

int counter = 100;
int Compare(const my_struct* left,const my_struct* right)
{
	if(left->last_name == right->last_name && left->name == right->name && left->data == right->data)
		return 0;
	else if (left->data <= right->data )
		return -1;
	return 1;
}

std::string FillString(char symbol, const int len)
{
	char str[len];
	for(int i = 0; i < len; ++i)
		str[i] = symbol;
	std::string str_(str);
	return str_;
}
my_struct** GenerateData()
{
	my_struct** data = new my_struct* [1560384];
	char symbols[200]  = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPRQRSTUVWXYZ0123456789абвгдеёжзийклмнопрстуфхцчшщъьэюяЫАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЬЭЮЯЫ";
	int k = 0;
	for(int size = 7; size <= 20; ++size)
	{
		for (int m = 0; m < 128; ++m)
		{
			std::string str = FillString(symbols[m], size);
			for (int i = 0; i < size; ++i)
				for (int j = m+1; j < 129; ++j)
				{
					str[i] = symbols[j];
					data[k] = new my_struct{ str, str, k };
					++k;
				}
		}
	}
	for(int i = 0; i < 1000000; ++i)
		std::swap(data[rand()%1560384], data[rand()%1560384]);
	return data;
}
bool HalfFind(my_struct** data,int len,  my_struct* key)
{
	/*my_struct** thi = data + len/2;
	if(len == 1 && Compare(data[0], key) != 0)
		return false;
	if(Compare(data[len/2],key) > 0)
		HalfFind(data, len/2, key);
	else if(Compare(data[len/2],key) < 0)
		HalfFind(data + len/2, len - len/2, key);
	else
		return true;*/
	int l = 0;
	int r = len - 1;
	bool flag = false;
	int mid;
	while ((l <= r) && !flag) {
		mid = (l + r) / 2; // считываем срединный индекс отрезка [l,r]
		if (Compare(data[mid],key) == 0) flag = true;
		if (Compare(data[mid],key) > 0) r = mid - 1;
		else l = mid + 1;
	}
	return flag;
}
int main(void)
{
	int SIZE = 1000000;
	// генерация данных
	// создание объектов
	lab618::CHash<my_struct, HashFunc, Compare> Hash(HASH_SIZE, BLOCK_SIZE);
	lab618::CAVLTree<my_struct, Compare> Tree;
	// тест на добавлениe
	std::cout << "ADD TEST\n";
	std::cout << "Mass\n\n";
	for(int N = 0; N <= SIZE; N += 100000)
	{
		my_struct** copy = GenerateData();
		auto start = std::chrono::steady_clock::now();
		templates::mergeSort(copy, N, Compare);
		auto end = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
		std::cout << elapsed.count() << "\n";
	}
	std::cout << "\nHash\n";
	my_struct** copy = GenerateData();
	int number_of_added = 0;
	for(int N = 0; N < SIZE; N += 100000)
	{
		auto start = std::chrono::steady_clock::now();
		for (int i = 0; i < 100000; ++i)
		{
			Hash.add(copy[N + i]);
			++number_of_added;
		}
		auto end = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
		std::cout << elapsed.count()<< "\n";
	}
	std::cout << "Hash added: " << number_of_added  << "\n";
	number_of_added = 0;
	std::cout << "\nAVL\n";
	for(int N = 0; N < SIZE; N += 100000)
	{
		auto start = std::chrono::steady_clock::now();
		for (int i = 0; i < 100000; ++i)
			if(Tree.add(copy[N + i]))
				++number_of_added;
		auto end = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
		std::cout <<  elapsed.count() << "\n";
	}
	std::cout << "Tree added : " << number_of_added << std::endl;
	std::cout << "\n FIND TEST\n";
	std::cout << "Mass\n\n";
	number_of_added = 0;
	for(int N = 100000; N < SIZE; N += 100000)
	{
		my_struct** copy_ = GenerateData();
		templates::mergeSort(copy_, N, Compare);
		auto start = std::chrono::steady_clock::now();
		for(int i = 0; i < N; ++i)
			if(HalfFind(copy_,N,copy_[i]))
				++number_of_added;
		auto end = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
		std::cout << elapsed.count() << "\n";
	}
	std::cout << "MAss finded: " << number_of_added << "\n";
	number_of_added = 0;
	std::cout << "\nTree\n";
	for(int N = 100000; N < SIZE; N += 100000)
	{
		auto start = std::chrono::steady_clock::now();
		for (int i = 0; i < N; ++i)
			if(Tree.find(*copy[i]))
				++number_of_added;
		auto end = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
		std::cout << elapsed.count() << "\n";
	}
	std::cout << "Tree finded:" << number_of_added << "\n";
	number_of_added = 0;
	std::cout << "\nHash\n";
	for(int N = 100000; N < SIZE; N += 100000)
	{
		auto start = std::chrono::steady_clock::now();
		for (int i = 0; i < N; ++i)
			if(Hash.find(*copy[i]))
				++number_of_added;
		auto end = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
		std::cout << elapsed.count()<< "\n";
	}
	std::cout << "Hash finded:" << number_of_added << "\n";
	std::cout << "\n Delete TEST\n";
	std::cout << "Mass\n\n";
	for(int N = 0; N <= SIZE; N += 100000)
	{
		my_struct** copy_ = GenerateData();
		auto start = std::chrono::steady_clock::now();
		for(int i = 0; i < N; ++i)
			delete copy_[i];
		auto end = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
		std::cout << elapsed.count()<< "\n";
	}
	std::cout << "\nTree clear\n";
	for(int N = 0; N <= SIZE; N += 100000)
	{
		for (int i = 0; i < N; ++i)
			Tree.add(copy[i]);
		auto start = std::chrono::steady_clock::now();
		Tree.clear();
		auto end = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
		std::cout << elapsed.count() << "\n";
	}
	std::cout << "\nHASH clear\n";
	for(int N = 0; N <= SIZE; N += 100000)
	{
		for (int i = 0; i < N; ++i)
			Hash.add(copy[i]);
		auto start = std::chrono::steady_clock::now();
		Hash.clear();
		auto end = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
		std::cout  << elapsed.count() << "\n";
	}
	std::cout << "\nTree delete by element\n";
	number_of_added = 0;
	my_struct** copy_ = GenerateData();
	for (int i = 0; i < SIZE; ++i)
		Tree.add(copy_[i]);
	for (int N = 0; N < SIZE; N += 100000)
	{
		auto start = std::chrono::steady_clock::now();
		for (int i = N; i < N+100000; ++i)
			if(Tree.remove(*copy_[i]))
				++number_of_added;
		auto end = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
		std::cout  << elapsed.count() << "\n";
	}
	std::cout << "number of deleted Tree :" << number_of_added << std::endl;
	std::cout << "\nHash delete by element\n";
	number_of_added = 0;
	for (int i = 0; i < SIZE; ++i) // 1000000
		Hash.add(copy_[i]);
	for (int N = 0; N < SIZE; N += 100000)
	{
		auto start = std::chrono::steady_clock::now();
		for (int i = N; i < N + 100000; ++i)
			if(Hash.remove(*copy_[i]))
				++number_of_added;
		auto end = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
		std::cout << elapsed.count() << "\n";
	}
	std::cout << "Number of deleted Hash:" << number_of_added << std::endl;
	for(int i = 0; i < SIZE; ++i)
		delete copy_[i];
	return 0;
}
