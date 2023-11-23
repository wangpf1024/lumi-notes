// MyProject.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
//#include <assert.h>
//#include<malloc.h> /* malloc()等 */
//#include<stdio.h> /* EOF(=^Z或F6),NULL */
//#include<process.h> /* exit() */

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
	//简单选择排序（从小到大）
	template<typename T>
	void SmpSelSort(T myarray[], int length)
	{
		int ic = 0;
		for (int i = 0; i < length - 1; ++i) //一共要进行length-1趟
		{
			int minidx = i;  //保存最小元素位置

			//在myarray[i]到myarray[length-1]中选择最小值
			for (int j = i + 1; j < length; ++j)
			{
				if (myarray[j] < myarray[minidx])
				{
					ic++;
					minidx = j; //保存最小元素值位置
				}
			} //endj

			if (minidx != i)
			{
				T temp = myarray[i];
				myarray[i] = myarray[minidx];
				myarray[minidx] = temp;
			}
		} //end for i
		return;
	}
	


	#define INT_MAX_MY 2147483647//整型能够保存的最大数值，作为标记使用

	//树形选择排序（从小到大）
	template<typename T>
	void TreeSelSort(T myarray[], int length)
	{
		//ceil是系统函数：ceil(x)函数返回的是大于或等于x的最小整数。
		int treelvl = (int)ceil(log(length) / log(2)) + 1; //5:完全二叉树高度(含有n个叶子节点的完全二叉树的高度是⌈log⁡n⌉ +1)

		//treelvl高的完全二叉树最多有nodecount个节点，如果有nodecount个节点，此时的完全二叉树其实是满二叉树。
		int nodecount = (int)pow(2, treelvl) - 1; //31：满二叉树是指一棵高度为h，且含有2h-1个节点的二叉树

		//treelvl-1 高的完全二叉树最多有nodecount2个节点
		int nodecount2 = (int)pow(2, treelvl - 1) - 1; //15

		int* pidx = new int[nodecount];//保存节点的下标用的内存		

		//叶子节点保存元素的下标值（就等于保存了元素的值）
		for (int i = 0; i < length; ++i) 
		{
			pidx[nodecount2 + i] = i; //pidx[15] = 0; pidx[16] = 1....;pidx[24] = 9
		} //end for

		//给多余的叶子节点赋予一个最大值作为标记
		for (int i = nodecount2 + length; i < nodecount; ++i) //i=25~30
		{
			pidx[i] = INT_MAX_MY;  //pidx[25] = MAX;pidx[26] = MAX; ......pidx[30] = MAX
		}
		
		int tmpnode2 = nodecount2;  //15
		int tmpnode = nodecount;    //31	

		//现在要开始给非叶子节点赋值了,非叶子节点下标是[0]~[14]
		//第一趟排序要给非叶子节点赋值，还要两两进行节点比较，所以要单独处理
		while (tmpnode2 != 0)
		{			
			//第一次for执行i值分别为：15、17、19、21、23、25、27、29
			//第二次for执行i值分别为：7,9,11,13
			//第三次for执行i值分别为：3,5
			//第四次for执行i值分别为：1
			for (int i = tmpnode2; i < tmpnode; i += 2) 
			{
				//第一次for这个pidx的下标【(i + 1) / 2 - 1】分别是7,8,9,10,11,12,13,14
				//第二次for这个pidx的下标【(i + 1) / 2 - 1】分别是3,4,5,6
				//第三次for这个pidx的下标【(i + 1) / 2 - 1】分别是1,2
				//第四次for这个pidx的下标【(i + 1) / 2 - 1】分别是0
				//把两个孩子中小的孩子值给爹
				if (pidx[i] != INT_MAX_MY && pidx[i + 1] != INT_MAX_MY)  //如果pidx[i]和pidx[i+1]都是正常值， 那自然是可以比较
				{
					if (myarray[pidx[i]].data <= myarray[pidx[i + 1]].data)
					{
						pidx[(i + 1) / 2 - 1] = pidx[i];
					}
					else
					{
						pidx[(i + 1) / 2 - 1] = pidx[i + 1];
					}
				}
				else if( pidx[i] != INT_MAX_MY) //pidx[i]是正常值，因为有上个if在，说明pidx[i + 1]不是正常值
				{
					pidx[(i + 1) / 2 - 1] = pidx[i];
				} 
				else //走到这里，说明pidx[i + 1]是正常值或者是INT_MAX_MY值
				{
					pidx[(i + 1) / 2 - 1] = pidx[i + 1];
				}
			} //end for			
			tmpnode = tmpnode2;  //15,7,3,1			
			tmpnode2 = (tmpnode2 - 1) / 2; //7,3,1,0
		} //end while
				
		T* ptmparray = new T[length]; //临时保存排好序的数据

		for (int i = 0; i < length; i++)
		{			
			ptmparray[i] =  myarray[pidx[0]];   //将当前最小值赋给ptmparray[i]临时保存

			int leafidx = 0;

			//沿树根找最小值结点在叶子中的序号
			//leafidx = 0,1,3,7,16分别追溯到叶子中的编号
			for (int j = 1; j < treelvl; j++) 
			{
				if (pidx[2 * leafidx + 1] == pidx[leafidx])
				{
					leafidx = 2 * leafidx + 1;
				}
				else
				{
					leafidx = 2 * leafidx + 2;
				}
			} //end for j

			//此时的leafidx就是完全二叉树叶子节点中的那个最小值的下标
			pidx[leafidx] = INT_MAX_MY; //leafidx = 16。
			while (leafidx)
			{
				//leafidx = 7,3,1,0
				leafidx = (leafidx + 1) / 2 - 1; //序号为leafidx的结点的双亲结点序号 				
				
				if (pidx[2 * leafidx + 1] != INT_MAX_MY && pidx[2 * leafidx + 2] != INT_MAX_MY)  //如果pidx[i]和pidx[i+1]都是正常值， 那自然是可以比较
				{
					if (myarray[ pidx[2 * leafidx + 1]].data <= myarray[pidx[2 * leafidx + 2]].data)
					{
						pidx[leafidx] = pidx[2 * leafidx + 1];
					}
					else
					{
						pidx[leafidx] = pidx[2 * leafidx + 2];
					}
				}
				else if (pidx[2 * leafidx + 1] != INT_MAX_MY)
				{
					pidx[leafidx] = pidx[2 * leafidx + 1];
				}
				else
				{
					pidx[leafidx] = pidx[2 * leafidx + 2];
				}
			}//end while
		} //end for i

		//把数据从ptmparray拷贝回myarray
		for (int i = 0; i < length; i++)
		{
			myarray[i] = ptmparray[i];
		} //end for i

		//释放内存
		delete[] ptmparray;
		delete[] pidx;	
		return;
	}

}
struct tempstruc
{
	int data;    //值
	int idx;      //序号
};
int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);//程序退出时检测内存泄漏并显示到“输出”窗口

	
	//int arr[] = { 16,1,45,23,99,2,18,67,42,10 };
	//tempstruc arr[] = { {1, 0}, { 3,1 }, {3,2},{3,3},{5,4},{5,5},{5,6},{4,7},{4,8},{4,9},{6,10},{6,11} };
	//tempstruc arr[] = { {2, 0}, { 2,1 }, {1,2}};
	tempstruc arr[] = { {1, 0}, { 2,1 }, {1,2}, {1,3} ,{ 2,4 } };


	
	int length = sizeof(arr) / sizeof(arr[0]);   //数组中元素个数
	_nmsp1::TreeSelSort(arr, length);//对数组元素进行简单选择排序

	cout << "树形选择排序结果为：";
	for (int i = 0; i < length; ++i)
	{
		cout << arr[i].data << "【" << arr[i].idx << "】   ";
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




