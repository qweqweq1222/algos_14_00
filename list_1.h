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
            ~leaf() = default;
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

            ~CIterator()=default;

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
                return (*this).m_pCurrent->data;;
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
                this->m_pCurrent = p;
            }

            void setLeafPreBegin(leaf* p)
            {
                if(p != nullptr && p->pnext != nullptr)
                {
                    leaf *buffer = new leaf(p->data, p->pnext);
                    this->m_pBegin = p->pnext;
                    this->m_pCurrent = buffer;
                }
                else if(p->pnext == nullptr)  //отдельно рассматривается случай когда удаляем последний элемент.
                {
                    leaf *buffer = new leaf(p->data, nullptr);
                    this->m_pBegin = nullptr;
                    this->m_pCurrent = buffer;
                }
                else
                    std::cout << "nothing to erase\n";
            }

            bool isValid(){
                return (this->m_pCurrent == nullptr)? false : true;
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
            while(m_pBegin != m_pEnd) {
                this->popFront();
            }
            leaf *buffer = m_pBegin;
            m_pBegin = nullptr;
            m_pEnd = nullptr;
            delete buffer;
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

        T popFront()
        {
            T tmp = T();
            if(m_pBegin != m_pEnd) {
                leaf *buffer = m_pBegin;
                tmp = (*m_pBegin).data;
                m_pBegin = m_pBegin->pnext;
                delete buffer;
                return tmp;
            }
            else if(m_pBegin == m_pEnd)
            {
                if (m_pBegin != nullptr) {
                    leaf *buffer = m_pBegin;
                    tmp = (*m_pBegin).data;
                    m_pBegin = nullptr;
                    m_pEnd = nullptr;
                    delete buffer;
                    return tmp;
                }
                else {
                    std::cout << "list is already empty\n";
                    return tmp;
                }
            }
            return tmp;
        }

        // изменяет состояние итератора. выставляет предыдущую позицию.
        void erase(CIterator& it)
        {
            leaf *buffer = this->m_pBegin;
            bool a = it.isValid();
            if(this->m_pBegin == it.getLeaf())
            {
                if (this->m_pBegin == this->m_pEnd && this->m_pBegin != nullptr)
                {
                    it.setLeafPreBegin(this->m_pBegin);
                    m_pBegin = nullptr;
                    m_pEnd = nullptr;
                    delete buffer;
                }
                else if (this->m_pBegin == nullptr) {
                    try {
                        throw 55;
                    } catch (int n) {
                        std::cout << "list is  empty - can't erase. Exit status 55 \n";
                        exit(n);
                    }
                }
                else
                {
                    it.setLeafPreBegin(this->m_pBegin);
                    m_pBegin = m_pBegin->pnext;
                    delete buffer;
                }
            }

            else
            {
                while(buffer->pnext != it.getLeaf() && buffer != it.getLeaf())
                    buffer = buffer->pnext;
                if(it.getLeaf() == this->m_pEnd)
                {
                    this->m_pEnd = buffer;
                    buffer = buffer->pnext;
                    delete buffer;
                }
                else
                {
                    leaf *buffer_ = buffer->pnext;
                    buffer->pnext = it.getLeaf()->pnext;
                    it.setLeaf(buffer);
                    buffer = buffer_;
                    delete buffer;
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
            while(this->m_pBegin != nullptr)
                this->popFront();
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
