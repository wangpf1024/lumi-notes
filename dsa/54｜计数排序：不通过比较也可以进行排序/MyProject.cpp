// MyProject.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
//#include <assert.h>
//#include<malloc.h> /* malloc()等 */
//#include<stdio.h> /* EOF(=^Z或F6),NULL */
//#include<process.h> /* exit() */
#include <list>

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
	/*
	//计数排序	
	template<typename T>
	void CountSort(T myarray[], int length, T maxvalue) //maxvalue：要排序的关键字最大值
	{
		T* pArrayCount = new T[maxvalue];		
		for (int i = 0; i < maxvalue; ++i) //清0
		{
			pArrayCount[i] = 0;
		}

		for (int i = 0; i < length; ++i)
		{
			pArrayCount[myarray[i]] ++;
		}

		int idx = 0; //下标索引
		for (int i = 0; i < maxvalue; ++i)
		{
			if (pArrayCount[i] == 0) //没计数的自然忽略掉
				continue;

			//有计数
			for (int j = 0; j < pArrayCount[i]; ++j)
			{
				myarray[idx] = i;
				idx++;
			}
		}
		delete[] pArrayCount;
		return;
	}
	*/
	
	/*
	//计数排序（稳定）（从小到大）
	template<typename T>
	void CountSort(T myarray[], int length, T maxvalue) //maxvalue：要排序的关键字最大值
	{
		T* pArrayCount = new T[maxvalue];
		for (int i = 0; i < maxvalue; ++i) //清0
		{
			pArrayCount[i] = 0;
		}

		for (int i = 0; i < length; ++i)
		{
			pArrayCount[myarray[i]] ++;
		}
		
		for (int i = 1; i < maxvalue; ++i)
		{
			pArrayCount[i] = pArrayCount[i] + pArrayCount[i - 1];
		} //end for

		//创建辅助数组
		T* pFZArray = new T[length]; 

		//倒序遍历要排序的数组
		for (int i = length - 1; i >= 0; --i)
		{
			int idx = pArrayCount[myarray[i]] - 1;
			pFZArray[idx] = myarray[i];
			pArrayCount[myarray[i]]--;
		} //end for 

		//把排好序的内容写回来
		for (int i = 0; i < length; ++i)
		{
			myarray[i] = pFZArray[i];
		}
		//释放内存
		delete[] pFZArray;
		delete[] pArrayCount;
		return;
	}
	*/

	//计数排序（稳定）（从大到小）
	template<typename T>
	void CountSort(T myarray[], int length, T maxvalue) //maxvalue：要排序的关键字最大值
	{
		T* pArrayCount = new T[maxvalue];
		for (int i = 0; i < maxvalue; ++i) //清0
		{
			pArrayCount[i] = 0;
		}

		for (int i = 0; i < length; ++i)
		{
			pArrayCount[myarray[i]] ++;
		}

		//for (int i = 1; i < maxvalue; ++i)
		//{
		//	pArrayCount[i] = pArrayCount[i] + pArrayCount[i - 1];
		//} //end for
		for (int i = maxvalue - 1; i > 0; --i)
		{
			pArrayCount[i - 1] = pArrayCount[i - 1] + pArrayCount[i];
		}

		//创建辅助数组
		T* pFZArray = new T[length];

		//倒序遍历要排序的数组
		for (int i = length - 1; i >= 0; --i)
		{
			int idx = pArrayCount[myarray[i]] - 1;
			pFZArray[idx] = myarray[i];
			pArrayCount[myarray[i]]--;
		} //end for 

		//把排好序的内容写回来
		for (int i = 0; i < length; ++i)
		{
			myarray[i] = pFZArray[i];
		}
		//释放内存
		delete[] pFZArray;
		delete[] pArrayCount;
		return;
	}

	
	//基数排序
	template<typename T>
	void RadixSort(T myarray[], int length)
	{
		if (length <= 1) //不超过1个元素的数组，没必要排序
			return;

		T* pResult = new T[length]; //新数组，用于保存每趟排序的结果
		
		//借用C++标准库中的list容器保存必要的信息，当然也可以用自己写的链表来保存数据
		std::list<T *> mylist[10]; //#include <list>  ，注意list中的<>里的数据类型

		//3，意味着分别取得个位、十位、百位 数字
		for (int i = 0; i < 3; ++i) //为简化代码，假设已经知道待排序数字最大不超过3位，所以这里就直接写i < 3了
		{				
			//(1)分配
			for (int j = 0; j < length; ++j)
			{				
				//根据i值来决定取得某个数字的个位、十位、百位
				int tmpi = i;
				T tmpvalue = myarray[j];
				T lastvalue;   //取得的个位、十位、百位数字保存在这里
				while (tmpi >= 0)
				{
					lastvalue = tmpvalue % 10;
					tmpvalue /= 10;
					tmpi--;
				} //end while

				mylist[lastvalue].push_back(&myarray[j]);  //在list尾部插入元素
			} //end for j

			//(2)收集
			int idx = 0;
			for (int k = 0; k < 10; ++k)
			{
				for (auto iter = mylist[k].begin(); iter != mylist[k].end(); ++iter)
				{
					pResult[idx] = *( * (iter));
					idx++;
				} //end iter
				mylist[k].clear(); //清空mylist,为下次向其中存数据做准备
			} //end i

			//(3)把数据拷贝回myarray
			for (int m = 0; m < length; ++m)
			{
				myarray[m] = pResult[m];
			}			
		} //end for i

		delete[] pResult;

		return;
	}
}

int main()
{	
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);//程序退出时检测内存泄漏并显示到“输出”窗口
	
	//int arr[] = { 16,1,45,23,99,2,18,67,42,10 };	
	//int arr[] = { 516, 231, 445, 323, 299, 2, 18, 67, 42, 102, 164, 755, 687, 437 };
	//int arr[] = { 35,22,53,33,35,29,35,55,60,33,21,29,22,52,37,46,40,29 };

	int arr[] = { 1,3,5,1,0,5,4,1,2,3,4,3,2,5,4,4,4,4,2,0 };

	int length = sizeof(arr) / sizeof(arr[0]);   //数组中元素个数	
	//_nmsp1::CountSort(arr, length,99 + 1);//对数组元素进行计数排序，注意最大元素值+1，以给计数数组留出空间
	//_nmsp1::CountSort(arr, length, 755 + 1);//对数组元素进行计数排序，注意最大元素值+1，以给计数数组留出空间
	//_nmsp1::CountSort(arr, length, 100);//对数组元素进行计数排序，注意最大元素值+1，以给计数数组留出空间	
	_nmsp1::CountSort(arr, length, 6);	

	//_nmsp1::RadixSort(arr, length);//对数组元素进行基数排序
	//cout << "基数排序结果为：";
	cout << "计数排序结果为：";
	for (int i = 0; i < length; ++i)
	{
		cout << arr[i] << "  ";
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




