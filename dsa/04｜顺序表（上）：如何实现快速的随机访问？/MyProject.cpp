// MyProject.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

#include <iostream>

#pragma warning (disable: 4996)  
using namespace std;

#define InitSize 10   //动态数组的初始尺寸
#define IncSize  5    //当动态数组存满数据后每次扩容所能多保存的数据元素数量

template <typename T> //T代表数组中元素的类型
class SeqList
{
public:
	SeqList(int length = InitSize); //构造函数，参数可以有默认值
	~SeqList();                   //析构函数

public:
	bool ListInsert(int i, const T& e);  //在第i个位置插入指定元素e
	bool ListDelete(int i);              //删除第i个位置的元素
	bool GetElem(int i, T& e);           //获得第i个位置的元素值
	int  LocateElem(const T& e);         //按元素值查找其在顺序表中第一次出现的位置

	void DispList();                     //输出顺序表中的所有元素
	int  ListLength();                   //获取顺序表的长度
	void ReverseList();                  //翻转顺序表		

private:
	void IncreaseSize();         //当顺序表存满数据后可以调用此函数为顺序表扩容

private:
	T* m_data;                   //存放顺序表中的元素
	int m_length;                 //顺序表当前长度（当前有几个元素）
	int m_maxsize;                //动态数组最大容量
};

//通过构造函数对顺序表进行初始化
template <typename T>
SeqList<T>::SeqList(int length)
{
	m_data = new T[length]; //为一维数组动态分配内存
	m_length = 0;           //顺序表当前实际长度为0，表示还未向其中存入任何数据元素
	m_maxsize = length;     //顺序表最多可以存储m_maxsize个数据元素
}

//通过析构函数对顺序表进行资源释放
template <typename T>
SeqList<T>::~SeqList()
{
	delete[] m_data;
	m_length = 0; //非必须
}

//在第i个位置（位置编号从1开始）插入指定元素e，时间复杂度：O(n)，时间开销主要源于元素的移动
template <typename T>
bool SeqList<T>::ListInsert(int i, const T& e)
{
	//如果顺序表已经存满了数据，则不允许再插入新数据了
	if (m_length >= m_maxsize)
	{
		cout << "顺序表已满，不能再进行插入操作了!" << endl;
		return false;
	}

	//判断插入位置i是否合法，i的合法值应该是1到m_length+1之间
	if (i < 1 || i >(m_length + 1))
	{
		cout << "元素" << e << "插入的位置" << i << "不合法，合法的位置是1到" << m_length + 1 << "之间!" << endl;
		return false;
	}

	//从最后一个元素开始向前遍历到要插入新元素的第i个位置，分别将这些位置中原有的元素向后移动一个位置
	for (int j = m_length; j >= i; --j)
	{
		m_data[j] = m_data[j - 1];
	}
	m_data[i - 1] = e;   //在指定位置i处插入元素e，因为数组下标从0开始，所以这里用i-1表示插入位置所对应的数组下标
	cout << "成功在位置为" << i << "处插入元素" << m_data[i - 1] << "!" << endl;
	m_length++;        //实际表长+1
	return true;
}

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);//程序退出时检测内存泄漏并显示到“输出”窗口

	SeqList<int> seqobj(10);

	seqobj.ListInsert(1, 15);
	seqobj.ListInsert(2, 10);
	seqobj.ListInsert(30, 8);

	return 0;
}


	// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
	// 调试程序: F5 或调试 >“开始调试”菜单

	// 入门使用技巧:
	//   1. 使用解决方案资源管理器窗口添加/管理文件
	//   2. 使用团队资源管理器窗口连接到源代码管理
	//   3. 使用输出窗口查看生成输出和其他消息
	//   4. 使用错误列表窗口查看错误nm,
	//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
	//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件



