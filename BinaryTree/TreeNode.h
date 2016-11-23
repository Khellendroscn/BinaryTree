/*
*���ڵ�
*ʹ��RAII�����ڴ�
*/
#pragma once
#include <memory>
namespace Khellendros
{
	template<typename T>
	class BinaryTreeNode
	{
	public:
		typedef typename T ValueType;
		typedef typename std::unique_ptr<T> ValuePtr;
		typedef typename std::weak_ptr<BinaryTreeNode<T>> WeakNodePtr;
		typedef typename std::shared_ptr<BinaryTreeNode<T>> SharedNodePtr;
	private:
		ValuePtr _valuePtr;//����ָ��
		SharedNodePtr _parent;//���ڵ�
		SharedNodePtr _leftChild;//����
		SharedNodePtr _rightChild;//�Һ���
	public:
		BinaryTreeNode() = default;
		BinaryTreeNode
		(const WeakNodePtr& leftChild, const WeakNodePtr& rightChild , const ValuePtr& valuePtr) :
			_valuePtr(valuePtr), _leftChild(leftChild), _rightChild(rightChild) {}
		BinaryTreeNode(const ValueType& value) :_valuePtr(new T(value)){}
		BinaryTreeNode(BinaryTreeNode&& node) :
			_valuePtr(node._valuePtr.release()),
			_leftChild(node._leftChild),
			_rightChild(node._rightChild),
			_parent(node._parent)
			{}

		ValueType& getValue() { return *_valuePtr; }
		void setValue(const ValueType& value) { *_valuePtr = value; }
		WeakNodePtr getLeft() { return WeakNodePtr(_leftChild); }
		void setLeft(const SharedNodePtr& leftPtr) { _leftChild = leftPtr; }
		WeakNodePtr getRight() { return WeakNodePtr(_rightChild); }
		void setRight(const SharedNodePtr& rightPtr) { _rightChild = rightPtr; }
		WeakNodePtr getParent() { return _parent; }
		void setParent(const SharedNodePtr& parentPtr) { _parent = parentPtr; }
		bool isLeaf() { return !(_leftChild || _rightChild); }
		bool isNil() { return isLeaf() && !_valuePtr; }
		bool hasLeft()const { return _leftChild.operator bool(); }
		bool hasRight()const { return _rightChild.operator bool(); }
		bool isRoot()const { return _parent.expired(); }
	};
}
