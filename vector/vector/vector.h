#pragma once
#include<utility>
#include<iostream>
#include<cassert>
#include<algorithm>

namespace yj
{

	template<class iterator, class Ref, class Ptr>
	struct reverse_iterator
	{
		typedef reverse_iterator<iterator, Ref, Ptr> self;
		iterator _cur;


		reverse_iterator(iterator cur)
			:_cur(cur)
		{}

		Ref operator*()
		{
			iterator tmp = _cur;
			return *--tmp;                // ����_cur��ǰһ��λ��
		}

		Ptr operator->()
		{
			return &(operator*());
		}

		self& operator++()
		{
			--_cur;
			return *this;
		}

		self& operator--()
		{
			++_cur;
			return *this;
		}

		self operator++(int)
		{
			self tmp(*this);
			--_cur;
			return tmp;
		}

		self operator--(int)
		{
			self tmp(*this);
			++_cur;
			return tmp;
		}

		bool operator==(const self& s)
		{
			return _cur == s._cur;
		}

		bool operator!=(const self& s)
		{
			return _cur != s._cur;
		}
	};


	template<class T>
	class vector
	{
	public:
		typedef T* iterator;
		typedef const T* const_iterator;
		typedef reverse_iterator<const_iterator, const T&, const T*> const_reverse_iterator;
		typedef reverse_iterator<iterator, T&, T*> reverse_iterator;


		reverse_iterator rbegin()
		{
			return reverse_iterator(end());
		}

		reverse_iterator rend()
		{
			return reverse_iterator(begin());
		}

		iterator begin()
		{
			return _start;
		}

		iterator end()
		{
			return _finish;
		}

		const_iterator begin()const
		{
			return _start;
		}

		const_iterator end()const
		{
			return _finish;
		}

		// �޲ι���
		vector()   
		{}

		// vector<int> v(10,5)   int�ᷢ������ת��(���Ը�ƥ��ص��õ��������乹�����ģ������)
		vector(size_t n, const T& val = T())
		{
			reserve(n);      // ��ǰ����, ���Ч��
			for (int i = 0; i < n; ++i)
			{
				push_back(val);
			}
		}


		// �ṩһ�����ذ汾
		vector(int n, const T& val = T())
		{
			reserve(n);      // ��ǰ����, ���Ч��
			for (int i = 0; i < n; ++i)
			{
				push_back(val);
			}
		}




		// ���������乹�� [first,last)
		template<class InputIterator>
		vector(InputIterator first, InputIterator last)
		{
			while (first != last)
			{
				push_back(*first);
				++first;
			}
		}

		
		//// v1(v2)  <��ͳд��>
		//vector(const vector<T>& v)
		//{
		//	_start = new T[v.capacity()];

		//	for (int i = 0; i < v.size(); ++i)
		//	{
		//		_start[i] = v._start[i];    // ��ֵ�������;
		//	}

		//	_finish = _start + v.size();
		//	_end_of_storage = _start + v.capacity();
		//}


		// v1(v2)  <�ִ�д��>
		vector(const vector<T>& v)
		{
			vector<int> tmp(v.begin(), v.end());
			swap(tmp);
		}

		// v1=v2
		vector<T>& operator=(vector<T> v)
		{
			swap(v);
			return *this;
		}


		~vector()
		{
			delete[]_start;
			_start = _finish = _end_of_storage = nullptr;
		}


		void reserve(size_t n)
		{
			if (n > capacity())    // ��ֹ����
			{
				int sz = size();
				T* tmp = new T[n];

				if (_start)
				{
					for (int i = 0; i < sz; ++i)
					{
						tmp[i] = _start[i];     // ��ֵ�������(memcpy��ǳ����)
					}
					delete[]_start;
				}
				_start = tmp;
				_finish = _start + sz;
				_end_of_storage = _start + n;
			}
		}


		void resize(size_t n, const T& val = T())
		{
			if (n < size())     // �൱��ɾ������
			{
				_finish = _start + n;
			}
			else if (n > size())
			{
				if (n > capacity())
					reserve(n);
				
				while (_finish != _start + n)
				{
					*_finish = val;
					++_finish;
				}
			}
		}

		void push_back(const T&val)
		{
			if (_finish == _end_of_storage)
			{
				reserve(capacity() == 0 ? 4 : 2 * capacity());
			}

			*_finish = val;
			++_finish;
		}

		void pop_back()
		{
			assert(!empty());
			--_finish;
		}

		iterator insert(iterator pos, const T&val)
		{
			assert(pos <= _finish);
			assert(pos >= _start);

			// 1. �������
			if (_finish == _end_of_storage)
			{
				// ���ܴ���������ݵ����, ��Ҫ��ǰ����pos��_start֮�����Ծ���
				size_t len = pos - _start;

				reserve(capacity()==0?4:2*capacity());

				// ���ݺ�ͨ����Ծ�������µ�posλ��
				pos = _start + len;  
			}

			// 2. Ų������
			iterator end = _finish - 1;
			while (end>=pos)
			{
				*(end + 1) = *(end);
				--end;
			}

			*pos = val;
			++_finish;
			return pos;   // ָ���²���ĵ�һ��Ԫ�ص�����
		}


		iterator erase(iterator pos)
		{
			assert(pos < _finish);
			assert(pos >= _start);
		
			iterator start = pos+1;
			while (start!=_finish)
			{
				*(start-1) = *(start);
				++start;
			}

			--_finish;
			return pos;   // ���ر�ɾ��Ԫ�ص���һ��Ԫ��
		}


		T& operator[](size_t pos)
		{
			assert(pos < size());

			return _start[pos];
		}

		void swap(vector<T>& v)
		{
			std::swap(_start, v._start);
			std::swap(_finish, v._finish);
			std::swap(_end_of_storage, v._end_of_storage);
		}

		bool empty()
		{
			return _start == _finish;
		}

		size_t size()const
		{
			return _finish - _start;
		}

		size_t capacity()const
		{
			return _end_of_storage - _start;
		}

	private:
		iterator _start=nullptr;
		iterator _finish=nullptr;
		iterator _end_of_storage=nullptr;
	};

	void test1()
	{
		vector<int> v1;
		v1.push_back(1);
		v1.push_back(2);
		v1.push_back(3);
		vector<int> v2(v1);
		v2.push_back(4);
		v2 = v1;
		for (auto& e : v2)
			std::cout << e << " ";
	}

	void test2()
	{
		vector<int> v1;
		v1.push_back(1);
		v1.push_back(2);
		v1.push_back(3);
		v1.push_back(4);
		v1.push_back(5);

		vector<int> v2(5,8);
		
		//v2 = v1;
		for (auto& e : v2)
			std::cout << e << " ";
	}

	void test3()
	{
		vector<int> v1;
		v1.push_back(1);
		v1.push_back(2);
		v1.push_back(3);
		v1.pop_back();
		v1.resize(5,1);
		v1.resize(3, 1);
		for (auto& e : v1)
			std::cout << e << " ";
	}

	// ɾ��Ԫ�غ������ʧЧ
	void test5()
	{
		vector<int> v1;
		v1.push_back(1);
		v1.push_back(2);
		v1.push_back(3);
		v1.push_back(5);

		//auto pos = std::find(v1.begin(), v1.end(), 2);      // ������ܲ������
		auto pos = std::find(v1.begin(), v1.end(), 4);        // һ�������
		if (pos != v1.end())
			v1.erase(pos);

		(*pos)++;

		for (auto e : v1)
			std::cout << e << " ";
		std::cout << std::endl;
	}


	// ɾ��Ԫ�غ�---���µ�����
	void test6()
	{
		vector<int> v1;
		v1.push_back(1);
		v1.push_back(2);
		v1.push_back(3);
		v1.push_back(4);

		auto it = v1.begin();       
		while(it!= v1.end())
		{
			if (*it % 2 == 0)
				it = v1.erase(it);   // ɾ��Ԫ�غ���µ�����
			else
				++it;
		}

		for (auto e : v1)
			std::cout << e << " ";
		std::cout << std::endl;
	}

	void test7()
	{
		vector<int> v1;
		v1.push_back(1);
		v1.push_back(2);
		v1.push_back(3);
		v1.push_back(4);

		//auto pos = v1.end();
		auto pos = v1.begin();
		v1.insert(pos, 8);
		for (auto e : v1)
			std::cout << e << " ";
		std::cout << std::endl;
	}


	void test8()
	{
		vector<int> v1;
		v1.push_back(1);
		v1.push_back(2);
		v1.push_back(3);
		v1.push_back(4);

		for (auto rit=v1.rbegin();rit!=v1.rend();++rit)
			std::cout << *rit << " ";
		std::cout << std::endl;
	}
}