// MyProject.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

#include <iostream>

#pragma warning (disable: 4996) 
using namespace std;

//红黑树中每个节点的定义
template <typename T>   //T代表数据元素的类型
struct RBNode
{
	T        data;
	RBNode* leftChild,   //左子节点指针
		* rightChild,  //右子节点指针
		* parentNd;      //父节点指针，引入方便操作

	bool     isRed;      //是否是红色节点，true：是，false：不是（而是黑色节点）
};

//红黑树的定义
template <typename T>
class RBTree
{
public:
	RBTree()  //构造函数
	{
		root = nullptr;
	}
	~RBTree() //析构函数
	{
		ReleaseNode(root);
	}
private:
	void ReleaseNode(RBNode<T>* pnode)
	{
		if (pnode != nullptr)
		{
			ReleaseNode(pnode->leftChild);
			ReleaseNode(pnode->rightChild);
		}
		delete pnode;
	}

private:
	RBNode<T>* root; //树根指针
};

//--------------------------------
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



