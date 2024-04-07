#pragma once
#include<utility>
#include<iostream>
#include<cassert>
#include<string>

namespace yj
{
	template<class T>
	struct list_node
	{
		T _val;
		list_node<T>* _prev;
		list_node<T>* _next;

		list_node(const T&val=T ())
			:_val(val)
			,_prev(nullptr)
			,_next(nullptr)
		{

		}
	};

	// 反向迭代器: 封装正向迭代器
	template<class iterator, class Ref,class Ptr>
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
			return *--tmp;                // 返回_cur的前一个位置
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

	template<class T,class Ref,class Ptr>
	struct _list_iterator
	{
		typedef list_node<T> node;
		typedef _list_iterator<T, Ref, Ptr> self;
		node* _node;

		_list_iterator(node*node)
			:_node(node)
		{

		}

		Ref operator*()
		{
			return _node->_val;
		}

		Ptr operator->()
		{
			return &(_node->_val);
		}

		self& operator++()
		{
			_node = _node->_next;
			return *this;
		}

		self& operator--()
		{
			_node = _node->_prev;
			return *this;
		}

		bool operator==(const self& s)
		{
			return _node == s._node;
		}

		bool operator!=(const self& s)
		{
			return _node != s._node;
		}

		self operator++(int)
		{
			self tmp(*this);
			_node = _node->_next;
			return tmp;
		}

		self operator--(int)
		{
			self tmp(*this);
			_node = _node->_prev;
			return tmp;
		}
	};

	template<class T>
	class list
	{
		typedef list_node<T> node;
		typedef _list_iterator<T,T&,T*> iterator;
		typedef _list_iterator<T, const T&,const T*> const_iterator;
		typedef reverse_iterator<const_iterator, const T&, const T*> const_reverse_iterator;
		typedef reverse_iterator<iterator, T&, T*> reverse_iterator;

	public:

		iterator begin()
		{	
			// 直接构造匿名对象返回
			return iterator(_head->_next);  
		}

		iterator end()
		{
			return iterator(_head);
		}


		reverse_iterator rbegin()
		{
			return iterator(end());
		}

		reverse_iterator rend()
		{
			return iterator(begin());
		}

		const_iterator begin()const
		{
			// 直接构造匿名对象返回
			return const_iterator(_head->_next);
		}

		const_iterator end()const
		{
			return const_iterator(_head);
		}

		void empty_list()
		{
			_head = new node;
			_head->_prev = _head;
			_head->_next = _head;
		}

		list()
		{
			empty_list();
		}

		
		template<class InputIterator>
		list(InputIterator first, InputIterator last)
		{
			empty_list();  // push_back的前提

			while (first != last)
			{
				push_back(*first);
				++first;
			}
		}


		list(const list<T>& lt)
		{
			empty_list();     // 初始化*this节点

			list<T> tmp(lt.begin(), lt.end());
			swap(tmp);
		}

		// lt1=lt3
		list<T>& operator=(list<T> lt)
		{
			swap(lt);
			return *this;
		}

		~list()
		{
			clear();
			delete _head;
			_head = nullptr;
		}

		void clear()
		{
			// 头删法删除
			iterator it = begin();
			while (it != end())
			{
				it = erase(it);
			}
		}

		iterator insert(iterator pos, const T& val)
		{
			node* cur = pos._node;
			node* prev = cur->_prev;

			node* newnode = new node(val);
			newnode->_next = cur;
			newnode->_prev = prev;
			prev->_next = newnode;
			cur->_prev = newnode;

			return iterator(pos);
		}


		iterator erase(iterator pos)
		{
			assert(pos != end());

			node* cur = pos._node;
			node* prev = cur->_prev;
			node* next = cur->_next;
			prev->_next = next;
			next->_prev = prev;

			delete cur;

			return iterator(next);  // 返回删除后的下一个位置
		}

		void push_back(const T& val)
		{
			//node* tail = _head->_prev;
			//node* newnode = new node(val);

			//tail->_next = newnode;
			//newnode->_prev = tail;
			//newnode->_next = _head;
			//_head->_prev = newnode;

			insert(end(), val);
		}


		void push_front(const T& val)
		{
			insert(begin(), val);
		}

		void pop_front()
		{
			erase(begin());
		}

		void pop_back()
		{
			erase(--end());
		}

		void swap(list<T>& tmp)
		{
			std::swap(_head, tmp._head);
		}

	private:
		node* _head;
	};

	void test1()
	{
		list<int> list;
		list.push_back(1);
		list.push_back(2);
		list.push_back(3);
		list.push_back(4);

		yj::list<int> list2;
		list2.push_back(7);
		list2.push_back(8);

		list2 = list;
	}


	void test2()
	{
		list<int> list;
		list.push_back(1);
		list.push_back(2);
		list.push_back(3);
		list.push_back(4);

		auto it = list.begin();
		//*it = 8;
		std::cout << *it << std::endl;
		for (auto it = list.begin(); it != list.end(); ++it)
		{
			std::cout << *it << " ";
		}
	}

	void test3()
	{
		list<int> list;
		list.push_back(1);
		list.push_back(2);
		list.push_back(3);
		list.push_back(4);

		yj::list<int> list2(list.begin(),list.end());
		yj::list<int> list3(list);
	}

	void test4()
	{
		list<int> list;
		list.push_back(1);
		list.push_back(2);
		list.push_back(3);
		list.push_back(4);

		list.push_front(7);
		//list.pop_front();
		list.pop_back();
		list.pop_back();
		//auto it = list.insert(list.end(), 8);
		//list.erase(++list.begin());
		//list.erase(--list.end());
		for (auto it = list.begin(); it != list.end(); ++it)
		{
			std::cout << *it << " ";
		}
	}


	void test5()
	{
		list<int> list;
		list.push_back(1);
		list.push_back(2);
		list.push_back(3);
		list.push_back(4);
		list.push_back(5);

		for (auto rit = list.rbegin(); rit != list.rend(); ++rit)
			std::cout << *rit << " ";
	}


	void test6()
	{
		list<std::string> list;
		list.push_back("1111");
		list.push_back("2222");
		list.push_back("njdsnvksvn");
		list.push_back("vjsnvknvk");
		list.push_back("vksvlsvnlsnvls");

		for (auto rit = list.rbegin(); rit != list.rend(); ++rit)
			std::cout << rit->size() << " ";
	}

}