#pragma once
#include <deque>
#include "TreeNode.h"
namespace Khellendros
{
	template<typename T, typename N>
	class BinaryTree;
	template<typename T,typename N>
	class BinaryTreeIterator:public std::iterator<std::bidirectional_iterator_tag,T>
	{
	public:
		typedef typename T ValueType;
		typedef typename N NodeType;
		typedef typename std::weak_ptr<NodeType> WeakNodePtr;
		typedef typename std::deque<WeakNodePtr> NodePtrDeque;
		friend class BinaryTree<ValueType, NodeType>;
		enum Type
		{
			//迭代器遍历方法
			PRE_ORDER,
			IN_ORDER,
			POST_ORDER,
			LEVEL_ORDER			
		};
	private:
		NodePtrDeque _nodePtrDeque;
		int _index;

		void initDeque(const WeakNodePtr& ptr,Type type)
		{
			switch (type)
			{
			case PRE_ORDER:makePreOrderDeque(ptr); break;
			case IN_ORDER:makeInOrderDeque(ptr); break;
			case POST_ORDER:makePostOrderDeque(ptr); break;
			case LEVEL_ORDER:makeLevelOrderDeque(ptr); break;
			default:
				break;
			}
		}
		void makePreOrderDeque(const WeakNodePtr& ptr)
		{
			auto tempPtr = ptr.lock();
			if (tempPtr)
			{
				_nodePtrDeque.push_back(ptr);
				makePreOrderDeque(tempPtr->getLeft());
				makePreOrderDeque(tempPtr->getRight());
			}
		}
		void makeInOrderDeque(const WeakNodePtr& ptr)
		{
			auto tempPtr = ptr.lock();
			if (tempPtr)
			{
				makeInOrderDeque(tempPtr->getLeft());
				_nodePtrDeque.push_back(ptr);
				makeInOrderDeque(tempPtr->getRight());
			}
		}
		void makePostOrderDeque(const WeakNodePtr& ptr)
		{
			auto tempPtr = ptr.lock();
			if (tempPtr)
			{
				makePostOrderDeque(tempPtr->getLeft());
				makePostOrderDeque(tempPtr->getRight());
				_nodePtrDeque.push_back(ptr);
			}
		}
		void makeLevelOrderDeque(const WeakNodePtr& ptr)
		{
			auto tempPtr = ptr.lock();
			_nodePtrDeque.push_front(ptr);
			int i = 0;
			while (tempPtr&&i<_nodePtrDeque.size())
			{
				tempPtr = _nodePtrDeque[i++].lock();
				auto temp = tempPtr->getLeft();
				if (temp.lock()) _nodePtrDeque.push_back(temp);
				temp = tempPtr->getRight();
				if (temp.lock()) _nodePtrDeque.push_back(temp);
			}
		}
	public:
		BinaryTreeIterator(const WeakNodePtr& nodePtr, Type type = Type::IN_ORDER):_index(0)
		{
			initDeque(nodePtr, type);
		}
		BinaryTreeIterator(const BinaryTreeIterator& iter) :
			_nodePtrDeque(iter._nodePtrDeque),_index(iter._index) {}
		BinaryTreeIterator(BinaryTreeIterator&& iter) :
			_nodePtrDeque(std::move(iter._nodePtrDeque)),_index(iter._index) {}
		BinaryTreeIterator& operator=(const BinaryTreeIterator& iter)
		{
			_nodePtrDeque = iter._nodePtrDeque;
			_index = iter._index;
			return *this;
		}
		BinaryTreeIterator& operator=(BinaryTreeIterator&& iter)
		{
			_nodePtrDeque = std::move(iter._nodePtrDeque);
			_index = iter._index;
			return *this;
		}
		BinaryTreeIterator& operator++()
		{
			++_index;
			return *this;
		}
		BinaryTreeIterator operator++(int)
		{
			auto temp = *this;
			++_index;
			return temp;
		}
		BinaryTreeIterator& operator--()
		{
			--_index;
			return *this;
		}
		BinaryTreeIterator operator--(int)
		{
			auto temp = *this;
			--_index;
			return temp;
		}
		ValueType operator*()const
		{
			//解除引用运算符(返回右值)
			auto nodePtr = _nodePtrDeque[_index].lock();
			return nodePtr->getValue();
		}
		bool operator==(const BinaryTreeIterator& iter)const
		{
			auto nodePtr1 = _index >= _nodePtrDeque.size()?
				WeakNodePtr().lock() :_nodePtrDeque[_index].lock();
			auto nodePtr2 = iter._index >= iter._nodePtrDeque.size() ? 
				WeakNodePtr().lock() : iter._nodePtrDeque[iter._index].lock();
			return nodePtr1 == nodePtr2;
		}
		bool operator!=(const BinaryTreeIterator& iter)const
		{
			return !operator==(iter);
		}
	};
}