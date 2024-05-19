#pragma once

#include<utility>
#include<iostream>
#include<vector>

namespace yj
{
	enum Colour
	{
		RED,
		BLACK
	};

	template<class T>
	struct RBTreeNode
	{
		RBTreeNode<T>* _left;
		RBTreeNode<T>* _right;
		RBTreeNode<T>* _parent;
		T _data;
		Colour _col;

		RBTreeNode(const T& data)
			:_left(nullptr)
			,_right(nullptr)
			,_parent(nullptr)
			,_data(data)
			,_col(RED)      // 默认插入红节点
		{

		}
	};


	template<class T, class Ref, class Ptr>
	struct _RBTreeIterator
	{
		typedef _RBTreeIterator<T, Ref, Ptr> self;
		typedef RBTreeNode<T> Node;
		Node* _node;

		_RBTreeIterator(Node* node)
			:_node(node)
		{

		}

		// 实例化成iterator, 就是拷贝构造
		// 实例化成const_iterator, 就是构造函数(支持用iterator去构造初始化const_iterator的构造函数)
		// 支持从普通迭代器隐式类型转换 成构造const迭代器的构造函数
		_RBTreeIterator(const _RBTreeIterator<T,T&,T*>&it)
			:_node(it._node)
		{

		}

		Ref operator*()
		{
			return _node->_data;
		}

		Ptr operator->()
		{
			return &(_node->_data);
		}

		bool operator!=(const self&s)
		{
			return _node != s._node;
		}

		self& operator++()
		{
			if (_node->_right)
			{
				// 1. 右不为空, 找右子树最左节点
				Node* subLeft = _node->_right;
				while (subLeft->_left)
				{
					subLeft = subLeft->_left;
				}
				_node = subLeft;
			}
			else
			{
				// 2. 右为空, 沿着到根路径, 找孩子是父亲左的那个节点
				Node* cur = _node;
				Node* parent = cur->_parent;

				while (parent && cur == parent->_right)
				{
					cur = parent;
					parent = cur->_parent;
				}
				_node = parent;
			}
			return *this;
		}


		self& operator--()
		{
			if (_node->_left)
			{
				// 1. 左不为空, 找左子树最右节点
				Node* subRight = _node->_left;
				while (subRight->_right)
				{
					subRight = subRight->_right;
				}
				_node = subRight;
			}
			else
			{
				// 2. 左为空, 沿着到根路径, 找孩子是父亲右的那个节点
				Node* cur = _node;
				Node* parent = cur->_parent;

				while (parent && cur == parent->_left)
				{
					cur = parent;
					parent = cur->_parent;
				}
				_node = parent;
			}
			return *this;
		}
	};


	template<class K,class T, class KeyofT>
	class RBTree
	{
		typedef RBTreeNode<T> Node;
	public:

		typedef _RBTreeIterator<T, T&, T*> iterator;
		typedef _RBTreeIterator<T, const T&, const T*> const_iterator;


		iterator begin()    // 中序遍历最左节点
		{
			Node* cur = _root;

			while (cur && cur->_left)
			{
				cur = cur->_left;
			}
			return iterator(cur);     // 节点指针构造迭代器
		}

		iterator end()
		{
			return iterator(nullptr);
		}

		const_iterator begin()const
		{
			Node* cur = _root;

			while (cur && cur->_left)
			{
				cur = cur->_left;
			}
			return const_iterator(cur);    
		}

		const_iterator end()const
		{
			return const_iterator(nullptr);
		}

		std::pair<iterator,bool> Insert(const T& data)
		{
			if (_root == nullptr)
			{
				_root = new Node(data);
				_root->_col = BLACK;    // 根节点是黑色
				return std::make_pair(iterator(_root), true);
			}

			KeyofT kot;        // 仿函数对象, 提取出data中的key
			Node* cur = _root;
			Node* parent = nullptr;

			while (cur)
			{
				if (kot(cur->_data)<kot(data))
				{
					parent = cur;
					cur = cur->_right;
				}
				else if (kot(cur->_data) > kot(data))
				{
					parent = cur;
					cur = cur->_left;
				}
				else
				{
					return std::make_pair(iterator(cur), false);
				}
			}

			cur = new Node(data);
			Node* newnode = cur;    // cur可能会变色, 需要提前记录

			if (kot(cur->_data) < kot(parent->_data))
				parent->_left = cur;
			else
				parent->_right = cur;
			cur->_parent = parent;


			// 处理红黑树颜色
			while (parent && parent->_col == RED)
			{
				Node* grandfather = parent->_parent;

				// 找叔叔节点 => 看父节点在祖父节点哪边
				if (grandfather->_left == parent)
				{
					Node* uncle = grandfather->_right;

					// 3种情况

					// 1.u存在且为红 -> p变红, g变黑, 继续向上调整
					if (uncle && uncle->_col == RED)
					{
						grandfather->_col = RED;
						parent->_col = BLACK;
						uncle->_col = BLACK;

						cur = grandfather;
						parent = cur->_parent;
					}
					else   // 2+3. u不存在/u存在且为黑 -> 旋转 + 变色
					{
						//      g
						//   p     u
						// c      
						if (cur == parent->_left)
						{
							RotateR(grandfather);
							grandfather->_col = RED;
							parent->_col = BLACK;
						}
						else
						{
							//      g
							//   p     u
							//      c     
							RotateL(parent);
							RotateR(grandfather);
							grandfather->_col = RED;
							cur->_col = BLACK;
						}
						break;
					}
				}
				else  // grandfather->_right == parent
				{
					Node* uncle = grandfather->_left;

					// 3种情况

					// 1. u存在, 且为红
					if (uncle && uncle->_col == RED)
					{
						parent->_col = BLACK;
						uncle->_col = BLACK;
						grandfather->_col = RED;

						// 继续向上调整
						cur = grandfather;
						parent = cur->_parent;
					}
					else      // 2+3. u不存在/u存在且为黑 -> 旋转 + 变色
					{
						//     g
						//  u     p
						//          c
						if (cur==parent->_right)
						{
							RotateL(grandfather);
							grandfather->_col = RED;
							parent->_col = BLACK;
						}
						else
						{
							//     g
							//  u     p
							//     c
							RotateR(parent);
							RotateL(grandfather);
							grandfather->_col = RED;
							cur->_col = BLACK;
						}
						break;
					}
				}
			}
			_root->_col = BLACK;    // 根节点是黑色
			return std::make_pair(iterator(newnode), true);
		}

		void Inorder()
		{
			_Inorder(_root);
		}

	private:
		void RotateL(Node* parent)
		{
			Node* subR = parent->_right;
			Node* subRL = subR->_left;
			Node* ppnode = parent->_parent;

			parent->_right = subRL;
			if (subRL)
				subRL->_parent = parent;

			subR->_left = parent;
			parent->_parent = subR;

			if (ppnode == nullptr)     // parent本身是根
			{
				_root = subR;
				_root->_parent = nullptr;
			}
			else                      // parent是子树
			{
				// 判断是左右哪颗子树
				if (ppnode->_left == parent)
				{
					subR->_parent = ppnode;
					ppnode->_left = subR;
				}
				else
				{
					subR->_parent = ppnode;
					ppnode->_right = subR;
				}
			}
		}

		void RotateR(Node* parent)
		{
			Node* subL = parent->_left;
			Node* subLR = subL->_right;
			Node* ppnode = parent->_parent;

			parent->_left = subLR;
			if (subLR)
				subLR->_parent = parent;

			subL->_right = parent;
			parent->_parent = subL;

			if (ppnode == nullptr)
			{
				_root = subL;
				_root->_parent = nullptr;
			}
			else
			{
				if (ppnode->_left == parent)
				{
					subL->_parent = ppnode;
					ppnode->_left = subL;
				}
				else
				{
					subL->_parent = ppnode;
					ppnode->_right = subL;
				}
			}
		}

		void _Inorder(Node* root)
		{
			if (root == nullptr)
				return;

			_Inorder(root->_left);
			std::cout << root->_kv.first << " ";
			_Inorder(root->_right);
		}

	private:
		Node* _root = nullptr;
	};

}