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
			,_col(RED)      // Ĭ�ϲ����ڵ�
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

		// ʵ������iterator, ���ǿ�������
		// ʵ������const_iterator, ���ǹ��캯��(֧����iteratorȥ�����ʼ��const_iterator�Ĺ��캯��)
		// ֧�ִ���ͨ��������ʽ����ת�� �ɹ���const�������Ĺ��캯��
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
				// 1. �Ҳ�Ϊ��, ������������ڵ�
				Node* subLeft = _node->_right;
				while (subLeft->_left)
				{
					subLeft = subLeft->_left;
				}
				_node = subLeft;
			}
			else
			{
				// 2. ��Ϊ��, ���ŵ���·��, �Һ����Ǹ�������Ǹ��ڵ�
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
				// 1. ��Ϊ��, �����������ҽڵ�
				Node* subRight = _node->_left;
				while (subRight->_right)
				{
					subRight = subRight->_right;
				}
				_node = subRight;
			}
			else
			{
				// 2. ��Ϊ��, ���ŵ���·��, �Һ����Ǹ����ҵ��Ǹ��ڵ�
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


		iterator begin()    // �����������ڵ�
		{
			Node* cur = _root;

			while (cur && cur->_left)
			{
				cur = cur->_left;
			}
			return iterator(cur);     // �ڵ�ָ�빹�������
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
				_root->_col = BLACK;    // ���ڵ��Ǻ�ɫ
				return std::make_pair(iterator(_root), true);
			}

			KeyofT kot;        // �º�������, ��ȡ��data�е�key
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
			Node* newnode = cur;    // cur���ܻ��ɫ, ��Ҫ��ǰ��¼

			if (kot(cur->_data) < kot(parent->_data))
				parent->_left = cur;
			else
				parent->_right = cur;
			cur->_parent = parent;


			// ����������ɫ
			while (parent && parent->_col == RED)
			{
				Node* grandfather = parent->_parent;

				// ������ڵ� => �����ڵ����游�ڵ��ı�
				if (grandfather->_left == parent)
				{
					Node* uncle = grandfather->_right;

					// 3�����

					// 1.u������Ϊ�� -> p���, g���, �������ϵ���
					if (uncle && uncle->_col == RED)
					{
						grandfather->_col = RED;
						parent->_col = BLACK;
						uncle->_col = BLACK;

						cur = grandfather;
						parent = cur->_parent;
					}
					else   // 2+3. u������/u������Ϊ�� -> ��ת + ��ɫ
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

					// 3�����

					// 1. u����, ��Ϊ��
					if (uncle && uncle->_col == RED)
					{
						parent->_col = BLACK;
						uncle->_col = BLACK;
						grandfather->_col = RED;

						// �������ϵ���
						cur = grandfather;
						parent = cur->_parent;
					}
					else      // 2+3. u������/u������Ϊ�� -> ��ת + ��ɫ
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
			_root->_col = BLACK;    // ���ڵ��Ǻ�ɫ
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

			if (ppnode == nullptr)     // parent�����Ǹ�
			{
				_root = subR;
				_root->_parent = nullptr;
			}
			else                      // parent������
			{
				// �ж��������Ŀ�����
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