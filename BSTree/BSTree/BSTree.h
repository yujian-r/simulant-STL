#pragma once

#include<iostream>
#include<utility>
#include<vector>
#include<string>

namespace yj
{
	template<class K>
	struct BSTreeNode
	{
		BSTreeNode<K>* _left;
		BSTreeNode<K>* _right;
		K _key;

		BSTreeNode(K key)
			:_left(nullptr)
			, _right(nullptr)
			, _key(key)
		{

		}
	};

	template<class K>
	class BSTree
	{
		typedef BSTreeNode<K> Node;
	protected:

		void Destroy(Node*& root)
		{
			if (root == nullptr)
				return;

			// 先销毁左子树, 在销毁右子树, 最后销毁根
			Destroy(root->_left);
			Destroy(root->_right);
			delete root;
			root = nullptr;
		}


		Node* Copy(Node* root)
		{
			if (root == nullptr)
				return nullptr;

			// 先拷贝左子树再拷贝右子树
			Node* newroot = new Node(root->_key);
			newroot->_left = Copy(root->_left);
			newroot->_right = Copy(root->_right);
			return newroot;
		}

	public:

		BSTree() = default;

		BSTree(const BSTree<K>& t)
		{
			_root = Copy(t._root);
		}

		BSTree<K>& operator=(BSTree<K> t)
		{
			std::swap(_root, t._root);
			return *this;
		}

		~BSTree()
		{
			Destroy(_root);
		}

		bool Find(const K& key)
		{
			Node* cur = _root;
			while (cur)
			{
				if (cur->_key < key)
					cur = cur->_right;
				else if (cur->_key > key)
					cur = cur->_left;
				else
					return true;
			}
			return false;
		}

		bool Insert(const K& key)
		{
			// 1. 查找插入位置

			// 第一次插入 --- 树为空, 给根节点赋值
			if (_root == nullptr)
			{
				_root = new Node(key);
				return true;
			}

			Node* parent = nullptr;
			Node* cur = _root;

			while (cur)
			{
				if (cur->_key < key)
				{
					parent = cur;
					cur = cur->_right;
				}
				else if (cur->_key > key)
				{
					parent = cur;
					cur = cur->_left;
				}
				else  // 二叉搜索树不允许插入重复值
				{
					return false;
				}
			}

			// 2. 链接
			cur = new Node(key);
			if (cur->_key < parent->_key)
				parent->_left = cur;
			else
				parent->_right = cur;
			return true;
		}

		bool Erase(const K& key)
		{
			Node* parent = nullptr;
			Node* cur = _root;

			while (cur)
			{
				if (cur->_key < key)
				{
					parent = cur;
					cur = cur->_right;
				}
				else if (cur->_key > key)
				{
					parent = cur;
					cur = cur->_left;
				}
				else  // 找到要删除的节点
				{
					// 3种情况: 1.左为空  2.右为空  3.左右都为空
					if (cur->_left == nullptr)
					{
						if (cur == _root)
						{
							_root = cur->_right;
						}
						else
						{
							if (parent->_right == cur)
								parent->_right = cur->_right;
							else
								parent->_left = cur->_right;
						}
						delete cur;
					}
					else if (cur->_right == nullptr)
					{
						if (cur == _root)
						{
							_root = cur->_left;
						}
						else
						{
							if (parent->_right == cur)
								parent->_right = cur->_left;
							else
								parent->_left = cur->_left;
						}
						delete cur;
					}
					else   // 左右都为空
					{
						// 找左最大/右最小;

						// 找右最小为例:
						Node* pminright = cur;
						Node* minright = cur->_right;

						while (minright->_left)
						{
							pminright = minright;
							minright = minright->_left;
						}

						std::swap(cur->_key, minright->_key);

						if (pminright->_right == minright)
							pminright->_right = minright->_right;
						else
							pminright->_left = minright->_right;

						delete minright;
					}
					return true;
				}
			}
			return false;
		}

		void Inorder()
		{
			_Inorder(_root);
			std::cout << std::endl;
		}

		bool FindR(const K& key)
		{
			return _FindR(_root,key);
		}

		bool InsertR(const K& key)
		{
			return _InsertR(_root, key);
		}

		bool EraseR(const K& key)
		{
			return _EraseR(_root, key);
		}

	private:
		void _Inorder(Node*root)
		{
			if (root == nullptr)
				return;

			_Inorder(root->_left);
			std::cout << root->_key << " ";
			_Inorder(root->_right);
		}

		bool _FindR(Node* root, const K& key)
		{
			if (root == nullptr)
				return false;

			if (root->_key == key)
				return true;

			if (root->_key < key)
				return _FindR(root->_right, key);
			else
				return _FindR(root->_left, key);
		}

		bool _InsertR(Node*& root, const K& key)   // *&相当于是: &root的别名
		{
			if (root == nullptr)
			{
				root = new Node(key);
				return true;
			}

			if (root->_key < key)
				return _InsertR(root->_right, key);
			else if (root->_key > key)
				return _InsertR(root->_left, key);
			else
				return false;
		}

		bool _EraseR(Node*& root, const K& key)
		{
			if (root == nullptr)
				return false;

			if (root->_key < key)
				return _EraseR(root->_right, key);
			else if (root->_key > key)
				return _EraseR(root->_left, key);
			else
			{
				Node* del = root;

				// 3种情况
				if (root->_right == nullptr)
				{
					root = root->_left;
				}
				else if (root->_left == nullptr)
				{
					root = root->_right;
				}
				else
				{
					Node* maxleft = root->_left;
					while (maxleft->_right)
					{
						maxleft = maxleft->_right;
					}
					
					std::swap(maxleft->_key, root->_key);

					// 转换成在左子树中删除
					return _EraseR(root->_left, key);
				}
				delete del;
				return true;
			}
		}

	private:
		Node* _root=nullptr;
	};

	void test1()
	{
		BSTree<int> t;
		t.Insert(1);
		t.Insert(3);
		t.Insert(-1);
		t.Insert(-8);
		t.Insert(18);
		t.Insert(10);
		t.Insert(-8);

		std::cout << t.Find(-8) << std::endl;
	}

	void test2()
	{
		//std::vector<int> num= {8, 3, 1, 10, 6, 4, 7, 14, 13};
		std::vector<int> num = { 8, 3, 1, 10, 6, 4, 7, 14, 13,16,22 };

		BSTree<int> t;
		for (auto& e : num)
			t.Insert(e);

		t.Inorder();

		std::cout << "-----------------------" << std::endl;

		for (auto& e : num)
		{
			t.Erase(e);
			t.Inorder();
		}
	}

	void test3()
	{
		std::vector<int> num = { 8, 3, 1, 10, 6, 4, 7, 14, 13,16,22 };

		BSTree<int> t;
		for (auto& e : num)
			t.InsertR(e);

		t.Inorder();

		std::cout << t.FindR(5) << std::endl;

		std::cout << "-----------------------" << std::endl;

		for (auto& e : num)
		{
			t.EraseR(e);
			t.Inorder();
		}
	}

	void test4()
	{
		std::vector<int> num = { 8, 3, 1, 10, 6, 4, 7, 14, 13,16,22 };

		BSTree<int> t;
		for (auto& e : num)
			t.InsertR(e);
		t.Inorder();

		BSTree<int> t1;
		t1.Insert(1);
		t1 = t;
		t1.Inorder();
	}
}




namespace kv
{
	template<class K,class V>
	struct BSTreeNode
	{
		BSTreeNode<K,V>* _left;
		BSTreeNode<K,V>* _right;
		K _key;
		V _val;

		BSTreeNode(K key,V val)
			:_left(nullptr)
			, _right(nullptr)
			, _key(key)
			,_val(val)
		{

		}
	};

	template<class K,class V>
	class BSTree
	{
		typedef BSTreeNode<K,V> Node;
	protected:

		void Destroy(Node*& root)
		{
			if (root == nullptr)
				return;

			// 先销毁左子树, 在销毁右子树, 最后销毁根
			Destroy(root->_left);
			Destroy(root->_right);
			delete root;
			root = nullptr;
		}

		Node* Copy(Node* root)
		{
			if (root == nullptr)
				return nullptr;

			// 先拷贝左子树再拷贝右子树
			Node* newroot = new Node(root->_key);
			newroot->_left = Copy(root->_left);
			newroot->_right = Copy(root->_right);
			return newroot;
		}

	public:

		BSTree() = default;

		BSTree(const BSTree<K,V>& t)
		{
			_root = Copy(t._root);
		}

		BSTree<K,V>& operator=(BSTree<K,V> t)
		{
			std::swap(_root, t._root);
			return *this;
		}

		~BSTree()
		{
			Destroy(_root);
		}

		Node* Find(const K& key)
		{
			Node* cur = _root;
			while (cur)
			{
				if (cur->_key < key)
					cur = cur->_right;
				else if (cur->_key > key)
					cur = cur->_left;
				else
					return cur;
			}
			return nullptr;
		}

		bool Insert(const K& key,const V&val)
		{
			// 1. 查找插入位置

			// 第一次插入 --- 树为空, 给根节点赋值
			if (_root == nullptr)
			{
				_root = new Node(key,val);
				return true;
			}

			Node* parent = nullptr;
			Node* cur = _root;

			while (cur)
			{
				if (cur->_key < key)
				{
					parent = cur;
					cur = cur->_right;
				}
				else if (cur->_key > key)
				{
					parent = cur;
					cur = cur->_left;
				}
				else  // 二叉搜索树不允许插入重复值
				{
					return false;
				}
			}

			// 2. 链接
			cur = new Node(key,val);
			if (cur->_key < parent->_key)
				parent->_left = cur;
			else
				parent->_right = cur;
			return true;
		}

		bool Erase(const K& key)
		{
			Node* parent = nullptr;
			Node* cur = _root;

			while (cur)
			{
				if (cur->_key < key)
				{
					parent = cur;
					cur = cur->_right;
				}
				else if (cur->_key > key)
				{
					parent = cur;
					cur = cur->_left;
				}
				else  // 找到要删除的节点
				{
					// 3种情况: 1.左为空  2.右为空  3.左右都为空
					if (cur->_left == nullptr)
					{
						if (cur == _root)
						{
							_root = cur->_right;
						}
						else
						{
							if (parent->_right == cur)
								parent->_right = cur->_right;
							else
								parent->_left = cur->_right;
						}
						delete cur;
					}
					else if (cur->_right == nullptr)
					{
						if (cur == _root)
						{
							_root = cur->_left;
						}
						else
						{
							if (parent->_right == cur)
								parent->_right = cur->_left;
							else
								parent->_left = cur->_left;
						}
						delete cur;
					}
					else   // 左右都为空
					{
						// 找左最大/右最小;

						// 找右最小为例:
						Node* pminright = cur;
						Node* minright = cur->_right;

						while (minright->_left)
						{
							pminright = minright;
							minright = minright->_left;
						}

						std::swap(cur->_key, minright->_key);

						if (pminright->_right == minright)
							pminright->_right = minright->_right;
						else
							pminright->_left = minright->_right;

						delete minright;
					}
					return true;
				}
			}
			return false;
		}

		void Inorder()
		{
			_Inorder(_root);
			std::cout << std::endl;
		}

	private:
		void _Inorder(Node*root)
		{
			if (root == nullptr)
				return;

			_Inorder(root->_left);
			std::cout << root->_key << " " << root->_val << std::endl;
			_Inorder(root->_right);
		}
	private:
		Node* _root=nullptr;
	};

	void test1()
	{
		// 统计水果出现的次数
		std::string arr[] = { "苹果", "西瓜", "苹果", "西瓜", "苹果", "苹果", "西瓜",
		"苹果", "香蕉", "苹果", "香蕉","椰子"};
		kv::BSTree<std::string, int> countTree;
		for (auto& str : arr)
		{
			// 先查找水果在不在搜索树中
			// 1、不在，说明水果第一次出现，则插入<水果, 1>
			// 2、在，则查找到的节点中水果对应的次数++

			kv::BSTreeNode<std::string, int>* ret = countTree.Find(str);

			if (ret == nullptr)
			{
				countTree.Insert(str, 1);
			}
			else
			{
				ret->_val++;
			}
		}
		countTree.Inorder();
	}

	void test2()
	{
		kv::BSTree<std::string, std::string> dict;

		dict.Insert("string", "字符串");
		dict.Insert("tree", "树");
		dict.Insert("left", "左边、剩余");
		dict.Insert("right", "右边");
		dict.Insert("sort", "排序");

		std::string str;
		while (std::cin >> str)
		{
			kv::BSTreeNode<std::string, std::string>* ret = dict.Find(str);

			if (ret)
			{
				std::cout << ":" << ret->_val << std::endl;
			}
			else
			{
				std::cout << "无此单词" << std::endl;
			}
		}
	}
}