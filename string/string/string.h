#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<cstring>
#include<utility>   
#include<iostream>
#include<cassert>

namespace yj
{
	class string
	{
	public:

		typedef char* iterator;
		typedef const char* const_iterator;

		iterator begin()
		{
			return _str;
		}

		iterator end()
		{
			return _str + _size;
		}

		const_iterator begin()const
		{
			return _str;
		}

		const_iterator end()const
		{
			return _str + _size;
		}

		string(const char*str="")
			:_size(strlen(str))
		{
			_capacity = _size == 0 ? 3 : _size;   // ��ΪҪ�����ַ�, _capacity����Ϊ0
			_str = new char[_capacity + 1];       // �࿪һ���ռ��'\0
			strcpy(_str, str);
		}

		// s1(s2) <��ͳд��>
		string(const string& s)
			:_size(s._size)
			,_capacity(s._capacity)
		{
			_str = new char[_capacity + 1];
			strcpy(_str, s._str);
		}


		// s1(s2) <�ִ�д��>
		//string(const string& s)
		//	:_size(s._size)
		//	,_str(nullptr)
		//	, _capacity(s._capacity)
		//{
		//	string tmp(s._str);
		//	swap(tmp);
		//}


		// s1=s2 <��ͳд��>
		//string& operator=(const string& s)
		//{
		//	// 3�����, ������: ���¿ռ�󿽱����ݵ�ԭ�ռ�
		//	if (this != &s)
		//	{
		//		char* tmp = new char[s._capacity+1];
		//		strcpy(tmp, s._str);
		//		delete[] _str;
		//		_str = tmp;
		//		_size = s._size;
		//		_capacity = s._capacity;
		//	}
		//	return *this;
		//}


		// s1=s2 <�ִ�д��>
		string& operator=(string s)
		{
			swap(s);
			return *this;
		}

		~string()
		{
			 delete[] _str;
			 _str = nullptr;
			 _size = _capacity = 0;
		}

		void reserve(size_t n)
		{
			if (n > _capacity)     // ��ֹ����
			{
				char* tmp = new char[n + 1];
				strcpy(tmp, _str);
				delete[]_str;
				_str = tmp;
				_capacity = n;
			}
		}


		void resize(size_t n,char ch='\0')
		{
			if (n < _size)
			{
				_size = n;
				_str[_size] = '\0';
			}
			else if (n > _size)
			{
				if (n > _capacity)
				{
					reserve(n);
				}
				memset(_str + _size, ch, n-_size);   //��ֵҲ����

				_size = n;
				_str[_size] = '\0';
			}
		}

		string& insert(size_t pos, char ch)
		{
			assert(pos <= _size);   // pos = _size����'\0'λ�ò���

			if (_size + 1 > _capacity)
				reserve(2 * _capacity);

			size_t end = _size+1;    // ��_size+1λ�ÿ�ʼ, ��ʾ��'\0'ҲҪ����ƶ�
			while (end > pos)
			{
				_str[end] = _str[end - 1];
				--end;
			}
			_str[pos] = ch;
			_size += 1;

			return *this;
		}


		string& insert(size_t pos, const char* str)
		{
			assert(pos <= _size);
			int len = strlen(str);

			if (_size + len > _capacity)
				reserve(_size + len);

			size_t end = _size + len;
			while (end>pos+len-1)
			{
				_str[end] = _str[end - len];
				--end;
			}

			// ��������
			strncpy(_str + pos, str, len);     // strncpy����ʱ����׷��\0
			_size += len;

			return *this;
		}

		string& erase(size_t pos,size_t len=npos)
		{
			assert(pos < _size);

			if (len == npos || len + pos >= _size)
			{
				_str[pos] = '\0';
				_size = pos;
			}
			else
			{
				strcpy(_str+pos, _str+pos + len);
				_size -= len;
			}
			return *this;
		}


		void push_back(char ch)
		{
			if (_size + 1 >= _capacity)
			{
				reserve(2 * _capacity);
			}

			_str[_size] = ch;
			++_size;

			_str[_size] = '\0';    // һ��Ҫ���ַ���ĩβ��\0

			//insert(_size, ch);
		}

		void append(const char* str)
		{
			size_t len = strlen(str);

			if (_size + len > _capacity)
			{
				reserve(_size + len);
			}

			strcpy(_str + _size, str);
			_size += len;

			//insert(_size, str);
		}

		size_t size()
		{
			return _size;
		}

		size_t capacity()
		{
			return _capacity;
		}

		void swap(string&s)
		{
			std::swap(_str, s._str);
			std::swap(_size, s._size);
			std::swap(_capacity, s._capacity);
		}

		const char* c_str()
		{
			return _str;
		}

		void clear()
		{
			_size = 0;
			_str[0] = '\0';
		}

		char& operator[](size_t pos)
		{
			assert(pos < _size);
			return _str[pos];
		}

		const char& operator[](size_t pos)const
		{
			assert(pos < _size);
			return _str[pos];
		}

	private:
		char* _str;
		size_t _size;
		size_t _capacity;

		static const size_t npos;
	};

	const size_t string::npos = -1;

	void test1()
	{
		string s1;
		string s2("abc");
		string s3(s2);
		string s4("hello");
		s4 = s2;
	}

	void test2()
	{
		string s("hello");
		std::cout << s.c_str() << std::endl;
		s.clear();
	}

	void test3()
	{
		string s("hello world");
		//yj::string::iterator it = s.begin();
		yj::string::const_iterator it = s.begin();
		for (; it != s.end(); ++it)
		{
			//*it = '8';
			std::cout << *it;
		}

		std::cout<<std::endl;
		std::cout << s[0];
	}

	void test4()
	{
		string s = "abc";
		s.push_back('h');
		s.append("hello world");
		//s.insert(0, 'z');
		//s.insert(4, 'z');
		//s.insert(3, "hehe!");
		//s.erase(1, 28);
		std::cout << s.c_str();
	}


	void test5()
	{
		string s = "abcdr";
		s.resize(6,'t');
		//s.push_back('h');
		//s.append("hello world");
		std::cout << s.c_str();
	}
}