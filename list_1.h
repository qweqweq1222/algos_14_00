//
// Created by anreydron on 06.02.2022.
//


//
// Created by anreydron on 06.02.2022.
//

namespace lab618
{
    template<class T>
    class CSingleLinkedList
    {
    private:
        struct leaf
        {
            T data;
            leaf * pnext;
            leaf(T& _data, leaf * _pnext): data(_data), pnext(_pnext){ }
        };
    public:
        class CIterator
        {
        public:
            CIterator(): m_pCurrent(0), m_pBegin(0) {}

            CIterator(leaf *p)
            {
                this->m_pBegin = p;
                this->m_pCurrent = p;
            }

            CIterator(const CIterator &src)
            {
                m_pBegin = src.m_pBegin;
                m_pCurrent = src.m_pCurrent;
            }

            ~CIterator()
            {
            }

            CIterator& operator = (const CIterator&  src)
            {
                m_pBegin = src.m_pBegin;
                m_pCurrent = src.m_pCurrent;
            }

            bool operator != (const CIterator&  it) const
            {
                return (it.m_pCurrent != this->m_pCurrent);
            }

            void operator++()
            {
                if(this->isValid() || ((*(this->m_pCurrent)).pnext == nullptr))
                    this->m_pCurrent = ((*this).m_pCurrent)->pnext;
            }

            T& getData()
            {
                return T();
            }

            T& operator* ()
            {
                return (*this).m_pCurrent->data;
            }

            leaf* getLeaf()
            {
                return this->m_pCurrent;
            }

            void setLeaf(leaf* p)
            {

            }

            void setLeafPreBegin(leaf* p)
            {
            }

            bool isValid()
            {
                if(this->m_pCurrent == nullptr)
                    return false;
                else
                    return true;
            }

        private:
            //храним голову списка, если мы находимся перед началом
            leaf* m_pBegin;
            // храним текущее положение
            leaf* m_pCurrent;
        };

    public:

        CSingleLinkedList()
        {
            m_pBegin = nullptr;
            m_pEnd = nullptr;
        }

        virtual ~CSingleLinkedList()
        {
            leaf *buffer = nullptr;
            while(m_pBegin != m_pEnd) {
                buffer = (*m_pBegin).pnext;
                m_pBegin->~leaf();
                m_pBegin = buffer;
            }
            m_pBegin->~leaf();
        }

        void pushBack(T& data)
        {
            leaf *element = new leaf(data, nullptr);
            if(m_pBegin == nullptr && m_pEnd == nullptr)
            {
                m_pBegin = element;
                m_pEnd  = element;
            }
            else
            {
                (*m_pEnd).pnext = element;
                m_pEnd = element;
            }
        }

        void pushFront(T& data)
        {
            leaf *element = new leaf(data, nullptr);
            if(m_pBegin == nullptr && m_pEnd == nullptr) {
                m_pBegin = element;
                m_pEnd  = element;
            }
            else
            {
                leaf *buffer= m_pBegin;
                m_pBegin = element;
                (*m_pBegin).pnext = buffer;
            }
        }
        void print_list()
        {
            leaf *l = m_pBegin;
            if(l == nullptr)
                std::cout<< "list is empty" << std::endl;
            while(l)
            {
                std::cout << l->data << " ";
                l = l->pnext;
            }
            std::cout << std::endl;
        }

        T popFront()
        {
            T tmp;
            if(m_pBegin != m_pEnd) {
                tmp = (*m_pBegin).data;
                leaf *buffer = (*m_pBegin).pnext;
                m_pBegin->~leaf();
                m_pBegin = buffer;
            }
            else if(m_pBegin == m_pEnd)
            {
                tmp = (*m_pBegin).data;
                m_pBegin->~leaf();
                m_pBegin = nullptr;
                m_pEnd = nullptr;
            }
            else
                std::cout << "List is empty" <<std::endl;
            return tmp;
        }

        // изменяет состояние итератора. выставляет предыдущую позицию.
        void erase(CIterator& it)
        {
            if(this->m_pBegin = this->m_pEnd)
            {
                it.m_pCurrent->~leaf();
                this->m_pBegin = nullptr;
                this->m_pEnd = nullptr;
            }
            else if(this->m_pBegin == nullptr)
                std::cout << "Nothing to erase" << std::endl;
            else if(it.m_pCurrent == this->m_pEnd)
            {
                if(it.m_pBegin != this->m_pBegin)
                    std::cout << "Error - no head pointer" << std::endl;
                else
                {
                    leaf *buffer = this->m_pBegin;
                    while(buffer->pnext != it.m_pCurrent)
                        buffer = buffer->pnext;
                    it.m_pCurrent->~leaf();
                    this->m_pEnd = buffer;
                    it.m_pCurrent = buffer;
                }
            }
        }

        int getSize()
        {
            int size = 0;
            leaf *runner = this->m_pBegin;
            while(runner)
            {
                runner = runner->pnext;
                ++size;
            }
            return size;
        }

        void clear()
        {
            if(m_pBegin != nullptr)
            {
                leaf *buffer = nullptr;
                while (m_pBegin != nullptr)
                {
                    buffer = (*m_pBegin).pnext;
                    m_pBegin->~leaf();
                    m_pBegin = buffer;
                }
                m_pEnd = nullptr;
            }
            else
                std::cout<<"List is already clear" << std::endl;
        }

        CIterator begin()
        {
            CIterator it(this->m_pBegin);
            return it;
        }

    private:
        leaf* m_pBegin, *m_pEnd;
    };
};//#ifndef TEMPLATES_LIST_2022_02_03
