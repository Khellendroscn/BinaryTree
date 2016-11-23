/*
** ��������Կ��
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
		//�����Զ���
		CollectionType& _collection;
		//������
		std::string _testName;
		//�����������->���Ժ�����map
		std::map<std::string, TestFunc> testerMap;
		//����������Ƶ�����
		std::list<std::string> testNameList;
		int _time;//���Դ���

		/*
		** ����
		*/
		const std::string line = "==============================";

		/*
		** ��ʱ
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
		** ���ɱ���
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
			//�������
			os << makeTitle() << endl;
			os << "��������\t��ʱ\tƽ����ʱ" << endl;
			for (auto name : testNameList)
			{
				//�������
				int t = getTime(testerMap[name]);
				os << name << " :\t\t";
				os << t << '\t';
				os << t / _time << endl;//ƽ����ʱ
			}
		}
	};
}