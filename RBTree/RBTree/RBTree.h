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

	template<class K,class V>
	struct RBTreeNode
	{
		RBTreeNode<K, V>* _left;
		RBTreeNode<K, V>* _right;
		RBTreeNode<K, V>* _parent;
		std::pair<K, V> _kv;
		Colour _col;

		RBTreeNode(const std::pair<K, V>& kv)
			:_left(nullptr)
			,_right(nullptr)
			,_parent(nullptr)
			,_kv(kv)
			,_col(RED)      // Ĭ�ϲ����ڵ�
		{

		}
	};

	template<class K,class V>
	class RBTree
	{
		typedef RBTreeNode<K, V> Node;
	public:

		~RBTree()
		{
			_Destroy(_root);
			_root = nullptr;
		}

		bool Insert(const std::pair<K,V>&kv)
		{
			if (_root == nullptr)
			{
				_root = new Node(kv);
				_root->_col = BLACK;    // ���ڵ��Ǻ�ɫ
				return true;
			}

			Node* cur = _root;
			Node* parent = nullptr ;

			while (cur)
			{
				if (cur->_kv.first < kv.first)
				{
					parent = cur;
					cur = cur->_right;
				}
				else if (cur->_kv.first > kv.first)
				{
					parent = cur;
					cur = cur->_left;
				}
				else
				{
					return false;
				}
			}

			cur = new Node(kv);
			if (cur->_kv.first < parent->_kv.first)
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
			return true;
		}

		void Inorder()
		{
			_Inorder(_root);
		}

		int Height()
		{
			return _Height(_root);
		}

		bool IsBalance()   // �ص������
		{
			if (_root && _root->_col == RED)
			{
				std::cout << "���ڵ��Ǻ�ɫ" << std::endl;
				return false;
			}

			int benchmark = 0;   // ��׼ֵ(�ڽڵ�����)
			Node* cur = _root;
			while (cur)          // ������·��
			{
				if (cur->_col == BLACK)
					++benchmark;

				cur = cur->_left;
			}

			return _Check(_root, 0, benchmark);
		}

		bool _Check(Node* root, int blackNum, int benchmark)
		{
			if (root == nullptr)     // ����Ҳ�Ǻ����
			{
				if (blackNum != benchmark)
				{
					std::cout << "ĳ��·���Ϻ�ɫ�ڵ�����������" << std::endl;
					return false;
				}
				return true;
			}

			if (root->_col == BLACK)
				++blackNum;

			// ������  --- ��ڵ㲻������
			if (root->_col == RED && root->_parent && root->_parent->_col == RED)
			{
				std::cout << "���������ĺ�ڵ�" << std::endl;
				return false;
			}

			return _Check(root->_left, blackNum, benchmark)
				&& _Check(root->_right, blackNum, benchmark);
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

		int _Height(Node* root)
		{
			if (root == nullptr)
				return 0;

			int leftH = _Height(root->_left);
			int rightH = _Height(root->_right);

			return leftH > rightH ? leftH + 1 : rightH + 1;
		}

		void _Destroy(Node* root)
		{
			if (root == nullptr)
				return;

			_Destroy(root->_left);
			_Destroy(root->_right);
			delete root;
		}
		
	private:
		Node* _root = nullptr;
	};


	void test1()
	{
		//std::vector<int> num= {8, 3, 1, 10, 6, 4, 7, 14, 13};
		std::vector<int> num = { 8, 3, 1, 70,10, 6, 4, 7, 14, 13,16,22 ,-99 };

		RBTree<int, int> t;
		for (auto& e : num)
			t.Insert({ e,1 });

		/*t.IsBalance();*/

		t.Inorder();
		t.IsBalance();
	}
}