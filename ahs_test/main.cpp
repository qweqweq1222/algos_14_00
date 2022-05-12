#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <iostream>
#include <chrono>
#include <fstream>
#include <cassert>
#include <vector>
#include <random>
#include <algorithm>
#include <string>
#include <set>
#include "sort.h"
#include "mm.h"
#include "hash.h"
#include "avltree.h"
#include <map>
#include <fstream>
std::random_device RANDOM_DEVICE;
std::mt19937 GENERATOR(RANDOM_DEVICE());
const std::string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
const int MAX_LENGTH = 20;
const int MIN_LENGTH = 7;
int HASH_SIZE = 5000;
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
int CompareStr(const std::string& lhs, const std::string& rhs) {
	int diff = lhs.size() - rhs.size();
	if (diff != 0) {
		return diff;
	}
	for (int i = 0; i < lhs.size(); ++i) {
		diff = lhs[i] - rhs[i];
		if (diff != 0)
			return diff;
	}
	return 0;
}
struct my_struct {
	std::string name;
	std::string last_name;
	bool operator< (const my_struct& other) const
	{
		int diff = CompareStr(name, other.name);
		if (diff != 0)
			return diff < 0;
		return CompareStr(last_name, other.last_name) < 0;
	}
};
unsigned int HashFunc (const my_struct* element)
{
	unsigned int hash = 5381;
	std::string str;
	for(int i = 0; i < (element->name.size() + element->last_name.size()); ++i)
	{
		str = element->name + element->last_name;
		hash = (hash<<5) + hash + str[i];
	}
	return hash;
}
int counter = 100;
int Compare(const my_struct* a, const my_struct* b) {
	int diff = CompareStr(a->name, b->name);
	if (diff != 0) {
		return diff;
	}
	return CompareStr(a->last_name, b->last_name);
}
std::string random_string()
{
	int length = (std::rand() % (MAX_LENGTH - MIN_LENGTH)) + MIN_LENGTH;
	std::uniform_int_distribution<> distribution(0, CHARACTERS.size() - 5);
	std::string random_string;
	for (std::size_t i = 0; i < length; ++i)
	{
		random_string += CHARACTERS[distribution(GENERATOR)];
	}
	return random_string;
}
void GenerateData(std::vector<my_struct>& elements, const int N)
{
	std::set<my_struct> set;
	for (int i = 0; i < N; ++i) {
		my_struct element;
		do {
			std::string x = random_string();
			std::string y = random_string();
			element = my_struct{x, y};
		} while (set.find(element) != set.end());
		set.insert(element);
		elements.push_back(element);
	}
}
bool HalfFind(my_struct** data,int len,  my_struct* key)
{
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
void Hash_Test(const int N,std::vector<my_struct>& data)
{
	std::ofstream file;
	file.open("/home/anreydron/TreeHashMassMapTests/data.csv",std::ios_base::app);

	lab618::CHash<my_struct, HashFunc, Compare> Hash(HASH_SIZE, BLOCK_SIZE);
	auto start = std::chrono::steady_clock::now();
	for(int i = 0; i < N; ++i)
		if(!Hash.add(&data[i]))
			throw std::invalid_argument( "add failed" );

	auto end = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
	file << "Hash," << N << "," << "add," << elapsed.count()/pow(10,9) <<"\n";
	start = std::chrono::steady_clock::now();
	for(int i = 0; i < N; ++i)
		if(!Hash.find(data[i]))
			throw std::invalid_argument( "find failed" );
	end = std::chrono::steady_clock::now();
	elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
	file << "Hash," << N << "," << "find," << elapsed.count()/pow(10,9) <<"\n";

	start = std::chrono::steady_clock::now();
	for(int i = 0; i < N; ++i)
		if(!Hash.remove(data[i]))
			throw std::invalid_argument( "remove failed" );
	end = std::chrono::steady_clock::now();
	elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
	file << "Hash," << N << "," << "remove," << elapsed.count()/pow(10,9) <<"\n";

	for(int i = 0; i < N; ++i)
		if(!Hash.add(&data[i]))
			throw std::invalid_argument( "clear add failed" );
	start = std::chrono::steady_clock::now();
	Hash.clear();
	end = std::chrono::steady_clock::now();
	elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
	file << "Hash," << N << "," << "clear," << elapsed.count()/pow(10,9) <<"\n";
	file.close();
}
void Tree_Test(const int N, std::vector<my_struct>& data)
{
	std::ofstream file;
	file.open("/home/anreydron/TreeHashMassMapTests/data.csv",std::ios_base::app);

	lab618::CAVLTree<my_struct, Compare> Hash;
	auto start = std::chrono::steady_clock::now();
	for(int i = 0; i < N; ++i)
		if(!Hash.add(&data[i]))
			throw std::invalid_argument( "add failed" );
	auto end = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
	file << "AVL," << N << "," << "add," << elapsed.count()/pow(10,9) <<"\n";
	start = std::chrono::steady_clock::now();
	for(int i = 0; i < N; ++i)
		if(!Hash.find(data[i]))
			throw std::invalid_argument( "find failed" );
	end = std::chrono::steady_clock::now();
	elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
	file << "AVL," << N << "," << "find," << elapsed.count()/pow(10,9) <<"\n";

	start = std::chrono::steady_clock::now();
	for(int i = 0; i < N; ++i)
		if(!Hash.remove(data[i]))
			throw std::invalid_argument( "remove failed" );
	end = std::chrono::steady_clock::now();
	elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
	file << "AVL," << N << "," << "remove," << elapsed.count()/pow(10,9) <<"\n";

	for(int i = 0; i < N; ++i)
		if(!Hash.add(&data[i]))
			throw std::invalid_argument( "clear failed" );
	start = std::chrono::steady_clock::now();
	Hash.clear();
	end = std::chrono::steady_clock::now();
	elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
	file << "AVL," << N << "," << "clear," << elapsed.count()/pow(10,9) <<"\n";
	file.close();
}
void Mass_Test(const int N, std::vector<my_struct> data)
{
	std::ofstream file;
	file.open("/home/anreydron/TreeHashMassMapTests/data.csv",std::ios_base::app);

	my_struct** mass = new my_struct* [data.size()];
	for(int i = 0; i < data.size(); ++i)
		mass[i] = &data[i];
	auto start = std::chrono::steady_clock::now();
	templates::mergeSort(mass, N, Compare);
	auto end = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
	file << "Mass," << N << "," << "add," << elapsed.count()/pow(10,9) <<"\n";

	start = std::chrono::steady_clock::now();
	for(int i = 0; i < N; ++i)
		if(!HalfFind(mass,N,mass[i]))
			throw std::invalid_argument( "add failed" );
	end = std::chrono::steady_clock::now();
	elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
	file << "Mass," << N << "," << "find," << elapsed.count()/pow(10,9) <<"\n";

	start = std::chrono::steady_clock::now();
	delete[] mass;
	end = std::chrono::steady_clock::now();
	elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
	file << "Mass," << N << "," << "clear," << elapsed.count()/pow(10,9) <<"\n";
	file.close();
}
void Map_Test(const int N, std::vector<my_struct> data)
{
	std::map<std::pair<std::string,std::string>,my_struct*> m;
	std::ofstream file;
	file.open("/home/anreydron/TreeHashMassMapTests/data.csv",std::ios_base::app);
	auto start = std::chrono::steady_clock::now();
	for(auto & el : data)
		m.insert(std::pair<std::pair<std::string,std::string>,my_struct*>(std::pair<std::string,std::string>(el.name,el.last_name),&el));
	auto end = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
	file << "Map," << N << "," << "add," << elapsed.count()/pow(10,9) <<"\n";
	start = std::chrono::steady_clock::now();
	for(auto & el : data)
		m.find(std::pair<std::string,std::string>(el.name,el.last_name));
	end = std::chrono::steady_clock::now();
	elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
	file << "Map," << N << "," << "find," << elapsed.count()/pow(10,9) <<"\n";
	file.close();
}
int main(void)
{
	std::ofstream file;
	file.open("/home/anreydron/TreeHashMassMapTests/data.csv",std::ios_base::app);
	file << "type,N,exp,time\n";
	file.close();
	for(int N = 100000; N <= 1000000; N+=100000)
	{
		std::cout << N << std::endl;
		std::vector<my_struct> elements;
		GenerateData(elements, N);
		Tree_Test(N, elements);
		Hash_Test(N, elements);
		Mass_Test(N, elements);
		Map_Test(N, elements);
	}
}
