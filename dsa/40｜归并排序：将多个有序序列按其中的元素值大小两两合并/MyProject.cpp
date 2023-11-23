
#include <iostream>

using namespace std;
#pragma warning(disable : 4996) 

//------------------------------
namespace _nmsp1
{
	//2路归并，该函数是归并排序的核心代码，用于把两个有序子序列归并成一个
	//left:指向第一个序列开头元素，mid：指向第一个序列末尾元素，right指向第二个序列末尾元素
	//通过left,mid,right指定要归并的两个子序列的范围（这两个子序列相邻）
	template<typename T>
	void Merge(T myarray[], T* pResult, int left, int mid, int right, int length)  //length纯用于显示信息目的
	{
		cout << "Merge()：left,mid,right=" << left << "," << mid << "," << right;
		cout << "|元素值begin：";
		for (int i = 0; i < length; ++i) cout << myarray[i] << " ";


		//把myarray指定的left到right范围内的数据先复制到pResult（临时空间）中
		for (int i = left; i <= right; ++i)
		{
			pResult[i] = myarray[i];
		} //end i

		int curpos1 = left;   //第一个序列的开始元素
		int curpos2 = mid + 1; //第二个序列的开始元素
		int curpos3 = left;  //最终合并好的序列的开始元素位置
		while (curpos1 <= mid && curpos2 <= right)
		{
			if (pResult[curpos1] <= pResult[curpos2]) //将较小值赋值给myarray，这里的比较符可以保证该算法稳定性
			{
				myarray[curpos3] = pResult[curpos1];
				curpos1++;
			}
			else
			{
				myarray[curpos3] = pResult[curpos2];
				curpos2++;
			}
			curpos3++;
		} //end while
		//如果两个子序列元素数目不同，则这里要单独处理
		while (curpos1 <= mid) //子序列1比子序列2长
		{
			//把子序列1中剩余的内容放入到myarray中去
			myarray[curpos3] = pResult[curpos1];
			curpos1++;
			curpos3++;
		}
		while (curpos2 <= right) //子序列2比子序列1长
		{
			myarray[curpos3] = pResult[curpos2];
			curpos2++;
			curpos3++;
		}

		cout << "|元素值end：";
		for (int i = 0; i < length; ++i) cout << myarray[i] << " ";
		cout << endl;
		return;
	}

	//归并排序重载函数
	template<typename T>
	void MergingSort(T myarray[], T* pResult, int left, int right, int length) //length用于显示信息目的
	{
		if (left >= right)
			return; //递归出口

		int mid = (left + right) / 2; //中间分开
		MergingSort(myarray, pResult, left, mid, length); //对左半部分归并排序
		MergingSort(myarray, pResult, mid + 1, right, length); //对右半部分归并排序

		//上面因为左半部分归并排序完成，右半部分归并排序完成，所以下面是合并左半部分和右半部分了
		//开始真正的归并操作
		Merge(myarray, pResult, left, mid, right, length);
	}

	//归并排序入口（从小到大）
	template<typename T>
	void MergingSort(T myarray[], int length)
	{
		T* pResult = new T[length]; //新数组，用于保存结果
		MergingSort(myarray, pResult, 0, length - 1, length); //调用重载函数
		delete[] pResult;
		return;
	}

	//----------------------------------
	//非递归程序编写方法
	template<typename T>
	void MergingSort_noRecu(T myarray[], int length)
	{
		if (length <= 1) //不超过1个元素的数组，没必要排序
			return;

		T* pResult = new T[length]; //新数组，用于保存结果		

		//标示两个子序列位置用
		int left, mid, right;

		int jianGe = 1;//间隔，开始时元素是紧挨着的两个比较，两个元素之间下标间隔是1
		int subseqTotal = length; //当前子序列数量，开始时一个元素算一个子序列
		int gbts = 0; //归并趟数
		while (subseqTotal > 1) //只要没有最终合并成1个序列，就一直循环
		{
			gbts++;
			cout << "第" << gbts << "趟归并：----------------" << endl;

			for (int i = 0; i < length; i += (jianGe * 2))
			{
				left = i;
				mid = left + jianGe - 1;
				if (mid >= length)
					break; //不行这不合法了

				right = mid + (mid - left + 1);
				if (right >= length) right = (length - 1); //保证right合法

				//必须保证left,mid,right都是合理值
				if (left <= mid && right > left && right > mid)
				{
					//肯定是两个序列，能合并 
					Merge(myarray, pResult, left, mid, right, length);
					subseqTotal--; //两个序列合并成一个，这里自然减少了1					
				}
				else
				{
					//不能合并，这次for循环退出
					break;
				}
			} //end for i
			jianGe *= 2;
		} //end while

		delete[] pResult;
		return;
	}

}


int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);//程序退出时检测内存泄漏并显示到“输出”窗口

	//int arr[] = { 16,1,45,23,99,2,18 };
	int arr[] = { 16,1,45,23,99,2,18,67,42,10 };
	int length = sizeof(arr) / sizeof(arr[0]);   //数组中元素个数

	//_nmsp1::MergingSort(arr, length);//对数组元素进行归并排序
	_nmsp1::MergingSort_noRecu(arr, length);//对数组元素进行归并排序(非递归)
	//cout << "树形选择排序结果为：";
	cout << "归并排序结果为：";
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




