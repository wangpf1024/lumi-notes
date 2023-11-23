// MyProject.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <iostream>

//#include <boost/type_index.hpp>
using namespace std;
#pragma warning(disable : 4996) 
//#pragma warning(disable : 6386) 

//------------------------------
//直接插入排序（从小到大）
template<typename T>  //T代表数组元素类型
void InsertSort(T myarray[], int length) //myarray：要排序的数组元素，length：数组中元素数量
{
	if (length <= 1) //不超过1个元素的数组，没必要排序
		return;

	for (int i = 1; i < length; ++i) //从第2个元素（下标为1）开始比较
	{
		if (myarray[i] < myarray[i - 1])
		{
			T temp = myarray[i];   //暂存myarray[i]值，防止后续移动元素时值被覆盖			

			int j;
			for (j = i - 1; j >= 0 && myarray[j] > temp; --j) //检查所有前面排好序的元素
			{
				myarray[j + 1] = myarray[j]; //所有大于temp的元素都向后移动
			} //end for j
			myarray[j + 1] = temp; //复制数据到插入位置，注意j因为被减了1，这里加回来
		} //end if
	} //end for i
	return;
}

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);//程序退出时检测内存泄漏并显示到“输出”窗口


	int arr[] = { 16,1,45,23,99,2,18,67,42,10 };

	int length = sizeof(arr) / sizeof(arr[0]);   //数组中元素个数
	InsertSort(arr, length); //对数组元素进行直接插入排序

	//输出排好序的数组中元素内容
	cout << "直接插入排序结果为：";
	for (int i = 0; i < length; ++i)
	{
		cout << arr[i] << " ";
	}
	cout << endl; //换行

	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件




