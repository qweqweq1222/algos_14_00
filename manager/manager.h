#ifndef MEMORY_MANAGER_HEAD_H_2022_02_17
#define MEMORY_MANAGER_HEAD_H_2022_02_17
#include <iostream>
#include <cstring>

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
            // Число заполненных ячеек
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
            this->clear();
        }

        // Получить адрес нового элемента из менеджера
        T* newObject()
        {
            int index = 0;
            if(m_pBlocks == nullptr)
            {
                m_pBlocks = newBlock();
                m_pCurrentBlk = m_pBlocks;
                index = m_pCurrentBlk->firstFreeIndex;
                m_pCurrentBlk->firstFreeIndex = *((int *) (m_pCurrentBlk->pdata + m_pCurrentBlk->firstFreeIndex));
            }
            else
            {
                index = m_pCurrentBlk->firstFreeIndex;
                if (index != -1)
                    m_pCurrentBlk->firstFreeIndex = *((int *) (m_pCurrentBlk->pdata + m_pCurrentBlk->firstFreeIndex));
                else // если в текущем блоке нету места
                {
                    block* buffer = m_pBlocks;
                    while(buffer->pnext != nullptr)
                    {
                        if(index == -1)
                            buffer = buffer->pnext;
                        else
                        {
                            buffer->firstFreeIndex = *((int *) (buffer->pdata + buffer->firstFreeIndex));
                            m_pCurrentBlk = buffer;
                            ConstructElements(m_pCurrentBlk->pdata + index);
                            return (m_pCurrentBlk->pdata + index);
                        }
                        index = buffer->firstFreeIndex;
                    }
                    if(index = -1)
                    {
                        // во всех блоках нету места
                        buffer->pnext = newBlock();
                        m_pCurrentBlk = buffer->pnext;
                        index = 0;
                    }
                }
            }
            ConstructElements(m_pCurrentBlk->pdata + index);
            return (m_pCurrentBlk->pdata + index);
        }

        // Освободить элемент в менеджере
        bool deleteObject(T* p)
        {
        }

        // Очистка данных, зависит от m_isDeleteElementsOnDestruct
        void clear()
        {
            T* buffer = m_pBlocks->pdata;
            bool is = (sizeof(T) == sizeof(*(m_pBlocks->pdata)));
            int s = sizeof(T);
            int b = sizeof(m_pBlocks->pdata);
            block* buffer_ = nullptr;
            m_pCurrentBlk = nullptr;
            while(m_pBlocks != nullptr)
            {
                while(m_pBlocks->pdata != nullptr)
                {
                    buffer = m_pBlocks->pdata;
                    ++(m_pBlocks->pdata);
                    delete buffer;
                }
                buffer_ = m_pBlocks;
                m_pBlocks = m_pBlocks->pnext;
                delete buffer_;
            }
        }
    private:

        // Создать новый блок данных. применяется в newObject
        block* newBlock()
        {
            block* new_block = new block;
            new_block->firstFreeIndex = 0;
            new_block->pnext = nullptr;
            new_block->pdata = (T *)new char[sizeof(T)*m_blkSize];
            T* buffer = new_block->pdata;
            for(int i = 0; i < m_blkSize; ++i)
            {
                if(i == m_blkSize - 1)
                    *((int*)buffer) = -1;
                else
                    *((int*)buffer) = i + 1;
                ++buffer;
            }
            return new_block;
        }

        // Освободить память блока данных. Применяется в clear
        void deleteBlock(block *p)
        {
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

#endif // #define MEMORY_MANAGER_HEAD_H_2022_02_17