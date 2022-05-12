#ifndef HASH_HEAD_H_2021_03_11
#define HASH_HEAD_H_2021_03_11

#include "mm.h"// необходимо использовать свой memorymanager
namespace lab618
{
	/**
	Шаблонный класс CHash.
	В качестве параметров шаблона применяется:
	тип - T;
	функция вычисления хеша - unsigned int HashFunc(const T* pElement);
	функция сравнения - int Compare(const T *pElement, const T* pElement2).

	 Класс реализует алгоритм контейнера Хеш-таблица (ассоциативный массив). Данный алгоритм базируется на идее формирования линейной адресации
	произвольных элементов в зависимости от специально вычисляемого целого значения, уникального для каждого элемента (хеш).
	 Формирование линейной адресации позволяет получать и сохранять элементы в контейнер за время не зависящее от числа элементов в контейнере — O(1).
	 В идеальном случае каждому хешу соответствует ячейка в линейном массиве данных. На самом деле подобрать функцию вычисления хеша, таким образом,
	чтобы соблюдалась уникальность сложно, или получаются числа очень большой размерности. Для преодоления данной проблемы, в Хеш-таблице допускаются
	коллизии — одинаковые значения, уникальность элементов определяется их полями (функциями сравнения).
	 Существует два способа разрешения коллизий — разреженная Хеш-таблица и Хеш-таблица со списками. Разреженные Хеш-таблицы при возникновении коллизии
	начинают искать первую свободную ячейку в самой таблице, поиск аналогично происходит от ячейки с номером равным значению хеша, пока не найдется нужный
	элемент. Хеш-таблица со списками, в каждой ячейке Хеш-таблицы хранит начало списка элементов с одинаковыми значениями хеша. Соответственно,
	вставка производится по алгоритму: вычисляем значение хеша для элемента, проверяем есть ли в списке с данным индексом (значение хеш функции)
	данный элемент с помощью функции сравнения, если нет до добавляем его в список. Поиск производится по алгоритму: вычисляем значение хеша для элемента,
	проверяем есть ли в списке с данным индексом искомый элемент с помощью функции сравнения.
	 Размер Хеш-таблицы, как правило, меньше размерности функции вычисления хеша, поэтому вычисленное значение хеша ограничивают по размеру таблицы
	с помощью операции вычисления остатка от деления.
	 Функция вычисления хеша и операция сравнения задаются как параметры шаблона, что позволяет построить разные Хеш-таблицы для одного набора элементов.
	 Основные недостатки Хеш-таблиц:
	  1. Сложность подбора хорошей хеш функции (функции дающей наименьшее число коллизий);
	  2. Отсутствие упорядоченности элементов;
	  3. Вычислительная сложность хеш функции может добавить большую константу в вычислительную сложность алгоритма.

	Данный класс реализует алгоритм Хеш-таблицы со списками
	Данный класс не хранит данные — хранит, только указатели на них.
	Хранение данных производится вне рамок данного класса!
	*/
	template <class T, unsigned int(*HashFunc)(const T* pElement), int(*Compare)(const T *pElement, const T* pElement2) >
	class CHash
	{
	private:
		/**
		Лист списка элементов. Обратите внимание, что хранится указатель на данные.
		Данный класс не хранит данные — хранит, только указатели на них.
		Хранение данных производится вне рамок данного класса!
		*/
		struct leaf
		{
			T* pData;
			leaf *pnext;
		};

	public:
		/**
		Исключение, которое применяется при нехватке памяти на работу алгоритма
		*/
		class CMemoryException
		{
		public:
			CMemoryException()
			{
			}
		};

	public:
		/**
		Конструктор с параметрами: размер Хеш-таблицы, и размер блока памяти для менеджера памяти.
		Размер Хеш таблицы реализуем жестко — изменение размера таблицы в зависимости от числа элементов в контейнере не требуется.
		Все создаваемые листики списков разрешения коллизий храним в менеджере памяти.
		*/
		CHash(int hashTableSize, int defaultBlockSize): m_tableSize(hashTableSize), m_Memory(defaultBlockSize, true), m_pTable(new leaf* [hashTableSize])
		{
			for(int i = 0; i < hashTableSize; ++i)
				m_pTable[i] = nullptr;
		}
		/**
		Деструктор. Должен освобождать всю выделенную память
		*/
		virtual ~CHash()
		{
			clear();
			delete [] m_pTable;
		}

		/**
		Функция добавления элемента в Хеш-таблицу. Возвращает false, если элемент уже есть и true, если элемент добавлен.
		*/
		bool add(T* pElement)
		{
			int idx = 0;

			if(findLeaf(pElement, idx) != nullptr) // элемент уже нашли
				return false;
			leaf* p = m_Memory.newObject();
			p->pnext = m_pTable[idx];
			p->pData = pElement;
			m_pTable[idx] = p;
			mass[idx]++;
			return true;
		}
		/**
		Функция обновления элемента в Хеш-таблице. Обновляет, если элемент уже есть добавляет, если элемента еще нет.
		Возвращает false, если был добавлен новый элемент, true если элемент обновлен.
		*/
		bool update(T* pElement)
		{
			int idx = 0;
			leaf* m_pCurrentLeaf = findLeaf(pElement, idx);
			if(m_pCurrentLeaf != nullptr)
			{
				m_pCurrentLeaf->pData = pElement;
				return true;
			}

			leaf* p = m_Memory.newObject();
			p->pnext = m_pTable[idx];
			p->pData = pElement;
			m_pTable[idx] = p;
			return false;
		}
		void CheckDev()
		{
			for(int i = 0; i < 5000; ++i)
				std::cout << mass[i] << std::endl;
		}
		/**
		Функция поиска элемента в Хеш-таблице. Возвращает указатель на данные. Если элемента не нашлось, то null.
		Обратите внимание, что для поиска используется частично заполненный объект, т.е. В нем должны быть заполнены поля на основе которых рассчитывается хеш.*/
		T* find(const T& element)
		{
			int idx = 0;
			const T* el = &element;
			leaf *m_pCurrentLeaf = findLeaf(el,idx);
			if(m_pCurrentLeaf == nullptr)
				return nullptr;
			return m_pCurrentLeaf->pData;
		}

		/**
		Функция удаления элемента из Хеш-таблицы. Возвращает false, если не нашлось элемента, true если элемент был удален.
		*/
		void print()
		{
			leaf* runner = nullptr;
			for(int i = 0; i < m_tableSize; ++i) {
				runner = m_pTable[i];
				while (runner != nullptr)
				{
					std::cout << *(runner->pData) << " ";
					runner = runner->pnext;
				}
				std::cout << std::endl;
			}
		}

		bool remove(const T& element)
		{
			unsigned int idx = HashFunc(&element) % m_tableSize;
			leaf* tmpLeaf = m_pTable[idx];
			leaf* preTmpLeaf = nullptr;
			while (tmpLeaf) {
				if (Compare(tmpLeaf->pData, &element) == 0) {
					break;
				}
				preTmpLeaf = tmpLeaf;
				tmpLeaf = tmpLeaf->pnext;
			}
			if (!tmpLeaf) {
				return false;
			}
			if (preTmpLeaf) {
				preTmpLeaf->pnext = tmpLeaf->pnext;
			} else {
				m_pTable[idx] = tmpLeaf->pnext;
			}
			m_Memory.deleteObject(tmpLeaf);
			return true;
		}

		std::vector<int> Test()
		{
			std::vector<int> answer(m_tableSize);
			for(int i = 0; i < m_tableSize; ++i)
			{
				if (m_pTable[i] == nullptr)
					answer[i] = 0;
				else
				{
					int counter = 0;
					leaf* runner = m_pTable[i];
					while (runner != nullptr)
					{
						++counter;
						runner = runner->pnext;
					}
					answer[i] = counter;
				}
			}
			return answer;
		}

		/**
		Удаление всех элементов. Можно вызвать в деструкторе
		*/
		void clear()
		{
			leaf* m_pCurrentLeaf = nullptr;
			leaf* m_pNext = nullptr;

			for(int i = 0; i < m_tableSize; ++i)
			{
				m_pCurrentLeaf = m_pTable[i];
				while(m_pCurrentLeaf != nullptr)
				{
					m_pNext = m_pCurrentLeaf->pnext;
					m_pCurrentLeaf->pnext = nullptr;
					m_pCurrentLeaf->pData = nullptr;
					m_pCurrentLeaf = m_pNext;
				}
				delete [] m_pCurrentLeaf;
				m_pTable[i] = nullptr;
			}
			m_Memory.clear();
		}
	private:
		/**

		Элементарная функция поиска узла в Хеш-таблицу. Возвращает найденный узел и в переменную idx выставляет актуальный индекс хеш-таблицы.
		Данную функцию следует использовать в функциях add, update, find.
		Алгоритм функции:
		 1. вычисляем хеш функцию
		 2. вычисляем индекс в Хеш-таблице (приводим вычисленное значение хеш функции к размеру массива)
		 3. Перебираем список значений и если нашли, то возвращаем его.
		 4. Если ничего не нашли, то возвращаем null
		*/
		leaf *findLeaf(const T* pElement, int& idx)
		{
			idx = HashFunc(pElement) % m_tableSize;
			leaf* m_pCurrentElem = m_pTable[idx];

			if(m_pCurrentElem == nullptr)
				return nullptr;

			while(m_pCurrentElem != nullptr)
			{
				if(Compare(m_pCurrentElem->pData, pElement) == 0)
					return m_pCurrentElem;
				m_pCurrentElem = m_pCurrentElem->pnext;
			}
			return nullptr;
		}

		/**
		Размер Хеш-таблицы
		*/
		int m_tableSize;
		/**
		Хеш-таблица
		*/
		leaf** m_pTable;
		/**
		Менеджер памяти, предназначен для хранение листов списков разрешения коллизий
		*/
		int mass[5000] = {0};
		CMemoryManager<leaf> m_Memory;
	};
}; // namespace templates

#endif // #define HASH_HEAD_H_2021_03_11