/*
*二叉树
*/
#pragma once
#include <stack>
#include <memory>
#include <iostream>
#include "TreeNode.h"
#include "BinaryTreeIterator.h"
namespace Khellendros
{
	template <typename T, typename N = BinaryTreeNode<T>>
	class BinaryTree
	{
	public:
		typedef typename T ValueType;
		typedef typename N NodeType;
		typedef typename int SizeType;
		//typedef typename BinaryTreeIterator<ValueType> IterType;
		typedef typename std::shared_ptr<NodeType> SharedNodePtr;
		typedef typename std::weak_ptr<NodeType> WeakNodePtr;
		typedef typename BinaryTreeIterator<ValueType, NodeType> IterType;
	private:
		SharedNodePtr _root;//根节点
		SizeType _size;//尺寸
	protected:
		//tool functions:
		SharedNodePtr findInsertTarget
		(WeakNodePtr& start, const ValueType& value)const
		{
			auto ptr = start.lock();
			if (!ptr)
			{
				//如果开始节点是nullptr就直接返回
				return ptr;
			}
			else
			{
				if (value < ptr->getValue())
				{
					if (!ptr->getLeft().lock())
						return ptr;
					else
						return findInsertTarget(ptr->getLeft(), value);
				}
				else
				{
					if (!ptr->getRight().lock())
						return ptr;
					else
						return findInsertTarget(ptr->getRight(), value);
				}
			}
		}
		void updateNode(const WeakNodePtr& nodePtr,const SharedNodePtr& newNode)
		{
			auto ptr = nodePtr.lock();
			auto parentPtr = ptr->getParent().lock();
			parentPtr->getLeft().lock() == ptr ? parentPtr->setLeft(newNode) : parentPtr->setRight(newNode);
			if (newNode)newNode->setParent(parentPtr);
		}
		ValueType remove(const WeakNodePtr& weakPtr)
		{
			auto nodePtr = weakPtr.lock();
			ValueType delValue = nodePtr->getValue();
			if (nodePtr->isLeaf())
			{
				//如果要删除的节点是叶节点，直接将删除
				updateNode(weakPtr, nullptr);
			}
			else if (nodePtr->hasLeft() && nodePtr->hasRight())
			{
				//如果该节点拥有两个孩子,找到该节点中序遍历中的前一个节点
				IterType iter(weakPtr);//通过迭代器找到准备删除的节点
				//递归删除节点
				ValueType temp = remove(iter._nodePtrDeque.front());
				//使用被删除的节点的值替换该节点
				nodePtr->setValue(temp);
			}
			else
			{
				//该节点只有一个孩子，直接使父节点指向该子树
				auto childPtr = nodePtr->hasLeft() ? nodePtr->getLeft() : nodePtr->getRight();
				updateNode(weakPtr, childPtr.lock());
			}
			return delValue;
		}
		WeakNodePtr find(const WeakNodePtr& nodePtr,const ValueType& target)const
		{
			auto ptr = nodePtr.lock();
			if (!ptr)
			{
				return WeakNodePtr();
			}
			else if (ptr->getValue() == target)
			{
				return nodePtr;
			}
			else if (target < ptr->getValue())
			{
				return find(ptr->getLeft(), target);
			}
			else
			{
				return find(ptr->getRight(), target);
			}

		}
		int height(const WeakNodePtr& wPtr)
		{
			auto ptr = wPtr.lock();
			if (!ptr)
			{
				return 0;
			}
			else
			{
				int heightLeft = height(ptr->getLeft())+1;
				int heightRight = height(ptr->getRight())+1;
				return heightLeft > heightRight ? heightLeft : heightRight;
			}
		}
	public:
		BinaryTree() :_size(0){}
		BinaryTree(const BinaryTree& tree) :_size(tree._size)
		{
			for (auto item : tree)
			{
				insert(item);
			}
		}
		BinaryTree(BinaryTree&& tree) :_root(tree._root), _size(tree._size) {}
		BinaryTree(const std::initializer_list<ValueType>& initList)
		{
			for (auto item : initList)
			{
				insert(item);
			}
		}
		void insert(const ValueType& item)
		{
			auto ptr = std::make_shared<NodeType>(item);
			if (!_root)
			{
				_root = ptr;
			}
			else
			{
				auto target = findInsertTarget(WeakNodePtr(_root), item);
				if (item < target->getValue())
					target->setLeft(ptr);
				else
					target->setRight(ptr);
				ptr->setParent(target);
			}
			++_size;
		}
		IterType remove(const IterType& iter)
		{
			if (iter._index < 0) return end();
			WeakNodePtr node = iter._nodePtrDeque[iter._index];
			if (node.expired()) return end();
			remove(node);
			return --IterType(iter);
		}
		IterType find(const ValueType& value)const
		{
			WeakNodePtr ptr = find(_root, value);
			if (ptr.expired()) return end();
			else return IterType(ptr,IterType::PRE_ORDER);
		}
		bool contains(const ValueType& value)const
		{
			WeakNodePtr ptr = find(_root, value);
			return !ptr.expired();
		}
		int height()
		{
			return height(_root);
		}
		IterType begin(typename IterType::Type type)const
		{
			return IterType(WeakNodePtr(_root), type);
		}
		IterType end(typename IterType::Type type)const
		{
			return IterType(WeakNodePtr(), type);
		}
		void clear()
		{
			_root.reset();
		}
		IterType begin()const
		{
			return begin(IterType::IN_ORDER);
		}
		IterType end()const
		{
			return end(IterType::IN_ORDER);
		}

		BinaryTree& operator<<(const ValueType& item)
		{
			insert(item);
			return *this;
		}
		template<typename T>
		friend std::ostream& operator<<(std::ostream& os, const BinaryTree<T>& tree);
	};
	template<typename T>
	std::ostream& operator<<(std::ostream& os, const BinaryTree<T>& tree)
	{
		os << "[";
		for (auto item : tree)
		{
			os << " " << item << " ,";
		}
		os << "\b]";
		return os;
	}
}