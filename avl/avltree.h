#ifndef AVL_HEAD_H_2022_03_24
#define AVL_HEAD_H_2022_03_24

#include "mm.h"

namespace templates
{

    template <class T, int(*Compare)(const T *pElement, const T* pElement2) >
    class CAVLTree
    {
    private:
        struct leaf
        {
			int height = 1;
            T* pData;
            leaf *pLeft = nullptr;
            leaf *pRight = nullptr;
            //int balanceFactor;
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
        CAVLTree(): m_pRoot(nullptr), m_Memory(100, true) { };

        virtual ~CAVLTree()
        {
			clear();
			m_Memory.deleteObject(m_pRoot);
			m_pRoot = nullptr;
        }
        bool add(T* pElement)
		{
			m_pRoot = Add(m_pRoot, pElement);
			if(m_pRoot != nullptr)
				return true;
			return false;
		}
		bool update (T* pElement)
		{
			leaf* buffer = Update(m_pRoot, pElement);
			if(buffer)
			{
				buffer->pData = pElement;
				return true;
			}
			return !add(pElement);
		}
		bool remove(const T& element) { return (Remove(m_pRoot, element)); }
		void clear()
		{
			Empty(m_pRoot);
			m_pRoot = nullptr;
			m_Memory.clear();
		}
        T* find(const T& pElement)
        {
			leaf *buffer = Find(m_pRoot, pElement);
			if(buffer)
				return buffer->pData;
			return nullptr;
        }
		leaf* ReturnRoot() { return m_pRoot;}

    private:

		int Height(leaf* p) { return p ? p->height : 0; }
		int Factor(leaf* p) { return Height(p->pRight) - Height(p->pLeft); }
		void FixHeight(leaf* p)
		{
			int hl = Height(p->pLeft);
			int hr = Height(p->pRight);
			p->height = (hl > hr ? hl : hr) + 1;
		}
		leaf* RotateRight(leaf* p)
		{
			leaf* q = p->pLeft;
			p->pLeft = q->pRight;
			q->pRight = p;
			FixHeight(p);
			FixHeight(q);
			return q;
		}
		leaf* Update(leaf *p, T* pElement)
		{
			if(p != nullptr)
			{
				if(Compare(pElement, p->pData) < 0)
					return Update(p->pLeft, pElement);
				else if (Compare(pElement, p->pData) > 0)
					return Update(p->pRight, pElement);
				else
					return p;
			}
			else
			{
				return nullptr;
			}
		}
		leaf* RotateLeft(leaf* q)
		{
			leaf* p = q->pRight;
			q->pRight = p->pLeft;
			p->pLeft = q;
			FixHeight(q);
			FixHeight(p);
			return p;
		}
		leaf* Add(leaf* p, T* pElement)
		{
			if(p == nullptr)
			{
				p =  m_Memory.newObject();
				p->pData = pElement;
				return p;
			}
			if(Compare(pElement, p->pData) < 0)
			{
				p->pLeft = Add(p->pLeft, pElement);
			}
			else if (Compare(pElement, p->pData) > 0)
			{
				p->pRight = Add (p->pRight, pElement);
			}
			else
			{
				return nullptr;
			}
			return Balance(p);
		}
		leaf* Balance(leaf* p)
		{
			FixHeight(p);
			if( Factor(p) == 2 )
			{
				if( Factor(p->pRight) < 0 )
					p->pRight = RotateRight(p->pRight);
				return RotateLeft(p);
			}
			if( Factor(p)==-2 )
			{
				if( Factor(p->pLeft) > 0  )
					p->pLeft = RotateLeft(p->pLeft);
				return RotateRight(p);
			}
			return p;
		}
		leaf* Find(leaf* p, const T& pElement)
		{
			if(p != nullptr)
			{
				if(Compare(&pElement, p->pData) < 0)
					return Find(p->pLeft, pElement);
				else if (Compare(&pElement, p->pData) > 0)
					return Find(p->pRight, pElement);
				else
					return p;
			}
			else
			{
				return nullptr;
			}
		}
		leaf* Empty(leaf* p)
		{
			if(!p) return nullptr;
			if(p->pRight)  p->pRight = Empty(p->pRight);
			if(p->pLeft) p->pLeft = Empty((p->pLeft));
			return nullptr;
		}
		leaf* FindMin(leaf* p) { return p->pLeft ? FindMin(p->pLeft) : p; }
		leaf* RemoveMin(leaf* p)
		{
			if( p->pLeft == 0 )
				return p->pRight;
			p->pLeft = RemoveMin(p->pLeft);
			return Balance(p);
		}
		leaf* Remove(leaf *p, const T& element)
		{
			if( !p) return nullptr;
			if( Compare(&element, p->pData) < 0 )
				p->pLeft = Remove(p->pLeft,element);
			else if( Compare(&element, p->pData) > 0 )
				p->pRight = Remove(p->pRight,element);
			else
			{
				leaf *buffer = p;
				leaf* q = p->pLeft;
				leaf* r = p->pRight;
				p = nullptr;
				m_Memory.deleteObject(buffer);
				if( !r ) return q;
				leaf* min = FindMin(r);
				min->pRight = RemoveMin(r);
				min->pLeft = q;
				return Balance(min);
			}
		}

		leaf* m_pRoot;
		lab618::CMemoryManager<leaf> m_Memory;
    };

}; // namespace templates

#endif // #define AVL_HEAD_H_2015_03_31