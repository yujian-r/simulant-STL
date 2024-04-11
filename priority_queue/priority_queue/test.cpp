
#include"priority_queue.h"

//int main()
//{
//	//yj::test1();
//	//yj::test2();
//
//}


void adjust_down(std::vector<int>&nums, int parent, int n)
{
	int child = parent * 2 + 1;
	while (child < n)
	{
		// 大堆: 找孩子节点中最大的
		if (child + 1 < n && nums[child]<nums[child + 1])
			++child;

		if (nums[child]>nums[parent])
		{
			std::swap(nums[child], nums[parent]);
			parent = child;
			child = parent * 2 + 1;
		}
		else
		{
			break;
		}
	}
}


void heap_sort(std::vector<int>& arr)
{
	int n = arr.size();

	for (int i = (n -1-1) / 2; i >= 0; --i)
	{
		adjust_down(arr,i,n);
	}

	int end = n - 1;
	while (end > 0)
	{
		std::swap(arr[end], arr[0]);
		adjust_down(arr, 0, end);
		--end;
	}
}

int main()
{
	std::vector<int> v{ 100,3,2,7,6,0,4,1,9,8,5,88,-1 };
	heap_sort(v);
	for (auto& e : v)
		std::cout << e << " ";
}