// MyProject.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include "MersenneTwister.h" //从外面找的一个文件，用来产生更随机的数字

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
	//每个节点的定义
	template <typename T>
	struct Node
	{
		T  data;		
		Node* right;  //向右指向的指针
		Node* down;   //向下指向的指针		
	};

	#define MAX_LEVEL 100 
	MTRand g_ac; //全局量以产生更加随机的随机数，#include "MersenneTwister.h" 

	//跳表定义
	template <typename T>
	class SkipList
	{
	public:
		SkipList()  //构造函数		
		{
			pathList.reserve(MAX_LEVEL); //保留空间提升性能
			m_head = new Node<T>; //先创建一个头结点
			m_head->right = nullptr;
			m_head->down = nullptr;
			m_head->data = -1; //可以随便给个值，方便调试时观察和区分			
		}
		~SkipList() //析构函数
		{
			while (m_head != nullptr)
			{
				Node<T>* pnode = m_head->right;
				Node<T>* ptmp;
				while (pnode != nullptr) //该循环负责释放数据节点
				{
					ptmp = pnode;
					pnode = pnode->right;
					delete ptmp;
				}

				Node<T>* tmphead = m_head;
				m_head = m_head->down;								
				delete tmphead;
				tmphead = nullptr;
			}
			return;
		}

		//插入数据
		template <typename T>
		void Insert(const T& e)
		{
			pathList.clear(); //记得每次清理，以免使用上次遗留的旧数据
			Node<T>* p = m_head;

			//先往右找再往下找(跨过多级索引，找到原始链表)，目的就是找到原始链表中的插入位置
			while (p)
			{
				//向右找合适的节点位置
				while (p->right != nullptr && p->right->data < e)
				{
					p = p->right; //先往右走
				}

				pathList.push_back(p);  //尾部插入元素，最终原始链表在最尾部
				p = p->down; //再往下走
			} //end while

			bool insertupsign = true; //向上层插入节点的概率，刚开始是原始链表，肯定是要将数据插入其中的
			Node<T>* downNode = nullptr; //创建一个新节点时，新节点的down指针指向的就是这个downNode所代表的节点

			//下面这个while能够从最底下往上回溯处理			
			while (insertupsign == true && pathList.size() > 0)
			{
				//insert指向插入位置的节点
				Node<T>* insert = pathList.back(); //返回最底部的数据——第一次返回的是原始链表
				pathList.pop_back(); //干掉最底部的数据

				//创建新节点
				Node<T>* newnode = new Node<T>();
				newnode->data = e;
				newnode->right = insert->right; //原来insert节点指向的元素现在让tmpnode指向
				newnode->down = downNode; 

				insert->right = newnode; //insert指向这个新节点
				downNode = newnode;
				
				//每一层节点被提取到上一层的概率是1/2（50%)				
				unsigned int tmpvalue = g_ac.randInt(); //产生随机数
				//printf("%u,",tmpvalue);
				if (tmpvalue % 2 == 0)
				{
					insertupsign = true;					
				}
				else
				{
					insertupsign = false;
				}
			} //end while			

			//标志要新加一级索引（新加一级索引意味着肯定要在这个索引中加入一个节点）
			if (pathList.size() <= 0 && g_ac.randInt() % 2 == 0)  //后一个条件表示是否增加新一级索引
			{
				//表示新节点被增加到了最上层索引 并且 增加新一级索引
				Node<T>* newnode = new Node<T>();
				newnode->right = nullptr;
				newnode->down = downNode;
				newnode->data = e;

				//既然是新加一级索引，那么索引也要个头节点，所以如下是增加头节点
				Node<T>* tmpheadnode = new Node<T>();
				tmpheadnode->right = newnode;
				tmpheadnode->down = m_head;
				tmpheadnode->data = -1;
				m_head = tmpheadnode; //m_head指向最上级索引的头节点
			}			
			return;
		}

		//删除数据，返回true表示找到并删除了数据，返回false表示没找到数据
		template <typename T>
		bool Delete(const T& e)
		{
			Node<T>* p = m_head;
			bool iffind = false; //是否找到
			while (p)
			{
				//向右找合适的节点位置
				while (p->right != nullptr && p->right->data < e)
				{
					p = p->right; //先往右走
				}

				//右边没节点了，或者右边的节点过大，则需要向下找
				if (p->right == nullptr || p->right->data > e)
				{
					p = p->down; //向下
				}
				else
				{
					//找到了要删除的节点
					Node<T>* pdelnode = p->right; //pdlnode是要删除的节点，而p是要删除的节点左侧的节点
					iffind = true;
					p->right = p->right->right; //要删除节点左侧的节点指向要删除节点右侧的节点					
					delete pdelnode; //释放要删除的节点的内存，注意删除位置放这里比较合适
					p = p->down;
				}
			} //end while

			//如果某级索引只有一个节点，删除该节点后，可能会导致该该级索引一个节点也不存在，只有索引头节点存在，不过这并不要紧，无需处理
			return iffind;
		}

		//查找数据
		template <typename T>
		bool Find(const T& e)
		{
			Node<T>* p = m_head;
			int jumptimes = 0; //寻找进行的指针跳跃次数统计

			while (p)
			{
				//向右找合适的节点位置
				while (p->right != nullptr && p->right->data < e)
				{
					jumptimes++;
					p = p->right; //先往右走
				}

				//右边没节点了，或者右边的节点过大，则需要向下找
				if (p->right == nullptr || p->right->data > e)
				{
					jumptimes++;
					p = p->down; //向下
				}
				else
				{
					//找到了
					cout << "Find成功找到元素:" << e << "共进行:" << jumptimes << "次跳跃。" << endl;
					return true;
				}
			} //end while
			cout << "Find寻找元素:" << e << "失败!" << endl;
			return false;
		}


		//输出跳表中的所有元素，从上到下，从左到右输出
		void DispSkipList()
		{	
			if (m_head->right == nullptr && m_head->down == nullptr)
				return; //空表没啥可输出的

			cout << "--------------begin----------------" << endl;
			Node<T>* tmphead = m_head;
			int level = 0; //统计下有多少级索引
			while (true)
			{	
				Node<T>* currp = tmphead->right;
				while (currp != nullptr)
				{
					cout << currp->data << "  ";					
					currp = currp->right;
				} //end while									
				cout << endl;
				level++;
				tmphead = tmphead->down; //下走
				if (tmphead == nullptr)
					break;				
			} //end while
			cout << "--------------end----------------" << endl;
			cout << "共产生了:" << level-1 << "级索引。" << endl;
			return;
		}

		//获取跳表的高度（有多少级索引），不计算原始链表高度		
		int getlevel()
		{
			if (m_head->down == nullptr)
				return 0;

			Node<T>* p = m_head;
			int level = 0; 
			while (p)
			{
				level++;
				p = p->down; //下走
			}
			return level - 1;
		}

	private:	
		Node<T>* m_head; //头指针，指向最上级索引的头节点
		vector<Node<T> *> pathList; //#include <vector>，记录这从索引到最底下的原始链表的向下行走路径		
	};


}

int main()
{	
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);//程序退出时检测内存泄漏并显示到“输出”窗口
	
	_nmsp1::SkipList<int> myskiplist;		
	
	myskiplist.Insert(3);
	myskiplist.Insert(1);
	myskiplist.Insert(7);
	myskiplist.Insert(6);	
	myskiplist.Insert(9);
	myskiplist.Insert(5);
	myskiplist.Insert(15);
	myskiplist.Insert(2);	
	myskiplist.DispSkipList();
		
	myskiplist.Delete(1);	
	/*myskiplist.DispSkipList();

	
	myskiplist.Delete(2);
	myskiplist.DispSkipList();

	myskiplist.Delete(3);
	myskiplist.DispSkipList();

	myskiplist.Delete(5);
	myskiplist.DispSkipList();

	myskiplist.Delete(6);
	myskiplist.DispSkipList();

	myskiplist.Delete(7);
	myskiplist.DispSkipList();

	myskiplist.Delete(9);
	myskiplist.DispSkipList();

	myskiplist.Delete(15);
	myskiplist.DispSkipList();

	myskiplist.Insert(5);
	myskiplist.Insert(15);
	myskiplist.Insert(2);*/
	myskiplist.DispSkipList();

	for (int i = 0; i < 21000000; ++i) //搞大量数据进行插入和查找测试
	{
		myskiplist.Insert(i);
	}

	myskiplist.Find(1);
	cout << "当前跳表索引高度：" << myskiplist.getlevel() << endl;

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




