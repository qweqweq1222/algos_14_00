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
            std::vector<bool> vec;
        };
    public:
        class CException
        {
        public:
            CException()
            {
                throw std::out_of_range("");
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
            int index = 0;
            if(m_pBlocks == nullptr) // создаем первый блок
            {
                m_pBlocks = newBlock();
                m_pCurrentBlk = m_pBlocks;
                m_pCurrentBlk->firstFreeIndex = *((int *) m_pCurrentBlk->pdata);
                ConstructElements(m_pCurrentBlk->pdata);
                ++(m_pCurrentBlk->usedCount);
                m_pCurrentBlk->vec[0] = true;
                return (m_pCurrentBlk->pdata);
            }
            else
            {
                if (m_pCurrentBlk->firstFreeIndex != -1) //повезло - в текущем сразу нашлось место
                {
                    index = runner->firstFreeIndex;
                    m_pCurrentBlk->firstFreeIndex = *((int *)(m_pCurrentBlk->pdata + m_pCurrentBlk->firstFreeIndex));
                    ConstructElements(runner->pdata + index);
                    ++(runner->usedCount);
                    runner->vec[index] = true;
                    return (runner->pdata + index);
                }
                else // надо искать новый блок (если он есть) или создавать новый
                {
                    while (runner->pnext != nullptr) //проверяем все блоки до последнего
                    {
                        if (runner->firstFreeIndex != -1) // нашли свободное место
                        {
                            index = runner->firstFreeIndex;
                            m_pCurrentBlk = runner;
                            m_pCurrentBlk->firstFreeIndex = *((int *) runner->pdata + runner->firstFreeIndex);
                            ConstructElements(runner->pdata + index);
                            ++(runner->usedCount);
                            runner->vec[index] = true;
                            return (runner->pdata + index);
                        } else
                            runner = runner->pnext;
                    }
                    //дошли до последнего блока
                    if (runner->firstFreeIndex != -1) // нашли место в последнем блоке
                    {
                        index = runner->firstFreeIndex;
                        m_pCurrentBlk = runner;
                        m_pCurrentBlk->firstFreeIndex = *((int *) runner->pdata + runner->firstFreeIndex);
                        ConstructElements(runner->pdata + index);
                        ++(runner->usedCount);
                        runner->vec[index] = true;
                        return (runner->pdata + index);
                    } else // не нашли место и в последнем блоке - создаем блок
                    {
                        runner->pnext = newBlock();
                        m_pCurrentBlk = runner->pnext;
                        m_pCurrentBlk->firstFreeIndex = 1;
                        m_pCurrentBlk->vec[0] = true;
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
            T* runner = nullptr;
            bool done = false;
            int index = 0;
            while(!done || buffer == nullptr) // ищим где элемент
            {
                runner = buffer->pdata;
                index = 0;
                while(!done && index < m_blkSize)
                {
                    if(runner == p)
                        done = true;
                    else
                    {
                        ++runner;
                        ++index;
                    }
                }
                if(!done)
                    buffer = buffer->pnext;
            }
            if(!done) //пытаемся удалить несуществующий в менеджере элемент
                CException();
            else
            {
                DestructElements(runner);
                buffer->vec[index] = false;
                --(buffer->usedCount);
                /* определяем что положить в качестве позиции следующего на выделенное место*/
                if (index <= buffer->firstFreeIndex && buffer->firstFreeIndex != -1) // удаляемый до первого свободного и место было
                {
                    *((int *)runner) = buffer->firstFreeIndex;
                    buffer->firstFreeIndex = index;
                }
                else if( buffer->firstFreeIndex == -1) // не было места до этого момента
                {
                    *((int *)runner) = -1;
                    buffer->firstFreeIndex = index;
                }
                else // удаляемый после первого свободного и место есть
                {
                    int index_ = buffer->firstFreeIndex;
                    T *dif = buffer->pdata + index_;
                    while ((dif - runner) <= 0)
                    {
                        index_ = *(int *) dif;
                        dif = buffer->pdata + index_;
                    }
                    *((int *)runner) = index_;
                }
            }
            return done;
        }

        // Очистка данных, зависит от m_isDeleteElementsOnDestruct
        void clear()
        {
            block* buffer = m_pBlocks;
            if(m_isDeleteElementsOnDestruct == false)
            {
                while(buffer != nullptr)
                {
                    if (buffer->usedCount != 0)
                        CException();
                    buffer = buffer->pnext;
                }
            }
            else
            {
                while(buffer != nullptr)
                {
                    m_pBlocks = m_pBlocks->pnext;
                    m_pCurrentBlk = m_pBlocks;
                    deleteBlock(buffer);
                    buffer = m_pBlocks;
                }
            }
        }
        void check()
        {
            block* buffer = m_pBlocks;
            while(buffer != nullptr)
            {
                for(int i =0; i < m_blkSize; ++i)
                {
                    if (buffer->vec[i] == true)
                        std::cout << " 1 ";
                    else
                        std::cout << " 0 ";
                }
                buffer = buffer->pnext;
                std::cout << std::endl;
            }
            std::cout << std::endl;
            std::cout << std::endl;
        }
        void check_exp()
        {
            block* buffer = m_pBlocks;
            T* runner = buffer->pdata;
            while(buffer != nullptr)
            {
                for(int i =0; i < m_blkSize; ++i)
                {
                    if(buffer->vec[i] == false)
                        std::cout<< *((int*)runner) << " ";
                    else
                        std::cout<< " __ ";
                    ++runner;
                }
                buffer = buffer->pnext;
                if(buffer != nullptr)
                    runner = buffer->pdata;
                std::cout << std::endl;
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
            new_block->vec.resize(m_blkSize, false);
            new_block->pdata = (T *)new char[sizeof(T)*m_blkSize];
            T* buffer = new_block->pdata;
            for(int i = 0; i < m_blkSize; ++i) // заполняем int
            {
                if(i == m_blkSize - 1)
                    *((int*)buffer) = -1;
                else
                    *((int*)buffer) = i + 1;
                std::cout << *((int*)buffer) << ' ';
                ++buffer;
            }
            std::cout << std::endl;
            return new_block;
        }

        // Освободить память блока данных. Применяется в clear
        void deleteBlock(block *p) // т.к. применятся в clear, который вызываем когда удаляем все, то занулять pnext не надо т.к. удаляем голову
        {
            T *runner = p->pdata;
            for(int i = 0; i < m_blkSize; ++i)
            {
                if((p->vec)[i] == true)
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