#pragma once
#include <random>
#include <ctime>
#include "BinaryTree.h"
#include "CollectionTestFrame.h"
using namespace std;
using namespace Khellendros;

uniform_int_distribution<unsigned> randInt(0, 10000);
default_random_engine e;
int testTreeInsert(BinaryTree<int>& tree)
{
	tree.clear();
	int t = clock();
	for (int i = 0; i < 1000; ++i)
	{
		tree.insert(randInt(e));
	}
	return clock() - t;
}
int testTreeTraversal(BinaryTree<int>& tree)
{
	tree.clear();
	for (int i = 0; i < 1000; ++i)
	{
		tree.insert(randInt(e));
	}
	int t = clock();
	for (auto item : tree);
	return clock() - t;
}
int testTreeFind(BinaryTree<int>& tree)
{
	tree.clear();
	tree.insert(randInt(e));
	int t = clock();
	for (int i = 0; i < 1000; ++i)
	{
		tree.find(randInt(e));
	}
	return clock() - t;
}
int main()
{
	cout << "功能测试:" << endl;
	cout << "构造并插入:" << endl;
	BinaryTree<int> tree = { 8,2,4,1,0,5,7,9,-1 };
	cout << tree << endl;
	cout << "移除:" << endl;
	auto iter = tree.begin();
	iter++;
	iter = tree.remove(iter);
	iter = tree.begin();
	iter = tree.remove(iter);
	cout << tree << endl;
	cout << "搜索2" << endl;
	cout << *tree.find(2) << endl;
	cout << "计算高度:" << endl;
	cout << tree.height() << endl;
	cout << "性能测试:" << endl;
	CollectionTestFrame<BinaryTree<int>> testFrame(tree, "BinaryTree test", 100);
	testFrame.add("插入", testTreeInsert);
	testFrame.add("遍历", testTreeTraversal);
	testFrame.add("搜索", testTreeFind);
	testFrame.test(cout);
	system("pause");
}