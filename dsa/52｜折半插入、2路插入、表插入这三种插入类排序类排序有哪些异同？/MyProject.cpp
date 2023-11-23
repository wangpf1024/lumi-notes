// MyProject.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>


#ifdef _DEBUG   //只在Debug（调试）模式下
#ifndef DEBUG_NEW
#define DEBUG_NEW new(_NORMAL_BLOCK,__FILE__,__LINE__) //重新定义new运算符
#define new DEBUG_NEW
#endif
#endif

//#include <boost/type_index.hpp>
using namespace std;
#pragma warning(disable : 4996) 
//#pragma warning(disable : 6386) 



//------------------------------
namespace _nmsp1
{
	//直接插入排序（从小到大）
	template<typename T>  //T代表数组元素类型
	void InsertSort(T myarray[], int length) //myarray：要排序的数组元素，length：数组中元素数量
	{
		if (length <= 1) //不超过1个元素的数组，没必要排序
			return; 

		for (int i = 1; i < length; ++i) //从第2个元素（下标为1）开始比较
		{
			if (myarray[i] <	 myarray[i - 1]) 
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

	//折半插入排序（从小到大）
	template<typename T>  
	void HalfInsertSort(T myarray[], int length)
	{
		if (length <= 1) //不超过1个元素的数组，没必要排序
			return;

		for (int i = 1; i < length; ++i) //从第2个元素（下标为1）开始比较
		{
			if (myarray[i] < myarray[i - 1])
			{
				T temp = myarray[i];   //暂存a[i]值，防止后续移动元素时值被覆盖

				//记录查找的左右区间范围
				int left = 0;
				int right = i - 1;

				while (left <= right) //注意while结束条件
				{
					//取得中间元素
					int mid = (right - left) / 2 + left;
					if (myarray[mid] > temp)
					{
						//待排的元素值更小，将搜索的区间缩小到左边的一半区域
						right = mid - 1;
					}
					else
					{
						//待排的元素值更大，将搜索区间你缩小到右边的一半区域
						left = mid + 1;
					}
				} //end while

				int j;
				for (j = i - 1; j >= right + 1; --j) 
				{
					myarray[j + 1] = myarray[j];
				} //end for j
				myarray[right + 1] = temp;
			} //end if
		} //end for i
		return;
	}
	//2路插入排序（从小到大）
	template<typename T>
	void TwoWayInsertSort(T myarray[], int length)
	{
		if (length <= 1) //不超过1个元素的数组，没必要排序
			return;

		T* pfzarray = new T[length]; //创建辅助数组空间
		pfzarray[0] = myarray[0];  //将原始数组空间的第一个元素放入辅助数组空间第1个位置

		int head = 0; //头指针指向第一个位置（下标为0的元素）
		int tail = 0; //尾指针指向第一个位置（下标为0的元素）

		for (int i = 1; i < length; ++i)
		{
			if (myarray[i] < pfzarray[head]) //小于头
			{
				//往头前面插入
				head = (head - 1 + length) % length; //要保证head值在0到(length-1)之间
				pfzarray[head] = myarray[i];
			}
			else if (myarray[i] > pfzarray[tail]) //大于尾
			{
				tail++;
				pfzarray[tail] = myarray[i];
			}
			else //数据既不小于头，也不大于尾，要往中间插入
			{
				//这里要移动数据了
				tail++;
				pfzarray[tail] = pfzarray[tail - 1];
				int j;
				for (j = tail - 1; myarray[i] < pfzarray[(j - 1 + length) % length]; j = (j - 1 + length) % length)
				{
					pfzarray[j] = pfzarray[(j - 1 + length) % length];
				}//end for j
				pfzarray[j] = myarray[i];
			} //end if
		} //end for i

		for (int i = 0; i < length; ++i)
		{
			myarray[i] = pfzarray[head];
			head = (head + 1) % length;
		} //end for i


		delete[] pfzarray;
		return;
	}

	//静态链表中每个节点的定义
	template <typename T> //T代表数据元素的类型
	struct SLNode
	{
		T        data;  //元素数据域，存放数据元素
		int      cur;   //游标，记录下个静态链表节点的数组下标
	};

	//表插入排序（从小到大）
	template<typename T>
	void TableInsertSort(T myarray[], int length)
	{
		if (length <= 1) //不超过1个元素的数组，没必要排序
			return;

		SLNode<T>* tbl = new SLNode<T>[length + 1];
		for (int i = 1; i < (length + 1); ++i) //注意i的开始值
		{
			tbl[i].data = myarray[i - 1];
			tbl[i].cur = 0;
		} //end for i		
		tbl[0].cur = 1; //头节点指向下标为1的位置
		for (int i = 2; i < (length + 1); ++i)
		{
			int tmpcur = tbl[0].cur; //1
			int tmpcur_prev = 0; //前趋

			while (tmpcur != 0 && tbl[tmpcur].data < tbl[i].data)
			{
				tmpcur_prev = tmpcur;
				tmpcur = tbl[tmpcur].cur;
			} //end while
			tbl[i].cur = tbl[tmpcur_prev].cur;
			tbl[tmpcur_prev].cur = i;
		} //end for i

		int tmpcur = tbl[0].cur;
		int curridx = 0; //数组下标
		while (tmpcur != 0)
		{
			myarray[curridx] = tbl[tmpcur].data;
			++curridx;
			tmpcur = tbl[tmpcur].cur;
		} //end while

		delete[] tbl;
		return;
	}

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

}

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);//程序退出时检测内存泄漏并显示到“输出”窗口


	/*
	 
	int arr[] = {16,1,45,23,99,2,18,67,42,10};
	//int arr[] = { 1, 2, 10 ,16, 18, 23, 42, 45, 67, 99 };

	int length = sizeof(arr) / sizeof(arr[0]);   //数组中元素个数
	//_nmsp1::InsertSort(arr, length); //对数组元素进行直接插入排序
	//_nmsp1::HalfInsertSort(arr, length); //对数组元素进行折半插入排序
	//_nmsp1::TwoWayInsertSort(arr, length);//对数组元素进行2路插入排序
	_nmsp1::TableInsertSort(arr, length);//对数组元素进行表插入排序


	//输出排好序的数组中元素内容
	//cout << "直接插入排序结果为：";
	//cout << "折半插入排序结果为：";
	//cout << "2路插入排序结果为："; 
	cout << "表插入排序结果为：";
	for (int i = 0; i < length; ++i)
	{
		cout << arr[i] << "  ";
	}
	cout << endl; //换行
	*/

	int arr[] = { 67,1,45,23,99,2,18,16,42,10,8,44,106,29,4 };
	int length = sizeof(arr) / sizeof(arr[0]);   //数组中元素个数
	_nmsp1::ShellSort(arr, length);//对数组元素进行希尔插入排序

	cout << "希尔排序最终结果为：";
	for (int i = 0; i < length; ++i)
		cout << arr[i] << "  ";
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




