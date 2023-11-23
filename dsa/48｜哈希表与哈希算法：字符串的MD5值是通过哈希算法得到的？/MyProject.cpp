// MyProject.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

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
	#define MAX_HASHTBLSIZE 37 //哈希表最大长度 ，素数保证分布尽量均匀

	//每个节点的定义
	template <typename T>
	struct ListNode
	{
		T            data;  //数据
		ListNode<T>* next;  //指针
	};

	//哈希表定义
	template <typename T> 
	class myHashTbl
	{
	public:
		myHashTbl() //构造函数
		{
			for (int i = 0; i < MAX_HASHTBLSIZE; ++i)
			{
				m_bucketArray[i] = nullptr;
			} //end for
		}

		~myHashTbl() //析构函数
		{
			for (int i = 0; i < MAX_HASHTBLSIZE; ++i)
			{
				if (m_bucketArray[i] == nullptr)
					continue;

				ListNode<T>* pnode = m_bucketArray[i];
				ListNode<T>* ptmp;
				while (pnode != nullptr)
				{
					ptmp = pnode;
					pnode = pnode->next;
					delete ptmp;
				}				
			} //end for
		}

		//增加元素
		void add(T key)
		{
			int idx = key % MAX_HASHTBLSIZE; //除留余数法，先得到桶的位置
			if (IfInBucket(m_bucketArray[idx], key))   //检查链表中是否已经存在该键，如果存在，则什么也不做
				return;

			//创建节点
			ListNode<T>* tempnode = new ListNode<T>;
			tempnode->data = key;  //这里只是把键插入到哈希表中，如果想让程序变得更实用，可以改造代码来插入各种数据到哈希表中

			//头插法
			tempnode->next = m_bucketArray[idx];
			m_bucketArray[idx] = tempnode; 
		}
		
		//查看哈希表中查找指定键
		bool Find(T key) 
		{
			int idx = key % MAX_HASHTBLSIZE;
			bool bfind = IfInBucket(m_bucketArray[idx], key);
			if (bfind == true)
			{
				cout << "在下标为[" << idx << "]的桶中找到了元素" << key << endl;
			}
			else
			{
				cout << "没有找到元素" << key << endl;
			}
			return bfind;
		}

		//从哈希表中删除key代表的项
		void Delete(T key)
		{
			int idx = key % MAX_HASHTBLSIZE;

			if (m_bucketArray[idx] == nullptr) //空桶
				return;

			ListNode<T>* parent = nullptr;
			ListNode<T> *tempnode = m_bucketArray[idx];
			while (tempnode != nullptr)
			{
				if (tempnode->data == key) //找到了键
				{
					if (parent == nullptr) //桶中第一个元素就是要找的元素
						m_bucketArray[idx] = tempnode->next;
					else
						parent->next = tempnode->next;
					delete tempnode;
					break;
				}
				parent = tempnode;
				tempnode = tempnode->next;
			} //end while
			return;
		}

		//判断是否某个key在桶中
		bool IfInBucket(ListNode<T>* head, T key)
		{
			ListNode<T>* tempnode = head;
			while (tempnode)
			{
				if (tempnode->data == key)
					return true;
				tempnode = tempnode->next;
			}
			return false;			
		}

		//输出哈希表中元素
		void DispHashList()
		{
			cout << "--------begin--------------" << endl;
			for (int i = 0; i < MAX_HASHTBLSIZE; ++i)
			{
				if (m_bucketArray[i] == nullptr)
				{
					cout << "[桶下标:"  << i << "]= nullptr" << endl;
					continue;
				}

				cout << "[桶下标:" << i << "]= ";
				ListNode<T>* p = m_bucketArray[i];
				while (p != nullptr)
				{
					cout << p->data << " "; 
					p = p->next;
				}
				cout << endl; //换行
			} //end for
			cout << "--------end--------------" << endl;
		}

	private:
		ListNode<T>* m_bucketArray[MAX_HASHTBLSIZE];
	};


}

int main()
{	
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);//程序退出时检测内存泄漏并显示到“输出”窗口
	
	_nmsp1::myHashTbl<int> myhashtbl;

	for (int i = 0; i < 100; ++i)
	{
		myhashtbl.add(i);
	}
	myhashtbl.DispHashList();
	myhashtbl.Find(24);	
	for (int i = 100; i >= 13; --i)
	{
		myhashtbl.Delete(i);
	}	
	myhashtbl.Find(24);
	myhashtbl.DispHashList();
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




