// MyProject.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

#include <iostream>
#include <cassert>

#pragma warning (disable: 4996) 
using namespace std;

//链式队列中每个节点的定义
template <typename T> //T代表数据元素的类型
struct QueueNode
{
	T        data;       //数据域，存放数据元素
	QueueNode<T>* next;  //指针域，指向下一个同类型（和本节点类型相同）节点
};

//链式队列的定义
template <typename T> //T代表数组中元素的类型
class LinkQueue
{
public:
	LinkQueue();                          //构造函数，参数可以有默认值
	~LinkQueue();                         //析构函数

public:
	bool EnQueue(const T& e);            //入队列（增加数据）		
	bool DeQueue(T& e);                  //出队列（删除数据）
	bool GetHead(T& e);                  //读取队头元素，但该元素并没有出队列而是依旧在队列中		

	void DispList();                     //输出链式队列中的所有元素
	int  ListLength();                   //获取链式队列的长度（实际拥有的元素数量）		

	bool IsEmpty();                      //判断链式队列是否为空		

private:
	QueueNode<T>* m_front;               //头指针（指向头结点），这一端允许出队（删除）
	QueueNode<T>* m_rear;                //专门引入尾指针以方便入队（插入）操作
	int m_length;                        //记录长度，方便获取长度
};

//通过构造函数对链式队列进行初始化
template <typename T>
LinkQueue<T>::LinkQueue()
{
	m_front = new QueueNode<T>; //先创建一个头结点
	m_front->next = nullptr;
	m_rear = m_front;
	m_length = 0;

	//若不带头节点的链式队列初始化代码应该如下，供参考
	/*m_front = nullptr;
	m_rear = nullptr;*/
}

//通过析构函数对链式队列进行资源释放
template <typename T>
LinkQueue<T>::~LinkQueue()
{
	QueueNode<T>* pnode = m_front->next;
	QueueNode<T>* ptmp;
	while (pnode != nullptr) //该循环负责释放数据节点
	{
		ptmp = pnode;
		pnode = pnode->next;
		delete ptmp;
	}
	delete m_front;             //释放头结点		
	m_front = m_rear = nullptr; //非必须
	m_length = 0;               //非必须
}

//入队列（增加数据），也就是从队尾增加数据
template <typename T>
bool LinkQueue<T>::EnQueue(const T& e)
{
	QueueNode<T>* node = new QueueNode<T>;
	node->data = e;
	node->next = nullptr;

	m_rear->next = node; //新节点插入到m_rear后面
	m_rear = node;       //更新队列尾指针

	m_length++;
	return true;
}

//出队列（删除数据），也就是删除队头数据
template <typename T>
bool LinkQueue<T>::DeQueue(T& e)
{
	if (IsEmpty() == true)
	{
		cout << "当前链式队列为空，不能进行出队操作!" << endl;
		return false;
	}

	QueueNode<T>* p_willdel = m_front->next;
	e = p_willdel->data;

	m_front->next = p_willdel->next;
	if (m_rear == p_willdel) //队列中只有一个元素节点（被删除后，整个队列就为空了）
		m_rear = m_front;  //设置队列为空(尾指针指向头指针)

	delete p_willdel;
	m_length--;
	return true;
}

//读取队头元素，但该元素并没有出队列而是依旧在队列中
template <typename T>
bool LinkQueue<T>::GetHead(T& e)
{
	if (IsEmpty() == true)
	{
		cout << "当前链式队列为空，不能读取队头元素!" << endl;
		return false;
	}

	e = m_front->next->data;
	return true;
}

//输出链式队列中的所有元素，时间复杂度为O(n)
template<class T>
void LinkQueue<T>::DispList()
{
	QueueNode<T>* p = m_front->next;
	while (p != nullptr)
	{
		cout << p->data << " ";  //每个数据之间以空格分隔
		p = p->next;
	}
	cout << endl; //换行
}

//获取链式队列的长度（实际拥有的元素数量），时间复杂度为O(1)
template<class T>
int LinkQueue<T>::ListLength()
{
	return m_length;
}

//判断链式队列是否为空，时间复杂度为O(1)
template<class T>
bool LinkQueue<T>::IsEmpty()
{
	if (m_front == m_rear) //当然，换一种判断方式也行：if(m_front->next == nullptr) return true;
	{
		return true;
	}
	return false;
}
//------------------------------------

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
public:
	//层序遍历二叉树，方便显示
	void levelOrder()
	{
		levelOrder(root);
	}
	//获取某个节点的高度（根高度为1，往下高度依次+1），用于显示节点时换行的目的
	int getNodeLevel(RBNode<T>* tNode)
	{
		int icurlvl = 0;
		while (tNode != nullptr)
		{
			tNode = tNode->parentNd;
			icurlvl++;
		} //end while
		return icurlvl;
	}
	void levelOrder(RBNode<T>* tNode)
	{
		if (tNode != nullptr) //若二叉树非空
		{
			RBNode<T>* tmpnode;
			LinkQueue<RBNode<T>*> lnobj;//注意，队列的元素类型是“节点指针”类型
			lnobj.EnQueue(tNode); //先把根节点指针入队
			int currdislvl = 1;   //当前显示第几层，根算第一层
			const char* pr = "[红]";
			const char* pb = "[黑]";
			char* ptmp;
			while (!lnobj.IsEmpty()) //循环判断队列是否为空
			{
				lnobj.DeQueue(tmpnode); //出队列
				int lvl = getNodeLevel(tmpnode);
				if (lvl != currdislvl) //用于换行
				{
					currdislvl = lvl;
					cout << endl;
				}
				if (tmpnode->isRed == false)
					ptmp = (char*)pb;
				else
					ptmp = (char*)pr;
				cout << tmpnode->data << ptmp << "       ";
				if (tmpnode->leftChild != nullptr)
					lnobj.EnQueue(tmpnode->leftChild);  //左孩子入队
				if (tmpnode->rightChild != nullptr)     //右孩子入队
					lnobj.EnQueue(tmpnode->rightChild);
			} //end while
		}
	}

	//插入元素
	void InsertElem(const T& e)  //不可以指定插入位置，程序内部会自动确定插入位置
	{
		InsertElem(root, e);
	}
	void InsertElem(RBNode<T>*& tNode, const T& e) //注意第一个参数类型
	{
		RBNode<T>* point = tNode;     //从指向根节点开始 
		RBNode<T>* parent = nullptr;  //保存父亲节点，根节点的父节点肯定先为nullptr			
		//通过一个while循环寻找要插入节点的位置，同时还要把插入路线上所经过的所有节点都保存到栈中，因为这些节点的平衡因子可能需要调整
		while (point != nullptr)
		{
			if (e == point->data)
				return;   //要插入的数据与当前树中某节点数据相同，则不允许插入，直接返回

			parent = point; //记录父节点，因为后续子节点要往下走，找合适自己的位置					
			if (e > point->data)
				point = point->rightChild;
			else
				point = point->leftChild;
		} //end while

		//走到这里，point 等于 nullptr，该生成新节点了
		point = new RBNode<T>;
		point->data = e;
		point->leftChild = nullptr;
		point->rightChild = nullptr;
		point->parentNd = nullptr;
		point->isRed = true;  //缺省插入的节点先给红色

		if (parent == nullptr)
		{
			//创建的是根节点
			point->isRed = false; //根节点是黑色
			tNode = point;
			return;
		}

		//走到这里表示创建的不是根节点，那么要把孩子链到父亲上
		if (e > parent->data)
			parent->rightChild = point; //在父节点的右树链入
		else
			parent->leftChild = point; //在父节点的左树链入

		point->parentNd = parent;  //记录父节点指针

		if (parent->isRed == false) //父节点为黑色，不需要做什么
			return;

		//能走到这里的，要插入的节点肯定是至少在第三层了。

		//父节点为红色才能走下来（当前节点也是红色的）
		BalanceTune(point, parent);
		root->isRed = false; //根节点颜色无条件给黑色
	}
	//平衡性调整	
	void BalanceTune(RBNode<T>* point, RBNode<T>* parent)//point：当前节点。parent：当前节点的父节点
	{
		RBNode<T>* parentBroNode = nullptr; //叔叔节点，有为空的可能性，此时表示没叔叔节点
		RBNode<T>* grandFatherNode = nullptr;      //爷爷节点，不会为空的，因为父亲节点为红色，红色不能做根，所以至少都是根节点做爷爷节点。

		while (true)
		{
			parentBroNode = (parent->parentNd != nullptr) ? (getBrotherNode(parent)) : nullptr;  //叔叔
			grandFatherNode = point->parentNd->parentNd;     //爷爷

			if (grandFatherNode == nullptr) //爷爷节点不可以为空
				break;

			//如果叔叔节点不为空并且叔叔节点为红色（这个代码很统一，只需要变色，集中写在这里）
			if (parentBroNode != nullptr && parentBroNode->isRed == true)
			{
				//这种情况只需要变色	
				//(1)父亲和叔叔变为黑色，爷爷变为红色
				parent->isRed = false;
				parentBroNode->isRed = false;
				grandFatherNode->isRed = true;

				//(2)如果爷爷节点是整个二叉树的根节点，跳出循环，根节点颜色在循环外进行设置为黑色的处理
				if (grandFatherNode == root)
					break;

				//(3)往上走继续调整
				point = grandFatherNode;    //当前节点指向爷爷，从爷爷节点（红）开始继续向上调整
				parent = point->parentNd;   //设置父亲节点

				//这情况不存在，否则前面的if (grandFatherNode == root)就会成立就会return出去，到不了这里						
								//记得#include <assert.h>
				if (parent == nullptr) { assert(false); }  //assert(false);意味着代码不可能执行到这条语句，若执行到了，会报告异常

				if (parent->isRed == false) //如果这个新父节点为黑色，认为调整完毕不继续向上调整
					break;

				continue;
			}

			//上面的代码下不来，下来的，不会满足上面的   if (parentBroNode != nullptr && parentBroNode->isRed == true)条件
			//因为如果叔叔节点不为空并且叔叔节点为红色的情况上面处理完了下不来了，所以走到下面的，都属于叔叔节点为空或者叔叔节点不为空但为黑色的情况

			//旋转变色之前确定一些信息，这是通用代码，所以放这里
			RBNode<T>* gff = grandFatherNode->parentNd;
			int sign = 0;  //标记：1表示grandFatherNode是其父亲的左孩子，2表示grandFatherNode是其父亲的右孩子
			if (gff != nullptr)
			{
				if (gff->leftChild == grandFatherNode)
					sign = 1;
				else
					sign = 2;
			}
			//-------------------------------------------------------------
			if (grandFatherNode->leftChild == parent)//第一种情形，父亲是爷爷节点的左孩子
			{
				//开始旋转和变色以调整平衡
				if (parent->leftChild == point)//新节点是父亲节点的左孩子
				{
					//右旋转
					RotateRight(grandFatherNode);
				}
				else //新节点是父亲节点的右孩子
				{
					//先左后右旋
					RotateLeftRight(grandFatherNode);
				} //end if (parent->leftChild == point)

				//旋转变色之后一些公用代码						
				//变色
				grandFatherNode->isRed = false;  //新根设置为黑色
				grandFatherNode->rightChild->isRed = true; //新右叶子设置为红色	
			}
			else   // 第二种情形，父亲是爷爷节点的右孩子
			{
				//开始旋转和变色以调整平衡
				if (parent->rightChild == point)//新节点是父亲节点的右孩子
				{
					//左旋转
					RotateLeft(grandFatherNode);
				}
				else //新节点是父亲节点的左孩子
				{
					//先右后左旋
					RotateRightLeft(grandFatherNode);
				} //end if (parent->rightChild == point)

				//旋转变色之后一些公用代码						
				//变色
				grandFatherNode->isRed = false;  //新根设置为黑色
				grandFatherNode->leftChild->isRed = true; //新左叶子设置为红色
			}
			//---------------------------------------------------------------
			// 一些通用代码放这里
			//根已经改变了，所以要设置一些节点指向信息
			if (gff == nullptr)
			{
				//根要变了吧
				root = grandFatherNode;
			}
			else if (sign == 1)
			{
				gff->leftChild = grandFatherNode;
			}
			else if (sign == 2)
			{
				gff->rightChild = grandFatherNode;
			}
			break; //应该不用往上走继续调整，所以break;			
		} //end while
		return;
	}
	//获取兄弟节点指针
	RBNode<T>* getBrotherNode(RBNode<T>* p)
	{
		//由调用者确定p->parent一定不为nullptr。
		if (p->parentNd->leftChild == p) //父亲的左儿子是不是自己
			return 	p->parentNd->rightChild; //返回父亲的右儿子

		return p->parentNd->leftChild; //返回父亲的左儿子
	}
	//左旋转（代码与AVL左旋其实是一样的，但要注意parent指针的设置）
	void RotateLeft(RBNode<T>*& pointer)//注意参数类型
	{
		//     4      -----左旋----             6
		//  /      \                         /     \
		// 3        6                     4         7
		//         /   \                 /  \
		//        5     7              3     5   
		RBNode<T>* ptmproot = pointer;   //让ptmproot指向4

		pointer = ptmproot->rightChild;  //让pointer指向6
		pointer->parentNd = ptmproot->parentNd; //设置6的父亲指向4的父亲

		ptmproot->rightChild = pointer->leftChild; //让4的右孩子不再指向6，而是指向5
		if (pointer->leftChild)
			pointer->leftChild->parentNd = ptmproot;   //设置5的父亲是4

		pointer->leftChild = ptmproot; //让6的左孩子指向4
		ptmproot->parentNd = pointer;  //设置4的父亲是6
	}

	//右旋转
	void RotateRight(RBNode<T>*& pointer) //注意参数类型
	{
		//         6      -----右旋----             4
		//     /      \                          /     \
		//    4        7                        3       6
		//  /   \                                      /  \
		// 3     5                                    5    7   
		RBNode<T>* ptmproot = pointer;   //让ptmproot指向6

		pointer = ptmproot->leftChild;    //让pointer指向4
		pointer->parentNd = ptmproot->parentNd; //设置4的父亲指向6的父亲

		ptmproot->leftChild = pointer->rightChild;  //让6的左孩子不再指向4，而是指向5
		if (pointer->rightChild)
			pointer->rightChild->parentNd = ptmproot;   //设置5的父亲是6

		pointer->rightChild = ptmproot;             //让4的右孩子指向6
		ptmproot->parentNd = pointer;               //设置6的父亲是4
	}

	//先左后右旋转
	void RotateLeftRight(RBNode<T>*& pointer)
	{
		//如下图不全，只是示意，不要按照这个图写代码否则会造成代码出现bug
		//      6      -----先左旋----    6       -----再右旋----    5
		//   /      \                   /   \                      /    \
		//  4        7                 5     7                    4      6 
		//    \                       /                                    \
		//     5                     4                                      7

		RBNode<T>* ptmproot = pointer;   //让ptmproot指向6
		RBNode<T>* ptmproot_lf = ptmproot->leftChild;  //让ptmproot_lf指向4
		pointer = ptmproot_lf->rightChild;  //让pointer指向5，5是旋转后最终的根
		RBNode<T>* orgparent = ptmproot->parentNd; //把老父亲保存起来，后面要用			
		//先左旋转
		ptmproot->leftChild = pointer; //6的左孩子指向5,其实后面会覆盖，所以这个赋值其实没用，但为了醒目清晰，就留着了
		pointer->parentNd = ptmproot;  //5的父亲指向6，上行没啥用，所以这行其实也没啥用		

		ptmproot_lf->rightChild = pointer->leftChild; //4的右孩子指向5的左孩子
		if (pointer->leftChild)
			pointer->leftChild->parentNd = ptmproot_lf;   //5的左孩子的父亲指向4

		pointer->leftChild = ptmproot_lf; //5的左孩子指向4
		ptmproot_lf->parentNd = pointer;    //4的父亲指向5

		//再右旋转			
		ptmproot->leftChild = pointer->rightChild;//6的左孩子指向5的右孩子
		if (pointer->rightChild)
			pointer->rightChild->parentNd = ptmproot;  //5的右孩子的父亲指向6

		pointer->rightChild = ptmproot;  //5的右孩子指向6
		ptmproot->parentNd = pointer;    //6的父亲指向5

		//再处理根节点的父亲指向
		pointer->parentNd = orgparent;
	}

	//先右后左旋转
	void RotateRightLeft(RBNode<T>*& pointer)
	{
		//      6     -----先右旋----     6       -----再左旋----     7
		//   /     \                   /     \                      /    \
		//  4        8                4       7                    6      8 
		//          /                           \                 /                   
		//         7                             8               4                    
		RBNode<T>* ptmproot = pointer;   //让ptmproot指向6
		RBNode<T>* ptmproot_rg = ptmproot->rightChild;  //让ptmproot_rg指向8
		pointer = ptmproot_rg->leftChild;  //让pointer指向7，7是旋转后最终的根
		RBNode<T>* orgparent = ptmproot->parentNd; //把老父亲保存起来，后面要用

		//先右旋转
		ptmproot->rightChild = pointer; //6的右孩子指向7,其实后面会覆盖，所以这个赋值其实没用，但为了醒目清晰，就留着了
		pointer->parentNd = ptmproot;  //7的父亲指向6，上行没啥用，所以这行其实也没啥用
		ptmproot_rg->leftChild = pointer->rightChild; //8的左孩子指向7的右孩子
		if (pointer->rightChild)
			pointer->rightChild->parentNd = ptmproot_rg;   //7的右孩子的父亲指向7
		pointer->rightChild = ptmproot_rg; //7的右孩子指向8
		ptmproot_rg->parentNd = pointer;    //8的父亲指向7

		//再左旋转
		ptmproot->rightChild = pointer->leftChild;//6的右孩子指向7的左孩子
		if (pointer->leftChild)
			pointer->leftChild->parentNd = ptmproot;  //7的左孩子的父亲指向6

		pointer->leftChild = ptmproot;  //7的左孩子指向6
		ptmproot->parentNd = pointer;    //6的父亲指向7

		//再处理根节点的父亲指向
		pointer->parentNd = orgparent;
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

	RBTree<int> myrbtr;
	int array[] = { 60,25,90,23,49,86,100,34,58,59,80 };
	int acount = sizeof(array) / sizeof(int);
	for (int i = 0; i < acount; ++i)
	{
		myrbtr.InsertElem(array[i]);
	}
	myrbtr.levelOrder();


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



