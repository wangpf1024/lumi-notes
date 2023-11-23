// MyProject.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

#include <iostream>
#include  <assert.h>
#include <stack>


using namespace std;

//------------------------------
namespace _nmsp1
{
#define MaxVertices_size 10 //最大顶点数大小

	//表示边的节点结构
	struct EdgeNode
	{
		int curridx; //边所对应的顶点下标值
		EdgeNode* next; //指向下一条边
	};

	//表示顶点的节点结构，其后是一个链表，链表中每个节点都代表着和该顶点相连的边
	template<typename T>  //T代表顶点类型
	struct VertexNode
	{
		T data;    //顶点中的数据
		EdgeNode* point; //指向第一个边节点的指针
	};

	//邻接表代表的图
	template<typename T>  //T代表顶点类型
	class GraphLink
	{
	public:
		GraphLink() //构造函数
		{
			m_numVertices = 0;
			m_numEdges = 0;
			for (int i = 0; i < MaxVertices_size; ++i)
			{
				m_VertexArray[i].point = nullptr; //指针可以率先指向nullptr
			}
		}
		~GraphLink() //析构函数
		{
			for (int i = 0; i < m_numVertices; ++i)
			{
				EdgeNode* ptmp = m_VertexArray[i].point;
				while (ptmp != nullptr)
				{
					m_VertexArray[i].point = ptmp->next;
					delete ptmp;
					ptmp = m_VertexArray[i].point;
				} //end while
				m_VertexArray[i].point = nullptr;
			} //end for i
		}

	public:
		//插入顶点
		bool InsertVertex(const T& tmpv)
		{
			if (m_numVertices >= MaxVertices_size) //顶点空间已满
			{
				cout << "顶点空间已满" << endl;
				return false;
			}

			if (GetVertexIdx(tmpv) != -1) //该顶点已经存在
			{
				cout << "顶点 " << tmpv << " 已经存在!" << endl;
				return false;
			}
			m_VertexArray[m_numVertices].data = tmpv;
			m_VertexArray[m_numVertices].point = nullptr;
			m_numVertices++;
			return true;
		}

		//插入边
		bool InsertEdge(const T& tmpv1, const T& tmpv2) //在tmpv1和tmpv2两个顶点之间插入一条边
		{
			int idx1 = GetVertexIdx(tmpv1);
			int idx2 = GetVertexIdx(tmpv2);
			if (idx1 == -1 || idx2 == -1) //某个顶点不存在，不可以插入边
				return false;

			//判断是否边重复
			EdgeNode* ptmp = m_VertexArray[idx1].point;
			while (ptmp != nullptr)
			{
				if (ptmp->curridx == idx2)
					return false; //边重复
				ptmp = ptmp->next;
			}

			//可以正常插入
			ptmp = new EdgeNode;
			ptmp->curridx = idx2;
			ptmp->next = m_VertexArray[idx1].point;  //为简化编码和提升代码执行效率，采用头插法将边节点插入到单链表的最前面
			m_VertexArray[idx1].point = ptmp;

			m_numEdges++; //边数量增加1
			return true;
		}


		void DispGraph() //显示图信息
		{
			for (int i = 0; i < m_numVertices; ++i)
			{
				cout << i << "   " << m_VertexArray[i].data << "：-->";   //输出顶点下标和顶点数据
				EdgeNode* ptmp = m_VertexArray[i].point;
				while (ptmp != nullptr)
				{
					cout << ptmp->curridx << "-->";  //输出顶点相关的边索引（编号）
					ptmp = ptmp->next;
				}
				cout << "nullptr" << endl; //显示指向nullptr并换行
			} //end for
			cout << "图中有顶点" << m_numVertices << "个，边" << m_numEdges << "条!" << endl;
		}

		//拓扑排序算法
		bool TopologicalSort()
		{
			int* pInVexDegree = new int[m_numVertices]; //分配空间记录顶点入度
			memset(pInVexDegree, 0, sizeof(int) * m_numVertices); //清0

			//统计各个顶点的入度值			
			for (int i = 0; i < m_numVertices; ++i)
			{
				EdgeNode* pEdgenode = m_VertexArray[i].point;
				while (pEdgenode != nullptr)
				{
					pInVexDegree[pEdgenode->curridx]++; //注意[]中内容
					pEdgenode = pEdgenode->next;
				} //end while				
			} //end for			

			//将入度为0的顶点先入栈
			std::stack<int> tempstack; //#include <stack>
			for (int i = 0; i < m_numVertices; ++i)
			{
				if (pInVexDegree[i] == 0)
				{
					tempstack.push(i);
				}
			} //end for

			int fuzauducount = 0; //计算时间复杂度用的计数，专门统计入度值-1操作的计算次数
			int iOutputVexcount = 0; //输出的顶点数量统计
			//栈不为空则循环			
			while (tempstack.empty() == false)
			{
				//出栈
				int topidx = tempstack.top(); //获取栈顶元素
				cout << m_VertexArray[topidx].data << "   ";  //输出没有前趋的顶点
				iOutputVexcount++;  //输出的拓扑顶点数量统计
				tempstack.pop(); //删除栈顶元素

				//要将topidx对应顶点的各个邻接点入度减1，所以要先找到第一条边
				EdgeNode* pEdgenode = m_VertexArray[topidx].point;
				while (pEdgenode != nullptr)
				{
					int tmpidx = pEdgenode->curridx;
					if (pInVexDegree[tmpidx] != 0) //入度已经为0的顶点，不理会
					{
						pInVexDegree[tmpidx]--; //入度值减1
						fuzauducount++;
						if (pInVexDegree[tmpidx] == 0)//入度为0的点入栈
							tempstack.push(tmpidx);
					}
					pEdgenode = pEdgenode->next;
				} //end while	
			} //end while
			cout << endl; //换行
			delete[] pInVexDegree;

			if (iOutputVexcount != m_numVertices) //拓扑排序失败
			{
				cout << "输出顶点数量:" << iOutputVexcount << ",而图中实际顶点数量:" << m_numVertices << ",说明图中有环，没办法输出所有顶点序列（非AOV网，拓扑排序错误）" << endl;
				return false;
			}
			return true;
		}

	private:
		//获取顶点下标
		int GetVertexIdx(const T& tmpv)
		{
			for (int i = 0; i < m_numVertices; ++i)
			{
				if (m_VertexArray[i].data == tmpv)
					return i;
			}
			return -1; //不存在的顶点
		}
	private:
		int m_numVertices;    //当前顶点数量
		int m_numEdges;       //边数量
		VertexNode<T>  m_VertexArray[MaxVertices_size]; //顶点数组
	};

}

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);//程序退出时检测内存泄漏并显示到“输出”窗口

	_nmsp1::GraphLink<char> gm2;
	//向图中插入顶点
	gm2.InsertVertex('A');
	gm2.InsertVertex('B');
	gm2.InsertVertex('C');
	gm2.InsertVertex('D');
	gm2.InsertVertex('E');
	gm2.InsertVertex('F');
	gm2.InsertVertex('G');
	gm2.InsertVertex('H');

	//向图中插入边
	gm2.InsertEdge('A', 'C');
	gm2.InsertEdge('A', 'D');
	gm2.InsertEdge('B', 'C');
	gm2.InsertEdge('C', 'E');
	gm2.InsertEdge('D', 'F');
	gm2.InsertEdge('E', 'G');
	gm2.InsertEdge('F', 'G');
	gm2.InsertEdge('G', 'H');
	//gm2.InsertEdge('H', 'G');//模拟图中有环的情形
	gm2.DispGraph();
	cout << "拓扑排序的结果为：   ";
	gm2.TopologicalSort();



	//_nmsp1::GraphLink<char> gm2;
	////向图中插入顶点
	//gm2.InsertVertex('A');
	//gm2.InsertVertex('B');
	//gm2.InsertVertex('C');
	//gm2.InsertVertex('D');
	//gm2.InsertVertex('E');
	//gm2.InsertVertex('F');
	////向图中插入边
	//gm2.InsertEdge('A', 'D');
	//gm2.InsertEdge('B', 'A');
	//gm2.InsertEdge('B', 'F');
	//gm2.InsertEdge('C', 'A');
	//gm2.InsertEdge('C', 'F');
	//gm2.InsertEdge('D', 'F');
	//gm2.InsertEdge('E', 'B');
	//gm2.DispGraph();
	//gm2.TopologicalSort();

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

