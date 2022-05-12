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
		CMemoryManager(int _default_block_size = 1000, bool isDeleteElementsOnDestruct = false): m_blkSize(_default_block_size),
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
			block* tmpBlock = m_pBlocks;
			while (tmpBlock)
			{
				int diff = p - tmpBlock->pdata;
				if (0 <= diff && diff < m_blkSize)
				{
					p->~T();
					memset(reinterpret_cast<void*>(p), 0, sizeof(T));
					int* pi = reinterpret_cast<int*>(p);
					*pi = tmpBlock->firstFreeIndex;
					tmpBlock->firstFreeIndex = diff;
					tmpBlock->usedCount -= 1;
					return true;
				}
				tmpBlock = tmpBlock->pnext;
			}
			return false;
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