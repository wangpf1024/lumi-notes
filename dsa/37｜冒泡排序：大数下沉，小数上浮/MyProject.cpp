// MyProject.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

#include <iostream>
#include <iomanip>
#include <list>
#include <vector>

using namespace std;
#pragma warning(disable : 4996) 


//冒泡排序（从小到大）
template<typename T>
void BubbleSort(T myarray[], int length)
{
	if (length <= 1) //不超过1个元素的数组，没必要排序
		return;

	//外层循环只控制排序的趟数
	for (int i = 0; i < length - 1; ++i)
	{
		bool cgflag = false; //表本趟冒泡排序是否发生过记录交换，false：无；true：有
		//内层循环控制元素的大小比较和交换位置
		for (int j = 0; j < length - i - 1; ++j) //每趟比较的次数都会减少
		{
			if (myarray[j] > myarray[j + 1])  //前面的数据如果比后面的数据大
			{
				//交换元素位置
				T temp = myarray[j + 1];
				myarray[j + 1] = myarray[j];
				myarray[j] = temp;

				cgflag = true; //标记本趟冒泡排序发生过记录交换(可能1次或者多次)
			}
		} //end for j
		if (cgflag == false) //本趟冒泡排序没有发生过记录交换，表示整个冒泡排序结束
			break;

		//每走一趟显示一下结果
		cout << "第" << i + 1 << "趟冒泡排序结果为： ";
		for (int i = 0; i < length; ++i) cout << myarray[i] << "  ";
		cout << endl;
	} //end for i
	return;
}

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);//程序退出时检测内存泄漏并显示到“输出”窗口

	int arr[] = { 16,1,45,23,99,2,18,67,42,10 };
	int length = sizeof(arr) / sizeof(arr[0]);   //数组中元素个数
	BubbleSort(arr, length);//对数组元素进行冒泡排序

	cout << "冒泡排序结果为：";
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




