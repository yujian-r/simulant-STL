#pragma once
#include<utility>
#include<vector>
#include<iostream>

namespace yj
{
	template<class T>
	struct less
	{
		bool operator()(const T& x, const T& y)
		{
			return x < y;
		}
	};

	template<class T>
	struct greater
	{
		bool operator()(const T& x, const T& y)
		{
			return x > y;
		}
	};


	template<class T, class Container = std::vector<T>,class Compare=less<T>>
	class priority_queue
	{
	public:

		void adjust_up(int child)
		{
			Compare com;

			int parent = (child - 1) / 2;
			while (child > 0)
			{
				//if (_con[child]>_con[parent])
				if (com(_con[parent],_con[child]))
				{
					std::swap(_con[parent], _con[child]);
					child = parent;
					parent = (child - 1) / 2;
				}
				else
				{
					break;
				}
			}
		}

		void adjust_down(int parent)
		{
			Compare com;

			int child = parent * 2 + 1;
			while (child<_con.size())
			{
				// 大堆: 找孩子节点中较大的
				if (child + 1 < _con.size() && com(_con[child],_con[child + 1]))
					++child;

				//if (con[_child]>_con[parent])
				if (com(_con[parent],_con[child]))
				{
					std::swap(_con[child], _con[parent]);
					parent=child;
					child = parent * 2 + 1;
				}
				else
				{
					break;
				}
			}
		}

	public:
		void push(const T& val)
		{
			_con.push_back(val);
			adjust_up(_con.size() - 1);
		}

		void pop()   // 删除堆顶元素
		{
			std::swap(_con[0], _con[_con.size() - 1]);
			_con.pop_back();
			adjust_down(0);
		}

		T& top()
		{
			return _con[0];
		}

		size_t size()
		{
			return _con.size();
		}

		bool empty()
		{
			return _con.empty();
		}

	private:
		Container _con;
	};

	void test1()
	{
		std::vector<int> v{ 3,2,7,6,0,4,1,9,8,5,88 };
		yj::priority_queue<int> q1;   
		for (auto& e : v)
			q1.push(e);
		std::cout << q1.top()<<std::endl;
		q1.pop();
		std::cout << q1.top();
	}

}