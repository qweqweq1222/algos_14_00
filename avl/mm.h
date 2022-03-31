#ifndef MEMORY_MANAGER_HEAD_H_2022_02_17
#define MEMORY_MANAGER_HEAD_H_2022_02_17
#include <iostream>
#include <cstring>
#include <vector>
#include <exception>

template <class TYPE>
inline void ConstructElements(TYPE* pElements)
{
	memset(reinterpret_cast<void*>(pElements), 0, sizeof(TYPE));
	::new(reinterpret_cast<void*>(pElements)) TYPE;
}

template<class TYPE>
inline void DestructElements(TYPE* pElements)
{
	pElements->~TYPE();
	memset(reinterpret_cast<void*>(pElements), 0, sizeof(TYPE));
}

namespace lab618
{
	template <class T>
	class CMemoryManager
	{
	private:
		struct block
		{
			// Массив данных блока
			T* pdata;
			// Адрес следующего блока
			block *pnext;
			// Первая свободная ячейка
			int firstFreeIndex;
			// Число заполненных ячее
			int usedCount;
		};
	public:
		class CException
		{
		public:
			CException()
			{
			}
		};

	public:
		CMemoryManager(int _default_block_size, bool isDeleteElementsOnDestruct = false): m_blkSize(_default_block_size),
																						  m_isDeleteElementsOnDestruct(isDeleteElementsOnDestruct)
		{
			m_pBlocks = nullptr;
			m_pCurrentBlk = nullptr;
		}

		virtual ~CMemoryManager()
		{
			clear();
		}

		// Получить адрес нового элемента из менеджера
		T* newObject()
		{
			block *pThisBlk = m_pCurrentBlk;
			int index = 0;
			if(m_pBlocks == nullptr) // создаем первый блок
			{
				m_pBlocks = newBlock();
				m_pCurrentBlk = m_pBlocks;
				m_pCurrentBlk->firstFreeIndex = 1;
				ConstructElements(m_pCurrentBlk->pdata);
				++(m_pCurrentBlk->usedCount);
				return (m_pCurrentBlk->pdata);
			}
			else
			{
				if (m_pCurrentBlk->firstFreeIndex != -1) //повезло - в текущем сразу нашлось место
				{
					index = pThisBlk->firstFreeIndex;
					m_pCurrentBlk->firstFreeIndex = *(reinterpret_cast<int*>(m_pCurrentBlk->pdata + m_pCurrentBlk->firstFreeIndex));
					ConstructElements(pThisBlk->pdata + index);
					++(pThisBlk->usedCount);
					return (pThisBlk->pdata + index);
				}
				else // надо искать новый блок (если он есть) или создавать новый
				{
					block* tLastBlk = nullptr;
					while (pThisBlk != nullptr) //проверяем все блоки до последнего
					{
						if (pThisBlk->firstFreeIndex != -1) // нашли свободное место
						{
							index = pThisBlk->firstFreeIndex;
							m_pCurrentBlk = pThisBlk;
							m_pCurrentBlk->firstFreeIndex = *(reinterpret_cast<int*>(pThisBlk->pdata + pThisBlk->firstFreeIndex));
							ConstructElements(pThisBlk->pdata + index);
							++(pThisBlk->usedCount);
							return (pThisBlk->pdata + index);
						}
						else
						{
							tLastBlk = pThisBlk;
							pThisBlk = pThisBlk->pnext;
						}

					}
					//дошли до последнего блока // не нашли место и в последнем блоке - создаем блок
					tLastBlk->pnext = newBlock();
					m_pCurrentBlk = tLastBlk->pnext;
					m_pCurrentBlk->firstFreeIndex = 1;
					ConstructElements(m_pCurrentBlk->pdata);
					++(m_pCurrentBlk->usedCount);
					return (m_pCurrentBlk->pdata);
				}
			}
		}

		// Освободить элемент в менеджере
		bool deleteObject(T* p)
		{
			block* pThisBlk = m_pBlocks;
			T* pTmpElement = nullptr;
			bool done = false;
			int index = 0;
			while(!done && pThisBlk != nullptr) // ищим где элемент
			{
				done = (pThisBlk->pdata <= p) && (p <= (pThisBlk->pdata + (m_blkSize - 1)));
				if(done)
				{
					pTmpElement = pThisBlk->pdata;
					while(pTmpElement != p)
					{
						++pTmpElement;
						++index;
					}
				}
				if(!done)
					pThisBlk = pThisBlk->pnext;
			}
			if(done == false) //пытаемся удалить несуществующий в менеджере элемент
				return false;
			else
			{
				DestructElements(pTmpElement);
				--(pThisBlk->usedCount);

				/* определяем что положить в качестве позиции следующего на выделенное место*/
				if (index <= pThisBlk->firstFreeIndex && pThisBlk->firstFreeIndex != -1) // удаляемый до первого свободного и место было
				{
					*(reinterpret_cast<int*>(pTmpElement)) = pThisBlk->firstFreeIndex;
					pThisBlk->firstFreeIndex = index;
				}
				else if(pThisBlk->firstFreeIndex == -1) // не было места до этого момента
				{
					*(reinterpret_cast<int*>(pTmpElement)) = -1;
					pThisBlk->firstFreeIndex = index;
				}
				else // удаляемый после первого свободного и место есть
				{
					T* ptrPst = pThisBlk->pdata + pThisBlk->firstFreeIndex;
					int idx = *(reinterpret_cast<int *>(ptrPst));
					if(*(reinterpret_cast<int *>(ptrPst)) == -1)
					{
						*(reinterpret_cast<int *>(pTmpElement)) = -1;
						*(reinterpret_cast<int *>(ptrPst)) = index;
					}
					else
					{
						T* pre_ptr  = ptrPst;
						while(pTmpElement > ptrPst)
						{
							pre_ptr = ptrPst;
							ptrPst = pThisBlk->pdata + idx;
							idx = *(reinterpret_cast<int *>(ptrPst));
							if(idx == -1 && pTmpElement > ptrPst)
							{
								*(reinterpret_cast<int *>(pTmpElement)) = -1;
								*(reinterpret_cast<int *>(ptrPst)) = index;
								return true;
							}
						}
						*(reinterpret_cast<int *>(pTmpElement)) = *(reinterpret_cast<int *>(pre_ptr));
						*(reinterpret_cast<int *>(pre_ptr)) = index;
					}
				}
			}
			return true;
		}

		// Очистка данных, зависит от m_isDeleteElementsOnDestruct
		void clear()
		{
			block* pThisBlk = m_pBlocks;
			if(!m_isDeleteElementsOnDestruct)
				while (pThisBlk != nullptr)
				{
					if (pThisBlk->usedCount != 0)
						throw new CException();
					pThisBlk = pThisBlk->pnext;
				}

			pThisBlk = m_pBlocks;
			while(pThisBlk != nullptr)
			{
				m_pBlocks = m_pBlocks->pnext;
				m_pCurrentBlk = m_pBlocks;
				deleteBlock(pThisBlk);
				pThisBlk = m_pBlocks;
			}
		}

	private:

		// Создать новый блок данных. применяется в newObject
		block* newBlock()
		{
			block* new_block = new block;
			new_block->firstFreeIndex = 0;
			new_block->pnext = nullptr;
			new_block->usedCount = 0;
			new_block->pdata = reinterpret_cast<T*>(new char[sizeof(T)*m_blkSize]);
			T* buffer = new_block->pdata;
			for(int i = 0; i < m_blkSize; ++i) // заполняем int
			{
				if(i == m_blkSize - 1)
					*(reinterpret_cast<int*>(buffer)) = -1;
				else
					*(reinterpret_cast<int*>(buffer)) = i + 1;
				++buffer;
			}
			std::cout << std::endl;
			return new_block;
		}

		// Освободить память блока данных. Применяется в clear
		void deleteBlock(block *p) // т.к. применятся в clear, который вызываем когда удаляем все, то занулять pnext не надо т.к. удаляем голову
		{
			T *pTmpElement = p->pdata;
			int index = 0;
			int curFirst = p->firstFreeIndex;
			while(index < m_blkSize && curFirst != -1)
			{
				if(index < curFirst)
					DestructElements(pTmpElement);
				else
					curFirst = *(reinterpret_cast<int*>(p->pdata + curFirst));
				++index;
			}
			delete[] reinterpret_cast<char*> (p);
		}

		// Размер блока
		int m_blkSize;
		// Начало списка блоков
		block* m_pBlocks;
		// Текущий блок
		block *m_pCurrentBlk;
		// Удалять ли элементы при освобождении
		bool m_isDeleteElementsOnDestruct;
	};
}; // namespace lab618

#endif // #define MEMORY_MANAGER_HEAD_H_2022_02_17`