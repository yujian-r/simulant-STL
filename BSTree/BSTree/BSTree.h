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

			// ������������, ������������, ������ٸ�
			Destroy(root->_left);
			Destroy(root->_right);
			delete root;
			root = nullptr;
		}


		Node* Copy(Node* root)
		{
			if (root == nullptr)
				return nullptr;

			// �ȿ����������ٿ���������
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
			// 1. ���Ҳ���λ��

			// ��һ�β��� --- ��Ϊ��, �����ڵ㸳ֵ
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
				else  // ��������������������ظ�ֵ
				{
					return false;
				}
			}

			// 2. ����
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
				else  // �ҵ�Ҫɾ���Ľڵ�
				{
					// 3�����: 1.��Ϊ��  2.��Ϊ��  3.���Ҷ�Ϊ��
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
					else   // ���Ҷ�Ϊ��
					{
						// �������/����С;

						// ������СΪ��:
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

		bool _InsertR(Node*& root, const K& key)   // *&�൱����: &root�ı���
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

				// 3�����
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

					// ת��������������ɾ��
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

			// ������������, ������������, ������ٸ�
			Destroy(root->_left);
			Destroy(root->_right);
			delete root;
			root = nullptr;
		}

		Node* Copy(Node* root)
		{
			if (root == nullptr)
				return nullptr;

			// �ȿ����������ٿ���������
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
			// 1. ���Ҳ���λ��

			// ��һ�β��� --- ��Ϊ��, �����ڵ㸳ֵ
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
				else  // ��������������������ظ�ֵ
				{
					return false;
				}
			}

			// 2. ����
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
				else  // �ҵ�Ҫɾ���Ľڵ�
				{
					// 3�����: 1.��Ϊ��  2.��Ϊ��  3.���Ҷ�Ϊ��
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
					else   // ���Ҷ�Ϊ��
					{
						// �������/����С;

						// ������СΪ��:
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
		// ͳ��ˮ�����ֵĴ���
		std::string arr[] = { "ƻ��", "����", "ƻ��", "����", "ƻ��", "ƻ��", "����",
		"ƻ��", "�㽶", "ƻ��", "�㽶","Ҭ��"};
		kv::BSTree<std::string, int> countTree;
		for (auto& str : arr)
		{
			// �Ȳ���ˮ���ڲ�����������
			// 1�����ڣ�˵��ˮ����һ�γ��֣������<ˮ��, 1>
			// 2���ڣ�����ҵ��Ľڵ���ˮ����Ӧ�Ĵ���++

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

		dict.Insert("string", "�ַ���");
		dict.Insert("tree", "��");
		dict.Insert("left", "��ߡ�ʣ��");
		dict.Insert("right", "�ұ�");
		dict.Insert("sort", "����");

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
				std::cout << "�޴˵���" << std::endl;
			}
		}
	}
}