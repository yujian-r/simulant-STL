#pragma once

#include<utility>
#include<vector>
#include<iostream>
#include<string>
#include<algorithm>

namespace OpenAddress
{
	// 标记哈希表中元素的状态
	enum State
	{
		EMPTY,
		EXIST,
		DELETE
	};

	template<class K, class V>
	struct HashData
	{
		std::pair<K, V> _kv;
		State _state = EMPTY;
	};

	template<class K,class V>
	class HashTable
	{
	public:
		// 传统写法
		//bool Insert(const std::pair<K, V>& kv)
		//{
		//	if (_tables.size() == 0)
		//	{
		//		_tables.resize(10);
		//	}

		//	// 控制负载因子, 负载因子 = 表中元素个数 / 表实际长度
		//	// 这里控制负载因子超过0.7就扩容
		//	
		//	// 此写法防止除0错误
		//	while ( _n*10 / _tables.size() >= 7)
		//	{
		//		size_t newsize = _tables.size() * 2;

		//		// 开新表 --- 将旧表元素映射到新表
		//		std::vector<HashData<K, V>> newtables(newsize);

		//		for (auto& e : _tables)
		//		{
		//			if (e._state == EXIST)    // 旧表中元素存在
		//			{
		//				size_t hashi = e._kv.first % newtables.size();   // 计算在新表中的位置

		//				size_t i = 1;
		//				size_t index = hashi;
		//				while (newtables[index]._state == EXIST)
		//				{
		//					index = hashi + i;
		//					index %= newtables.size();
		//					++i;
		//				}
		//				newtables[index]._kv = e._kv;
		//				newtables[index]._state = EXIST;
		//			}
		//		}
		//		_tables.swap(newtables);
		//	}

		//	size_t hashi = kv.first % _tables.size();    // 要插入的位置

		//	// 线性探测
		//	size_t i = 1;
		//	size_t index = hashi;
		//	while (_tables[index]._state == EXIST)
		//	{
		//		index = hashi + i;        // 二次探测 index = hashi + i*i
		//		index %= _tables.size();
		//		++i;
		//	}

		//	_tables[index]._kv = kv;
		//	_tables[index]._state = EXIST;
		//	++_n;

		//	return true;
		//}


		// 现代写法
		bool Insert(const std::pair<K, V>& kv)
		{
			if (Find(kv.first))  // 元素已经存在, 就不要插入
			{
				return false;
			}

			// 控制负载因子, 负载因子 = 表中元素个数 / 表实际长度
			// 这里控制负载因子超过0.7就扩容

			// 此写法防止除0错误
			while (_tables.size()==0 || _n * 10 / _tables.size() >= 7)
			{
				size_t newsize = _tables.size()==0?10:_tables.size() * 2;

				HashTable<K, V> newht;     // 重新创建一个对象
				newht._tables.resize(newsize);

				// 先重新映射旧表的关系, 映射完后, 再去插入新的元素
				for (auto& e : _tables)
				{
					if (e._state == EXIST)    // 旧表中元素存在
					{
						newht.Insert(e._kv);
					}
				}
				_tables.swap(newht._tables);
			}

			size_t hashi = kv.first % _tables.size();    // 要插入的位置

			// 线性探测
			size_t i = 1;
			size_t index = hashi;
			while (_tables[index]._state == EXIST)
			{
				index = hashi + i;        // 二次探测 index = hashi + i*i
				index %= _tables.size();
				++i;
			}

			_tables[index]._kv = kv;
			_tables[index]._state = EXIST;
			++_n;

			return true;
		}

		HashData<K, V>* Find(const K&key)
		{
			if (_tables.size() == 0)
				return nullptr;

			size_t hashi = key % _tables.size();

			size_t i = 1;
			size_t index = hashi;
			while (_tables[index]._state != EMPTY)
			{
				if (_tables[index]._state == EXIST && _tables[index]._kv.first == key)
				{
					return &_tables[index];
				}
				
				index = hashi + i;
				index %= _tables.size();
				++i;
				
				// 已经查找一圈还未找到, 说明全是存在 + 删除
				if (index == hashi)
				{
					break;
				}
			}
			return nullptr;
		}

		// 伪删除法: 没有真正地把这个数据删除, 只是把这位置对应的状态标记为删除
		bool Erase(const K& key)
		{
			HashData<K, V>* ret = Find(key);  // 查找的是地址
			if (ret)
			{
				ret->_state = DELETE;
				--_n;
				return true;
			}
			else
			{
				return false;
			}
		}
	private:
		std::vector<HashData<K, V>> _tables;
		size_t _n=0;    // 存储有效数据的个数
	};
}



namespace HashBucket
{
	template<class K,class V>
	struct HashNode
	{
		HashNode<K, V>* _next;
		std::pair<K, V> _kv;

		HashNode(std::pair<K, V> kv)
			:_next(nullptr)
			,_kv(kv)
		{

		}
	};

	template<class K>
	struct HashFunc
	{
		size_t operator()(const K& key)
		{
			return key;
		}
	};


	//模板特化  --- 实现字符串比较不用自己显示传仿函数, 和库里保持一致
	template<>
	struct HashFunc<std::string>
	{
		//BKDR算法
		size_t operator()(const std::string& s)
		{
			size_t hash = 0;
			for (auto ch : s)
			{
				hash += ch;
				hash *= 31;
			}
			return hash;
		}
	};

	template<class K, class V,class Hash=HashFunc<K>>
	class HashTable
	{
	public:

		// 除留余数法扩容
		//size_t newsize = GetNextPrime(_tables.size());
		size_t GetNextPrime(size_t prime)
		{
			// SGI  --- 除留余数法模素数
			static const int __stl_num_primes = 28;
			static const unsigned long __stl_prime_list[__stl_num_primes] =
			{
				53, 97, 193, 389, 769,
				1543, 3079, 6151, 12289, 24593,
				49157, 98317, 196613, 393241, 786433,
				1572869, 3145739, 6291469, 12582917, 25165843,
				50331653, 100663319, 201326611, 402653189, 805306457,
				1610612741, 3221225473, 4294967291
			};

			size_t i = 0;
			for (; i < __stl_num_primes; ++i)
			{
				if (__stl_prime_list[i] > prime)
					return __stl_prime_list[i];
			}

			return __stl_prime_list[i];
		}


		~HashTable()
		{
			for (auto& cur : _tables)
			{
				while (cur)
				{
					HashNode<K, V>* next = cur->_next;
					delete cur;
					cur = next;
				}
				cur = nullptr;
			}
		}

		bool Insert(const std::pair<K, V>& kv)
		{
			Hash hash;

			if (Find(kv.first))    // 元素已经存在就不要插入
			{
				return false;
			}

			while (_n == _tables.size())
			{
				size_t newsize = GetNextPrime(_tables.size());    //模素数
				//size_t newsize = _tables.size() == 0 ? 10 : _tables.size() * 2;

				std::vector<HashNode<K, V>*> newtables(newsize, nullptr);

				for (auto& cur : _tables)
				{
					while (cur)
					{
						HashNode<K, V>* next = cur->_next;

						size_t hashi = hash(cur->_kv.first) % newtables.size();
						cur->_next = newtables[hashi];
						newtables[hashi] = cur;

						cur = next;
					}
				}
				_tables.swap(newtables);
			}

			size_t hashi = hash(kv.first) % _tables.size();

			// 头插的逻辑
			HashNode<K, V>* newnode = new HashNode<K, V>(kv);
			newnode->_next = _tables[hashi];
			_tables[hashi] = newnode;

			++_n;
			return true;
		}

		HashNode<K, V>* Find(const K& key)
		{
			Hash hash;

			if (_tables.size() == 0)
				return nullptr;

			size_t hashi = hash(key) % _tables.size();
			HashNode<K, V>* cur = _tables[hashi];
			while (cur)
			{
				if (cur->_kv.first == key)
					return cur;
				cur = cur->_next;
			}
			return nullptr;
		}

		bool Erase(const K& key)
		{
			Hash hash;

			size_t hashi = hash(key)%_tables.size();
			HashNode<K, V>* cur = _tables[hashi];
			HashNode<K, V>* prev = nullptr;

			while (cur)    // 链表头删
			{
				if (cur->_kv.first == key)
				{
					if (prev == nullptr)
					{
						_tables[hashi] = cur->_next;
					}
					else
					{
						prev->_next = cur->_next;
					}

					delete cur;
					return true;
				}
				else
				{
					prev = cur;
					cur = cur->_next;
				}
			}
			return false;
		}

		size_t MaxBucketSize()
		{
			size_t maxcount = 0;

			for (int i = 0; i < _tables.size(); ++i)
			{
				HashNode<K, V>* cur = _tables[i];
				size_t size = 0;

				while (cur)
				{
					++size;
					cur = cur->_next;
				}

				printf("[%d]->[%d]\n", i, size);
				maxcount = std::max(maxcount, size);
			}
			return maxcount;
		}

	private:
		std::vector<HashNode<K, V>*> _tables;
		size_t _n = 0;				// 有效数据个数
	};
}