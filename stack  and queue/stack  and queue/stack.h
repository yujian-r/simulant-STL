#pragma once
#include<vector>
#include<iostream>
#include<deque>

namespace yj
{
	template<class T,class Container=std::deque<T>>
	class stack
	{
	public:
		void push(const T& val)
		{
			_con.push_back(val);
		}

		void pop()
		{
			_con.pop_back();
		}

		const T& top()
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

	void test1()
	{
		stack<int> st; 
		st.push(1);
		st.push(2);
		st.push(3);
		st.push(4);
		st.pop();
		std::cout << st.top() << " "<< st.size() << std::endl;
		std::cout << st.empty() << std::endl;
	}
}