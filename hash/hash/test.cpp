#include"HashTable.h"

void test_hashtable1()
{
	int a[] = { 3, 33, 2, 13, 5, 12, 1002 };
	OpenAddress::HashTable<int, int> ht;

	for (auto e : a)
	{
		ht.Insert(std::make_pair(e, e));
	}
	ht.Insert(std::make_pair(15, 15));

	if (ht.Find(13))
	{
		std::cout << "13在" << std::endl;
	}
	else
	{
		std::cout << "13不在" << std::endl;
	}

	ht.Erase(13);
	if (ht.Find(13))
	{
		std::cout << "13在" << std::endl;
	}
	else
	{
		std::cout << "13不在" << std::endl;
	}
}


void test_hashtable2()
{
	int a[] = { 3, 33, 2, 13, 5, 12, 1002 };
	HashBucket::HashTable<int, int> ht;

	for (auto e : a)
	{
		ht.Insert(std::make_pair(e, e));
	}
	ht.Insert(std::make_pair(15, 15));

	if (ht.Find(13))
	{
		std::cout << "13在" << std::endl;
	}
	else
	{
		std::cout << "13不在" << std::endl;
	}

	ht.Erase(13);
	if (ht.Find(13))
	{
		std::cout << "13在" << std::endl;
	}
	else
	{
		std::cout << "13不在" << std::endl;
	}
}


void test_HashTable3()
{
	HashBucket::HashTable<std::string, std::string> ht;
	ht.Insert(std::make_pair("sort", "排序"));
	ht.Insert(std::make_pair("string", "字符串"));
	ht.Insert(std::make_pair("left", "左边"));
	ht.Insert(std::make_pair("right", "右边"));
	ht.Insert(std::make_pair("", "右边"));

	if (ht.Find("string"))
	{
		std::cout << "string在" << std::endl;
	}
	else
	{
		std::cout << "string不在" << std::endl;
	}

	ht.Erase("string");
	if (ht.Find("string"))
	{
		std::cout << "string在" << std::endl;
	}
	else
	{
		std::cout << "string不在" << std::endl;
	}
}


void test_HashTable4()
{
	size_t N = 100000;
	HashBucket::HashTable<int, int> ht;
	srand(time(0));
	for (size_t i = 0; i < N; ++i)
	{
		size_t x = rand() + i;
		ht.Insert(std::make_pair(x, x));
	}

	std::cout << ht.MaxBucketSize() << std::endl;
}

//int main()
//{
//	//test_hashtable1();
//	//test_hashtable2();
//	test_HashTable3();
//	//test_HashTable4();
//}




//std::vector<int> path;
//int n;
//
//void dfs(int i)
//{
//	if (i>n)
//	{
//		for (int j = 0; j < path.size(); ++j)
//			std::cout << path[j] << " ";
//		std::cout << std::endl;
//		return;
//	}
//
//	// 选
//	path.push_back(i);
//	dfs(i + 1);
//	path.pop_back();
//
//	// 不选
//	dfs(i + 1);
//}
//
//int main()
//{
//	std::cin >> n;
//	dfs(1);
//}



//std::vector<int> path;
//int n;
//
//void dfs(int pos)
//{
//	for (int j = 0; j < path.size(); ++j)
//			std::cout << path[j] << " ";
//		std::cout << std::endl;
//
//	for (int i = pos; i <= n; ++i)
//	{
//		path.push_back(i);
//		dfs(i + 1);
//		path.pop_back();
//	}
//}
//
//int main()
//{
//	std::cin >> n;
//	dfs(1);
//}

//int longestConsecutive(std::vector<int>& nums) {
//
//	if (nums.size() == 0)
//		return 0;
//
//	sort(nums.begin(), nums.end());
//
//	int ret = 1, count = 1;
//	for (int i = 1; i < nums.size(); ++i)
//	{
//		if (nums[i] == nums[i - 1])
//		{
//			continue;
//		}
//		else if (nums[i] - nums[i - 1] == 1)
//		{
//			++count;
//			ret = std::max(count, ret);
//		}
//		else
//		{
//			count = 1;
//		}
//	}
//	return ret;
//}


int main()
{
	//std::vector<int> nums = { 1,2,0,1 };
	//longestConsecutive(nums);

}