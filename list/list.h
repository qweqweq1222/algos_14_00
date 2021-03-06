#ifndef TEMPLATES_LIST_2022_02_03
#define TEMPLATES_LIST_2022_02_03

#include<exception>
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
                m_pBegin = nullptr;
                m_pCurrent = p;
            }

            CIterator(const CIterator &src)
            {
                m_pBegin = src.m_pBegin;
                m_pCurrent = src.m_pCurrent;
            }

            ~CIterator()
            {
                m_pCurrent = nullptr;
                m_pBegin = nullptr;
            }

            CIterator& operator = (const CIterator&  src)
            {
                m_pBegin = src.m_pBegin;
                m_pCurrent = src.m_pCurrent;
                return *this;
            }

            bool operator != (const CIterator&  it) const
            {
                return (it.m_pCurrent != m_pCurrent);
            }

            void operator++()
            {
                if(m_pCurrent != nullptr && (isValid() || (m_pCurrent->pnext == nullptr)))
                    m_pCurrent = m_pCurrent->pnext;
                else if(m_pCurrent == nullptr && m_pBegin != nullptr)
                    m_pCurrent = m_pBegin;
            }

            T& getData()
            {
                return m_pCurrent->data;;
            }

            T& operator* ()
            {
                return m_pCurrent->data;
            }

            leaf* getLeaf()
            {
                return m_pCurrent;
            }

            void setLeaf(leaf* p)
            {
                m_pCurrent = p;
            }

            void setLeafPreBegin(leaf* p)
            {
                m_pBegin=p;
                m_pCurrent = nullptr;
            }

            bool isValid(){
                return (m_pCurrent == nullptr)? false : true;
            }

        private:
            //???????????? ???????????? ????????????, ???????? ???? ?????????????????? ?????????? ??????????????
            leaf* m_pBegin;
            // ???????????? ?????????????? ??????????????????
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
            this->clear();
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
            T tmp = m_pBegin->data;
            if(m_pBegin != m_pEnd)
            {
                leaf *buffer = m_pBegin;
                m_pBegin = m_pBegin->pnext;
                delete buffer;
            }
            else if(m_pBegin == m_pEnd)
            {
                if (m_pBegin != nullptr)
                {
                    leaf *buffer = m_pBegin;
                    tmp = m_pBegin->data;
                    m_pBegin = nullptr;
                    m_pEnd = nullptr;
                    delete buffer;
                }
            }
            return tmp;
        }

        // ???????????????? ?????????????????? ??????????????????. ???????????????????? ???????????????????? ??????????????.
        void erase(CIterator& it)
        {
            leaf* buffer = m_pBegin;
            leaf* current = it.getLeaf();
            if(current == m_pBegin)
            {
                if(m_pBegin == m_pEnd && m_pBegin != nullptr)
                {
                    it.setLeafPreBegin(nullptr);
                    m_pBegin = nullptr;
                    m_pEnd = nullptr;
                }
                else if (m_pBegin != m_pEnd)
                {
                    m_pBegin = m_pBegin->pnext;
                    it.setLeafPreBegin(m_pBegin);
                }
            }
            else
            {
                while(buffer->pnext != it.getLeaf())
                    buffer = buffer->pnext;
                leaf* buffer_ = it.getLeaf();
                if(current == m_pEnd)
                {
                    it.setLeaf(buffer);
                    buffer->pnext = nullptr;
                    m_pEnd = buffer;
                }
                else
                {
                    buffer->pnext = it.getLeaf()->pnext;
                    it.setLeaf(buffer);
                }
                buffer = buffer_;
            }
            delete buffer;
        }

        int getSize()
        {
            int size = 0;
            leaf *runner = m_pBegin;
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
                leaf *runner = nullptr;
                while (m_pBegin != m_pEnd) {
                    runner = m_pBegin;
                    m_pBegin = m_pBegin->pnext;
                    delete runner;
                }
                runner = m_pBegin;
                m_pBegin = nullptr;
                m_pEnd = nullptr;
                delete m_pBegin;
            }
        }

        CIterator begin()
        {
            CIterator it(m_pBegin);
            return it;
        }

    private:
        leaf* m_pBegin, *m_pEnd;
    };
    template<class T>
    class CDualLinkedList
    {
    private:
        struct leaf
        {
            T data;
            leaf * pnext, *pprev;
            leaf(T& _data, leaf * _pprev, leaf * _pnext):data(_data), pprev(_pprev), pnext(_pnext){}
        };
    public:
        class CIterator
        {
        public:
            CIterator():m_pCurrent(0), m_pBegin(0), m_pEnd(0){}

            CIterator(leaf *p)
            {
                m_pBegin = nullptr;
                m_pCurrent = p;
                m_pEnd = nullptr;
            }

            CIterator(const CIterator &src)
            {
                m_pCurrent =src.m_pCurrent;
                m_pEnd = src.m_pEnd;
                m_pBegin = src.m_pBegin;
            }

            ~CIterator()
            {
                m_pCurrent = nullptr;
                m_pBegin = nullptr;
                m_pEnd = nullptr;
            }

            CIterator& operator = (const CIterator&  src)
            {
                m_pCurrent =src.m_pCurrent;
                m_pEnd = src.m_pEnd;
                m_pBegin = src.m_pBegin;
                return *this;
            }

            bool operator != (const CIterator&  it) const
            {
                return (m_pCurrent != it.m_pCurrent);
            }

            void operator++()
            {
                if(m_pCurrent != nullptr)
                    m_pCurrent = m_pCurrent->pnext;
                else if(m_pCurrent == nullptr && m_pBegin != nullptr)
                    m_pCurrent = m_pBegin;
            }

            void operator--()
            {
                if(m_pCurrent != nullptr)
                    m_pCurrent = m_pCurrent->pprev;
                else if(m_pCurrent == nullptr && m_pEnd != nullptr)
                    m_pCurrent = m_pEnd;
            }

            T& getData()
            {
                return m_pCurrent->data;;
            }

            T& operator* ()
            {
                return m_pCurrent->data;
            }

            leaf* getLeaf()
            {
                return m_pCurrent;
            }

            // ?????????????????????? ?? erase ?? eraseAndNext
            void setLeaf(leaf* p)
            {
                m_pCurrent = p;
            }

            // ?????????????????????? ?? erase ?? eraseAndNext
            void setLeafPreBegin(leaf* p)
            {
                m_pBegin=p;
                m_pCurrent = nullptr;
            }

            // ?????????????????????? ?? erase ?? eraseAndNext (?????????????????? end)
            void setLeafPostEnd(leaf* p)
            {
                m_pEnd = p;
                m_pCurrent = nullptr;
                m_pBegin = nullptr;
            }

            bool isValid(){
                return (m_pCurrent == nullptr)? false : true;
            }

        private:
            //???????????? ???????????? ????????????, ???????? ???? ?????????????????? ?????????? ??????????????
            leaf* m_pBegin;
            // ???????????? ?????????????? ??????????????????
            leaf* m_pCurrent;
            //???????????? ?????????? ????????????, ???????? ???? ?????????????????? ?????????? ??????????
            leaf* m_pEnd;
        };

    public:

        CDualLinkedList()
        {
            m_pBegin = nullptr;
            m_pEnd = nullptr;
        };

        virtual ~CDualLinkedList()
        {
            this->clear();
        };

        void pushBack(T& data)
        {
            leaf *element = new leaf(data, m_pEnd, nullptr);
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

        T popBack()
        {
            T tmp = T();
            if(m_pBegin != nullptr)
            {
                tmp = m_pEnd->data;
                leaf *buffer = nullptr;
                if (m_pBegin != m_pEnd) {
                    (*m_pEnd->pprev).pnext = nullptr;
                    buffer = m_pEnd;
                    m_pEnd = m_pEnd->pprev;
                    delete buffer;
                } else {
                    buffer = m_pEnd;
                    m_pEnd = nullptr;
                    m_pBegin = nullptr;
                    delete buffer;
                }
            }
            return tmp;
        }

        void pushFront(T& data)
        {
            leaf *element = new leaf(data, nullptr, m_pBegin);
            if(m_pBegin == nullptr && m_pEnd == nullptr)
            {
                m_pBegin = element;
                m_pEnd = element;
            }
            else
            {
                m_pBegin->pprev = element;
                m_pBegin = element;
            }
        }

        T popFront()
        {
            leaf *buffer = m_pBegin;
            T tmp = m_pBegin->data;;
            if(m_pBegin !=m_pEnd)
            {
                m_pBegin = m_pBegin->pnext;
                m_pBegin->pprev = nullptr;
            }
            else
            {
                m_pBegin = nullptr;
                m_pEnd = nullptr;
            }
            delete buffer;
            return tmp;
        }

        // ???????????????? ?????????????????? ??????????????????. ???????????????????? ???????????????????? ??????????????.
        void erase(CIterator& it)
        {
            leaf* buffer = m_pBegin;
            leaf* current = it.getLeaf();
            if(current == m_pBegin)
            {
                if(m_pBegin == m_pEnd && m_pBegin != nullptr)
                {
                    it.setLeafPreBegin(nullptr);
                    m_pBegin = nullptr;
                    m_pEnd = nullptr;
                }
                else
                {
                    m_pBegin = m_pBegin->pnext;
                    m_pBegin->pprev = nullptr;
                    it.setLeafPreBegin(m_pBegin);
                }
            }
            else
            {
                buffer = it.getLeaf();
                if(current == m_pEnd)
                {
                    m_pEnd = it.getLeaf()->pprev;
                    m_pEnd->pnext= nullptr;
                    it.setLeaf(m_pEnd);
                }
                else
                {
                    buffer->pnext->pprev = buffer->pprev;
                    buffer->pprev->pnext = buffer->pnext;
                    it.setLeaf(buffer->pprev);
                }
            }
            delete buffer;
        }

        // ???????????????? ?????????????????? ??????????????????. ???????????????????? ?????????????????? ??????????????.
        void eraseAndNext(CIterator& it)
        {
            leaf *buffer = it.getLeaf();
            if(buffer == m_pBegin)
            {
                if(m_pBegin == m_pEnd && m_pBegin != nullptr)
                {
                    it.setLeafPostEnd(nullptr);
                    m_pBegin = nullptr;
                    m_pEnd = nullptr;
                }
                else
                {
                    m_pBegin = m_pBegin->pnext;
                    m_pBegin->pprev = nullptr;
                    it.setLeaf(m_pBegin);
                }
            }
            else
            {
                if(buffer == m_pEnd)
                {
                    m_pEnd = m_pEnd->pprev;
                    m_pEnd->pnext = nullptr;
                    it.setLeafPostEnd(m_pEnd);
                }
                else
                {
                    buffer->pnext->pprev = buffer->pprev;
                    buffer->pprev->pnext = buffer->pnext;
                    it.setLeaf(buffer->pnext);
                }
            }
            delete buffer;
        }

        int getSize()
        {
            int size = 0;
            leaf *runner = m_pBegin;
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
                leaf *runner = nullptr;
                while (m_pBegin != m_pEnd) {
                    runner = m_pBegin;
                    m_pBegin = m_pBegin->pnext;
                    delete runner;
                }
                runner = m_pBegin;
                m_pBegin = nullptr;
                m_pEnd = nullptr;
                delete m_pBegin;
            }
        }

        CIterator begin()
        {
            CIterator it(m_pBegin);
            return it;
        }

        CIterator end()
        {
            CIterator it(m_pEnd);
            return it;
        }

    private:
        leaf* m_pBegin, *m_pEnd;
    };
};
#endif//#ifndef TEMPLATES_LIST_2022_02_03
