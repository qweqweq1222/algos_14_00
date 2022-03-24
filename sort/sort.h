#ifndef SORT_HEAD_H_2022_03_17
#define SORT_HEAD_H_2022_03_17

namespace templates
{
	/** Определение типа функции сравнения элементов
	\return
	\li > 0  - pElem1 меньше pElem2
	\li 0   - элементы pElem1 и pElem2 равны
	\li < 0 - pElem1 больше pElem2
	*/
	typedef  int (CompareSortType)(const void *pElem1, const void *pElem2);
	void compareSort(void **ppArray, int length, CompareSortType pCompareFunc)
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
	void mergeSort(void **ppArray, int length, CompareSortType pCompareFunc)
	{
		if(length > 1)
		{
			int left_size = length / 2;
			int right_size = length - left_size;

			mergeSort(ppArray,left_size, pCompareFunc);
			mergeSort(ppArray + left_size, right_size, pCompareFunc);

			int lidx = 0;
			int ridx = left_size;
			int idx = 0;
			void* tmp_array[length];

			while(lidx < left_size || ridx < length)
			{

					if (pCompareFunc(ppArray[lidx], ppArray[ridx]) == -1)
					{
						tmp_array[idx++] = ppArray[lidx];
						lidx++;
					}
					else
					{
						tmp_array[idx++] = ppArray[ridx];
						ridx++;
					}
					if (lidx == left_size)
					{
						for (int i = ridx; i < length; ++i)
							tmp_array[idx++] = ppArray[i];
						break;
					}
					if (ridx == length)
					{
						for (int i = lidx; i < left_size; ++i)
							tmp_array[idx++] = ppArray[i];
						break;
					}
			}
			for(int i = 0; i < length; ++i)
				ppArray[i] = tmp_array[i];
		}
	}
	void heapSort(void **ppArray, int length, CompareSortType pCompareFunc);

	template <class T>
	void compareSort(T **ppArray, int length, int (pCompareFunc)(const T *pElem1, const T *pElem2))
	{
		compareSort(reinterpret_cast<void**>(ppArray), length, (CompareSortType*)pCompareFunc);
	}

	template <class T>
	void mergeSort(T **ppArray, int length, int (pCompareFunc)(const T *pElem1, const T *pElem2))
	{
		mergeSort(reinterpret_cast<void**>(ppArray), length, (CompareSortType*)pCompareFunc);
	}
	template <class T>
	void heapSort(T **ppArray, int length, int (pCompareFunc)(const T *pElem1, const T *pElem2))
	{
		heapSort(reinterpret_cast<void**>(ppArray), length, (CompareSortType*)pCompareFunc);
	}
}; // namespace templates

#endif // #define SORT_HEAD_H_2021_02_25