#pragma once


#include"RBTree.h"


namespace yj
{
	template<class K,class V>
	class map
	{
	public:
		struct MapKeyOfT
		{
			const K& operator()(const std::pair<const K,V>&kv)
			{
				return kv.first;
			}
		};
	public:
		// typename: 告诉编译器取类模板的内嵌类型
		typedef typename RBTree<K, std::pair<const K, V>, MapKeyOfT>::iterator iterator;
		typedef typename RBTree<K, std::pair<const K, V>, MapKeyOfT>::const_iterator const_iterator;

		iterator begin()
		{
			return _t.begin();
		}

		iterator end()
		{
			return _t.end();
		}

		const_iterator begin()const
		{
			return _t.begin();
		}

		const_iterator end()const
		{
			return _t.end();
		}

		V& operator[](const K& key)
		{
			std::pair<iterator, bool> ret = _t.Insert(std::make_pair(key, V()));
			return ret.first->second;
		}

		std::pair<iterator, bool> insert(const std::pair<const K, V>& kv)
		{
			return _t.Insert(kv);
		}

	private:
		RBTree<K, std::pair<const K, V>, MapKeyOfT> _t;
	};
}