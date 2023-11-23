// MyProject.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

#include <iostream>
#include <cassert>

#pragma warning (disable: 4996) 
using namespace std;

//表示边的节点结构
struct EdgeNode_adjmt
{
	int iidx; //边的第一个顶点下标
	EdgeNode_adjmt* ilink;//指向下一个依附于iidx所代表的顶点的边

	int jidx; //边的第二个顶点下标	
	EdgeNode_adjmt* jlink; //指向下一个依附于jidx所代表的顶点的边
	//int weight; //权值，可以根据需要决定是否需要此字段
};

//表示顶点的节点结构
template<typename T>
struct VertexNode_adjmt
{
	T data;    //顶点中的数据
	EdgeNode_adjmt* firstedge; //与该顶点相连的第一条边
};

//---------------
//表示边的结构
struct Edge_esa
{
	int beginidx; //边的起始顶点下标
	int endidx; //边的终止顶点下标
	//int weight; //权值，可以根据需要决定是否需要此字段
};

int main()
{	
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);//程序退出时检测内存泄漏并显示到“输出”窗口

	




	return 0;
}


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧:  ∞
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误nm,
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件



