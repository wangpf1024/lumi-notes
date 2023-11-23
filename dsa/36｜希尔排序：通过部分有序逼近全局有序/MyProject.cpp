// MyProject.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

#include <iostream>
#include <iomanip>
#include <list>
#include <vector>

using namespace std;
#pragma warning(disable : 4996) 

//希尔排序（从小到大）
template<typename T>
void ShellSort(T myarray[], int length)
{
	if (length <= 1) //不超过1个元素的数组，没必要排序
		return;

	//显示一下原始数据
	//15个元素：【67   1   45   23   99    2   18   16   42   10    8     44   106    29    4】
	//下标：    【0】【1】【2】【3】【4】【5】【6】【7】【8】【9】【10】【11】【12】【13】【14】
	cout << "原始数据为：";
	for (int i = 0; i < length; ++i) cout << myarray[i] << "  ";
	cout << endl;

	int Dim = length / 2; // Dim：增量，取值分别为7、3、1
	while (Dim >= 1)
	{
		//每一趟采用直接插入排序来实现(实现代码于插入排序其实是一摸一样)
		//第一次while循环：i=7~14；第二次while循环：i=3~14；第三次while循环i=1~14；
		for (int i = Dim; i < length; ++i) //i值每次改变可以处理到不同的子序列
		{
			if (myarray[i] < myarray[i - Dim])
			{
				T temp = myarray[i];//暂存myarray[i]值，防止后续移动元素时值被覆盖
				int j;
				for (j = i - Dim; j >= 0 && myarray[j] > temp; j -= Dim) //检查所有前面排好序的元素
				{
					//所有大于temp的元素都向后移动
					myarray[j + Dim] = myarray[j]; //大于temp的元素都向后移动
				} //end for j
				myarray[j + Dim] = temp; //复制数据到插入位置，注意j因为被减了Dim，这里加回来
			}//end if		
		} //end for i	

		//每走一趟显示一下结果
		cout << "本趟希尔排序，增量为:" << Dim << "  " << "结果为： ";
		for (int i = 0; i < length; ++i) cout << myarray[i] << "  ";
		cout << endl;

		Dim /= 2; //增量每次减半
	} //end while(Dim >= 1)
	return;
}

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);//程序退出时检测内存泄漏并显示到“输出”窗口

	int arr[] = { 67,1,45,23,99,2,18,16,42,10,8,44,106,29,4 };
	int length = sizeof(arr) / sizeof(arr[0]);   //数组中元素个数
	ShellSort(arr, length);//对数组元素进行希尔插入排序

	cout << "希尔排序最终结果为：";
	for (int i = 0; i < length; ++i)
		cout << arr[i] << " ";
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




