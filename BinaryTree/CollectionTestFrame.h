/*
** 容器类测试框架
*/

#pragma once
#include <map>
#include <list>
#include <string>
#include <functional>
#include <iostream>
namespace Khellendros
{
	template<typename T>
	class CollectionTestFrame
	{
	public:
		typedef typename T CollectionType;
		typedef typename std::function<int(CollectionType)> TestFunc;
	private:
		//被测试对象
		CollectionType& _collection;
		//测试名
		std::string _testName;
		//保存测试名称->测试函数的map
		std::map<std::string, TestFunc> testerMap;
		//保存测试名称的链表
		std::list<std::string> testNameList;
		int _time;//测试次数

		/*
		** 常量
		*/
		const std::string line = "==============================";

		/*
		** 计时
		*/
		int getTime(TestFunc& func)
		{
			int t = 0;
			for (int i = 0; i < _time; ++i)
			{
				t+=func(_collection);
			}
			return t;
		}

		/*
		** 生成标题
		*/
		std::string makeTitle()const
		{
			int begin = (line.size() - _testName.size()-2) / 2;
			std::string title = line;
			title[begin++] = ' ';
			for (char c : _testName)
			{
				title[begin++] = c;
			}
			title[begin] = ' ';
			return title;
		}
	public:
		CollectionTestFrame(CollectionType& collection,const std::string& testName, int time) :
			_collection(collection), _testName(testName), _time(time) {}
		void add(const std::string& name, const TestFunc& func)
		{
			testNameList.push_back(name);
			testerMap[name] = func;
		}
		void test(std::ostream& os)
		{
			using std::endl;
			//输出标题
			os << makeTitle() << endl;
			os << "测试名称\t用时\t平均用时" << endl;
			for (auto name : testNameList)
			{
				//逐项测试
				int t = getTime(testerMap[name]);
				os << name << " :\t\t";
				os << t << '\t';
				os << t / _time << endl;//平均用时
			}
		}
	};
}