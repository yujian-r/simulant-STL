#pragma once

#include<utility>
#include<vector>
#include<iostream>
#include<cassert>
#include<algorithm>

namespace yj
{
	template<class K, class V>
	struct AVLTreeNode
	{
		// 三叉链形式
		AVLTreeNode<K, V>* _left;
		AVLTreeNode<K, V>* _right;
		AVLTreeNode<K, V>* _parent;
		std::pair<K, V> _kv;
		int _bf;              // 平衡因子

		AVLTreeNode(const std::pair<K, V>& kv)
			:_left(nullptr)
			,_right(nullptr)
			,_parent(nullptr)
			,_kv(kv)
			,_bf(0)
		{

		}
	};


	template<class K,class V>
	class AVLTree
	{
		typedef AVLTreeNode<K, V> Node;
	public:
		bool Insert(const std::pair<K, V>& kv)
		{
			// 1. 找插入位置

			if (_root == nullptr)
			{
				_root=new Node(kv);
				return true;
			}

			Node* cur = _root;
			Node* parent = nullptr;
			while (cur)
			{
				if (cur->_kv.first < kv.first)
				{
					parent = cur;
					cur = cur->_right;
				}
				else if(cur->_kv.first > kv.first)
				{
					parent = cur;
					cur = cur->_left;
				}
				else
				{
					return false;
				}
			}

			// 2. 链接
			cur = new Node(kv);
			if (kv.first < parent->_kv.first)
				parent->_left = cur;
			else
				parent->_right = cur;
			cur->_parent = parent;


			// 3. 更新平衡因子
			while (parent)
			{
				if (parent->_left == cur)
					--parent->_bf;
				else
					++parent->_bf;

				// 分情况讨论:
				if (parent->_bf == 1 || parent->_bf == -1)
				{
					parent = parent->_parent;
					cur = cur->_parent;
				}
				else if (parent->_bf==0)
				{
					break;
				}
				else if (parent->_bf == 2 || parent->_bf == -2)
				{
					if (parent->_bf == 2 && cur->_bf == 1)    // 左单旋
					{
						RotateL(parent);
					}
					else if (parent->_bf == -2 && cur->_bf == -1)   // 右单旋
					{
						RotateR(parent);
					}
					else if (parent->_bf==-2 && cur->_bf==1)
					{
						RotateLR(parent);
					}
					else if (parent->_bf == 2 && cur->_bf == -1)
					{
						RotateRL(parent);
					}
					else
					{
						assert(false);
					}
					break;
				}
				else
				{
					assert(false);
				}
			}
			return true;
		}


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
			parent->_bf = subR->_bf = 0;      // 更新平衡因子
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
			parent->_bf = subL->_bf = 0;
		}


		void RotateLR(Node*parent)
		{
			Node* subL = parent->_left;
			Node* subLR = subL->_right;
			int bf = subLR->_bf;

			RotateL(subL);
			RotateR(parent);

			if (bf==1)
			{
				subLR->_bf = 0;
				subL->_bf = -1;
				parent->_bf = 0;
			}
			else if (bf==-1)
			{
				subLR->_bf = 0;
				subL->_bf = 0;
				parent->_bf = 1;
			}
			else if (bf==0)
			{
				subL->_bf = 0;
				subLR->_bf = 0;
				parent->_bf = 0;
			}
			else
			{
				assert(false);
			}
		}


		void RotateRL(Node* parent)
		{
			Node* subR = parent->_right;
			Node* subRL = subR->_left;
			int bf = subRL->_bf;

			RotateR(subR);
			RotateL(parent);

			if (bf == 1)
			{
				subRL->_bf = 0;
				subR->_bf = 0;
				parent->_bf = -1;
			}
			else if (bf == -1)
			{
				subRL->_bf = 0;
				subR->_bf = 1;
				parent->_bf = 0;
			}
			else if (bf == 0)
			{
				subR->_bf = 0;
				subRL->_bf = 0;
				parent->_bf = 0;
			}
			else
			{
				assert(false);
			}
		}


		void Inorder()
		{
			_Inorder(_root);
			std::cout << std::endl;
		}


		bool IsBalance()
		{
			return _IsBalance(_root);
		}

	private:
		void _Inorder(Node* root)
		{
			if (root == nullptr)
				return;

			_Inorder(root->_left);
			std::cout << root->_kv.first << " " << root->_bf << std::endl;;
			_Inorder(root->_right);
		}

		int _Height(Node*root)
		{
			if (root == nullptr)
				return 0;

			int left = _Height(root->_left);
			int right = _Height(root->_right);
			return std::max(left, right) + 1;
		}

		bool _IsBalance(Node* root)
		{
			if (root == nullptr)
				return true;

			int leftH = _Height(root->_left);
			int rightH = _Height(root->_right);

			if (rightH - leftH != root->_bf)
			{
				std::cout << root->_kv.first << "节点平衡因子异常" << std::endl;
				return false;
			}

			return std::abs(leftH - rightH) < 2 
				&& _IsBalance(root->_left) 
				&& _IsBalance(root->_right);
		}
	private:
		Node* _root = nullptr;
	};



	void test1()
	{
		//std::vector<int> num= {8, 3, 1, 10, 6, 4, 7, 14, 13};
		std::vector<int> num = { 8, 3, 1, 10, 6, 4, 7, 14, 13,16,22 ,-99};

		AVLTree<int,int> t;
		for (auto& e : num)
			t.Insert({ e,1 });

		t.IsBalance();

		t.Inorder();
	}
}