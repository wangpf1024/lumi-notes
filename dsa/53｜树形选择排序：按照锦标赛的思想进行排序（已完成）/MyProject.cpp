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
		for (int i = 0; i < length - 1; ++i) //一共要进行length-1趟
		{
			int minidx = i;  //保存最小元素位置

			//在myarray[i]到myarray[length-1]中选择最小值
			for (int j = i + 1; j < length; ++j)
			{
				if (myarray[j] < myarray[minidx])
					minidx = j; //保存最小元素值位置
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
		//ceil是系统函数：ceil(x)函数返回的是大于或等于x的最小整数
		int treelvl = (int)ceil(log(length) / log(2)) + 1; //5:完全二叉树高度(含有n个叶子节点的完全二叉树的高度是⌈logn⌉ +1)

		//treelvl高的完全二叉树最多有nodecount个节点，如果有nodecount个节点，此时的完全二叉树其实是满二叉树
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
					if (myarray[pidx[i]] <= myarray[pidx[i + 1]])
					{
						pidx[(i + 1) / 2 - 1] = pidx[i];
					}
					else
					{
						pidx[(i + 1) / 2 - 1] = pidx[i + 1];
					}
				}
				else if (pidx[i] != INT_MAX_MY) //pidx[i]是正常值，因为有上个if在，说明pidx[i + 1]不是正常值
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
			ptmparray[i] = myarray[pidx[0]]; //将当前最小值赋给ptmparray[i]临时保存

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
				leafidx = (leafidx + 1) / 2 - 1;//序号为leafidx的结点的双亲结点序号 
				if (pidx[2 * leafidx + 1] != INT_MAX_MY && pidx[2 * leafidx + 2] != INT_MAX_MY)  //如果pidx[i]和pidx[i+1]都是正常值， 那自然是可以比较
				{
					if (myarray[pidx[2 * leafidx + 1]] <= myarray[pidx[2 * leafidx + 2]])
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
		
	//调整以curpos为根的子树为大顶堆（这里要一直向下调整到尾部叶子才能结束）
	template<typename T>
	void AdjustDown(T myarray[], int length, int curpos) //length：堆中元素个数，curpos：当前开始调整的位置
	{
		int i = curpos;  //4，当前节点下标
		int j = 2 * i + 1; //9，左子节点下标

		while (j < length)//i有左子节点
		{			
			//找左右子节点中值比较大的节点，让j指向该节点		
			if (j < length - 1) //i也有右子树
			{				
				if (myarray[j] < myarray[j + 1]) //如果左子节点值 < 右子节点值
					j = (j + 1); //让j始终指向值最大的子节点。
			}			
			if (myarray[i] >= myarray[j]) //父节点保持值最大
				break; //不需要调整直接break;

			//走到这里表示孩子节点比父亲节点大，那么就要进行父子位置交换了
			T temp = myarray[i];
			myarray[i] = myarray[j];
			myarray[j] = temp;

			//交换数据之后，可能就会影响到下一层了。所以还要继续向下调整
			i = j;
			j = 2 * i + 1;
		}//end while
		return;
	}

	//把最大值从大顶堆中删除掉，把尾元素移动到首部并且继续调整为大顶堆
	template<typename T>
	void RemoveMaxValue(T myarray[], int length) //length:堆中元素个数
	{		
		if (length == 0) //就一个元素了，就不需要调整了本身就认为是堆了
			return;
		T temp = myarray[0]; //该位置元素值最大
		myarray[0] = myarray[length]; //将最后一个元素放到最上面位置，等价于最上面位置元素被删除了	
		myarray[length] = temp;
		AdjustDown(myarray, length, 0);
		return;
	}

	//堆排序（从小到大）
	template<typename T>
	void HeapSelSort(T myarray[], int length)
	{
		//(1)由一个无序序列建成一个大顶堆		
		//现在要把myarray数组调整为大顶堆（检查树中所有非叶子节点，看看这些非叶子节点是否满足大顶堆的要求，如果不满足要求，则调整成满足大顶堆的要求）
		int curpos = length / 2 - 1;//非叶子节点的位置
		while (curpos >= 0) //逐渐向树的根部调整
		{
			//要从当前节点向下调整的
			AdjustDown(myarray, length, curpos);
			curpos--;
		} //end while

		//(2)通过大顶堆来进行排序
		for (int i = 0; i < length; ++i)
		{						
			RemoveMaxValue(myarray, length - i - 1); //把最大值从大顶堆中删除掉，并重新整理使之再次变成大顶堆
		} //end for i		
		return;			
	}
}


int main()
{	
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);//程序退出时检测内存泄漏并显示到“输出”窗口

	//int arr[] = { 16,1,45,23,99,2,18,67,42,10 };
	//int length = sizeof(arr) / sizeof(arr[0]);   //数组中元素个数
	//_nmsp1::SmpSelSort(arr, length);//对数组元素进行简单选择排序
	//cout << "简单选择排序结果为：";
	//for (int i = 0; i < length; ++i)
	//{
	//	cout << arr[i] << "  ";
	//}
	//cout << endl; //换行

	int arr[] = { 16,1,45,23,99,2,18,67,42,10 };
	int length = sizeof(arr) / sizeof(arr[0]);   //数组中元素个数
	//_nmsp1::TreeSelSort(arr, length);//对数组元素进行树形选择排序
	_nmsp1::HeapSelSort(arr, length);
	//cout << "树形选择排序结果为：";
	cout << "堆排序结果为：";
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




