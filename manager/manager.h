#ifndef MEMORY_MANAGER_HEAD_H_2022_02_17
#define MEMORY_MANAGER_HEAD_H_2022_02_17
#include <iostream>
#include <cstring>
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
        };
    public:
        class CException
        {
        public:
            CException()
            {
                throw std::out_of_range("threw something");
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
            block *runner = m_pCurrentBlk;
            if(m_pBlocks == nullptr)
            {
                m_pBlocks = newBlock();
                m_pCurrentBlk = m_pBlocks;
                m_pCurrentBlk->firstFreeIndex = *((int *) m_pCurrentBlk->pdata);
                ConstructElements(m_pCurrentBlk->pdata);
                return (m_pCurrentBlk->pdata);
            }
            else
            {
                if (m_pCurrentBlk->firstFreeIndex != -1) //повезло - в текущем сразу нашлось место
                {
                    m_pCurrentBlk->firstFreeIndex = *((int *)(m_pCurrentBlk->pdata + m_pCurrentBlk->firstFreeIndex));
                    ConstructElements(runner->pdata + runner->firstFreeIndex);
                    return (runner->pdata + runner->firstFreeIndex);
                } else // надо искать новый блок (если он есть) или создавать новый
                {
                    while (runner->pnext != nullptr) //проверяем все блоки до последнего
                    {
                        if (runner->firstFreeIndex != -1) // нашли свободное место
                        {
                            m_pCurrentBlk = runner;
                            m_pCurrentBlk->firstFreeIndex = *((int *) runner->pdata + runner->firstFreeIndex);
                            ConstructElements(runner->pdata + runner->firstFreeIndex);
                            return (runner->pdata + runner->firstFreeIndex);
                        } else
                            runner = runner->pnext;
                    }
                    //дошли до последнего блока
                    if (runner->firstFreeIndex != -1) // нашли место в последнем блоке
                    {
                        m_pCurrentBlk = runner;
                        m_pCurrentBlk->firstFreeIndex = *((int *) runner->pdata + runner->firstFreeIndex);
                        ConstructElements(runner->pdata + runner->firstFreeIndex);
                        return (runner->pdata + runner->firstFreeIndex);
                    } else // не нашли место и в последнем блоке - создаем блок
                    {
                        runner->pnext = newBlock();
                        m_pCurrentBlk = runner->pnext;
                        m_pCurrentBlk->firstFreeIndex = 1;
                        ConstructElements(m_pCurrentBlk->pdata);
                        return (m_pCurrentBlk->pdata);
                    }
                }
            }
        }

        // Освободить элемент в менеджере
        bool deleteObject(T* p)
        {
            block* buffer = m_pBlocks;
            T* runner = buffer->pdata;
            bool done = false;
            int index = 0;
            while(!done || buffer == nullptr) // ищим где элемент
            {
                runner = buffer->pdata;
                while(!done || runner == nullptr)
                {
                    if(runner == p)
                        done = true;
                    else
                    {
                        ++runner;
                        ++index;
                    }
                }
                index = 0;
                if(!done)
                    ++buffer;
            }
            if(!done)
                CException();
            else
            {
                DestructElements(runner);
                /* определяем что положить в качестве позиции следующего на выделенное место*/
                if (index <= buffer->firstFreeIndex) // удаляемый до первого свободного
                {
                    *((int *)runner) = buffer->firstFreeIndex;
                    buffer->firstFreeIndex = index;
                }
                else // удаляемый после первого свободного
                {
                    int index_ = buffer->firstFreeIndex;
                    T *dif = buffer->pdata + index_;
                    while ((dif - runner) <= 0)
                    {
                        index_ = *(int *) dif;
                        dif = buffer->pdata + index_;
                    }
                    *(int *) runner = index_;
                }
            }
            return done;
        }

        // Очистка данных, зависит от m_isDeleteElementsOnDestruct
        void clear()
        {
            block* buffer = m_pBlocks;
            while(buffer != nullptr)
            {
                if(m_isDeleteElementsOnDestruct == false)
                    CException();
                else
                {
                    m_pBlocks = m_pBlocks->pnext;
                    m_pCurrentBlk = m_pBlocks;
                    deleteBlock(buffer);
                    buffer = m_pBlocks;
                }
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
            //делаем так,чтобы никаких указателей на удаленный блок не оставалось
            p->firstFreeIndex = 0;
            T *runner = p->pdata;
            for(int i=0; i < m_blkSize; ++i)
            {
                DestructElements(runner);
                ++runner;
            }
            delete[] ((char*)p);
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