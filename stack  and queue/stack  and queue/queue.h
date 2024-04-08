#pragma once
#include<list>
#include<iostream>
#include<deque>

namespace yj
{
	template<class T, class Container = std::deque<T>>
	class queue
	{
	public:
		void push(const T& val)
		{
			_con.push_back(val);
		}

		void pop()
		{
			_con.pop_front();
		}

		const T& front()
		{
			return _con.front();
		}

		const T& back()
		{
			return _con.back();
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

	void test2()
	{
		queue<int> q;
		q.push(1);
		q.push(2);
		q.push(3);
		q.push(4);
		q.pop();
		std::cout << q.front() << " " << q.back() << " " << q.size() << std::endl;
		std::cout << q.empty() << std::endl;
	}
}