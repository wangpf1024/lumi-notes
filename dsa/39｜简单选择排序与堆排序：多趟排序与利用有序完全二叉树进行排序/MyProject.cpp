// MyProject.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

using namespace std;
#pragma warning(disable : 4996) 
 
// 
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
//-----------------
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
			if (myarray[j] < myarray[j + 1]) //如果左子节点值<右子节点值
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

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);//程序退出时检测内存泄漏并显示到“输出”窗口

	//int arr[] = { 16,1,45,23,99,2,18,67,42,10 };
	//int length = sizeof(arr) / sizeof(arr[0]);   //数组中元素个数
	//SmpSelSort(arr, length);//对数组元素进行简单选择排序
	//cout << "简单选择排序结果为：";
	//for (int i = 0; i < length; ++i)
	//{
	//	cout << arr[i] << " ";
	//}
	//cout << endl; //换行

	int arr[] = { 16,1,45,23,99,2,18,67,42,10 };
	int length = sizeof(arr) / sizeof(arr[0]);   //数组中元素个数
	HeapSelSort(arr, length);
	cout << "堆排序结果为：";
	for (int i = 0; i < length; ++i)
	{
		cout << arr[i] << " ";
	}
	cout << endl; //换行

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

