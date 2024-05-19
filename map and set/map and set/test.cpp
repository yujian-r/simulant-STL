
#include<string>
#include"map.h"
#include"set.h"



void test1()
{
	yj::set<int> set;
	set.insert(8);
	set.insert(3);
	set.insert(10);
	set.insert(14);
	set.insert(2);
	set.insert(6);
	set.insert(7);
	set.insert(1);
	set.insert(9);

	yj::set<int>::iterator it = set.begin();
	for ( ; it != set.end(); ++it)
	{
		std::cout << *it << " ";
	}
	std::cout << std::endl;
}


void test2()
{
	yj::map<int,int> map;
	map.insert(std::make_pair(1,2 ));
	map.insert(std::make_pair(8, 2));
	map.insert(std::make_pair(6, 2));
	map.insert(std::make_pair(5, 2));
	map.insert(std::make_pair(21, 2));


	yj::map<int,int>::iterator it = map.begin();
	for (; it != map.end(); ++it)
	{
		std::cout << it->first << " ";
	}
	std::cout << std::endl;
}


void test_map3()
{
	std::string arr[] = { "Î÷¹Ï", "Î÷¹Ï", "Æ»¹û", "Î÷¹Ï", "Æ»¹û",
		"Æ»¹û", "Î÷¹Ï", "Æ»¹û", "Ïã½¶", "Æ»¹û", "Ïã½¶", "Àæ","Ã¢¹û"};
	yj::map<std::string, int> countMap;
	for (auto& e : arr)
	{
		countMap[e]++;
	}

	for (auto& kv : countMap)
	{
		std::cout << kv.first << ":" << kv.second << std::endl;
	}
}

int main()
{
	//test1();
	//test2();

	test_map3();

	return 0;
}