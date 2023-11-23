// MyProject.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include  <assert.h>
#include <stack>

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
#define MaxVertices_size 10 //最大顶点数大小

	//表示边的节点结构
	struct EdgeNode
	{
		int curridx; //边所对应的顶点下标值
		int weight; //权值
		EdgeNode* next; //指向下一条边
	};

	//表示顶点的节点结构，其后是一个链表，链表中每个节点都代表着和该顶点相连的边
	template<typename T>  //T代表顶点类型
	struct VertexNode
	{
		int indegree; //入度
		int outdegree; //出度
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
				m_VertexArray[i].outdegree = 0;    //出度先给0
				m_VertexArray[i].indegree = 0;    //入度先给0
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
		bool InsertEdge(const T& tmpv1, const T& tmpv2, int weight) //在tmpv1和tmpv2两个顶点之间插入一条边
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
			ptmp->weight = weight;
			ptmp->next = m_VertexArray[idx1].point;  //为简化编码和提升代码执行效率，采用头插法将边节点插入到单链表的最前面
			m_VertexArray[idx1].point = ptmp;
			m_VertexArray[idx1].outdegree++; //开始顶点出度数增加
			m_VertexArray[idx2].indegree++; //终止顶点入度数增加
			m_numEdges++; //边数量增加1
			return true;
		}

		void DispGraph() //显示图信息
		{
			for (int i = 0; i < m_numVertices; ++i)
			{
				cout << i << "   入度/出度(" << m_VertexArray[i].indegree << "/" << m_VertexArray[i].outdegree << ")" << " " << m_VertexArray[i].data << "：-->";   //输出顶点下标和顶点数据
				EdgeNode* ptmp = m_VertexArray[i].point;
				while (ptmp != nullptr)
				{
					cout << ptmp->curridx << "(权值:" << ptmp->weight << ")-->";  //输出顶点相关的边索引（编号）
					ptmp = ptmp->next;
				}
				cout << "nullptr" << endl; //显示指向nullptr并换行
			} //end for
			cout << "图中有顶点" << m_numVertices << "个，边" << m_numEdges << "条!" << endl;
		}

		//拓扑排序算法
		bool TopologicalSort(int* pPopResult, int* pve)
		{
			int* pInVexDegree = new int[m_numVertices]; //分配空间记录顶点入度
			memset(pInVexDegree, 0, sizeof(int) * m_numVertices); //清0

			//顶点的入度值先拿过来
			for (int i = 0; i < m_numVertices; ++i)
			{
				pInVexDegree[i] = m_VertexArray[i].indegree;
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

			int iOutputVexcount = 0; //输出的顶点数量统计
			//栈不为空则循环			
			while (tempstack.empty() == false)
			{
				//出栈
				static int sign = 0;
				if (sign == 0)
				{
					sign = 1;
					cout << "拓扑排序的结果为：   ";
				}
				int topidx = tempstack.top(); //获取栈顶元素
				cout << m_VertexArray[topidx].data << "   ";  //输出没有前趋的顶点

				pPopResult[iOutputVexcount] = topidx; //记录出栈的元素顺序

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
						if (pInVexDegree[tmpidx] == 0)//入度为0的点入栈
							tempstack.push(tmpidx);
					}

					//顺带计算事件的最早发生时间ve供后续CriticalPath计算关键路径使用
					if (pve[tmpidx] < (pve[topidx] + pEdgenode->weight))
					{
						pve[tmpidx] = pve[topidx] + pEdgenode->weight;
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

		//求关键路径
		bool CriticalPath()
		{
			//在AOE网中只有一个入度为0的顶点，称为开始顶点，也只有一个出度为0的顶点，称为结束顶点
			int iStartVerIdx = -1;
			int iEndVerIdx = -1;
			for (int i = 0; i < m_numVertices; ++i)
			{
				if (m_VertexArray[i].indegree == 0)
				{
					if (iStartVerIdx != -1)
					{
						cout << "图中发现超过1个入度为0的节点，非法AOE网，不能求关键路径";
						return false;
					}
					iStartVerIdx = i;
				} //end if

				if (m_VertexArray[i].outdegree == 0)
				{
					if (iEndVerIdx != -1)
					{
						cout << "图中发现超过1个出度为0的节点，非法AOE网，不能求关键路径";
						return false;
					}
					iEndVerIdx = i;
				}
			} //end for i

			//(1)事件的最早发生时间ve分配内存准备开始计算：
			int* pve = new int[m_numVertices];
			memset(pve, 0, sizeof(int) * m_numVertices); //清0

			//这个用来计算后续的vl用的
			int* pPopResult = new int[m_numVertices];
			memset(pve, 0, sizeof(int) * m_numVertices);

			//通过拓扑排序能够得到ve值（当然不通过拓扑排序而是单独计算ve值也可以）：
			if (TopologicalSort(pPopResult, pve) == false)
			{
				//内存不要忘记释放
				delete[] pve;
				delete[] pPopResult;
				return false; //图中有环，直接返回
			}

			//拓扑排序可能的结果为： A   B   C   E   G   D   F   H   I
			//pve结果应该为：0,6,4,5,7,7,16,14,18
			//pPopResult的结果应该为：0,1,2,4,6,3,5,7,8

			//(2)事件的最迟发生时间vl计算，注意vl值是从后向前求的：
			int* pvl = new int[m_numVertices];
			memset(pvl, 0, sizeof(int) * m_numVertices);

			int toppos = m_numVertices - 1; //栈顶位置=9-1=8
			int vexIdx = pPopResult[toppos]; //栈顶位置所代表的顶点的下标值
			toppos--;

			for (int i = 0; i < m_numVertices; ++i)
			{
				//初始化vl值，vl值一般都比最大的ve值(pve[idxTop])小，所以把最大ve值先给vl没问题，后续要进行min判断
				pvl[i] = pve[vexIdx]; //18
			} //end for

			while (toppos >= 0) //栈里有数据
			{
				int fromVexIdx = pPopResult[toppos];  //7：出栈
				toppos--;
				EdgeNode* pTmpEdge = m_VertexArray[fromVexIdx].point; //下标7所代表的顶点为H，这里拿到H指向的第一条边
				while (pTmpEdge != nullptr) //遍历顶点H指向的其他边
				{
					int toVexIdx = pTmpEdge->curridx; //8
					if (pvl[fromVexIdx] > (pvl[toVexIdx] - pTmpEdge->weight))
						pvl[fromVexIdx] = pvl[toVexIdx] - pTmpEdge->weight;

					pTmpEdge = pTmpEdge->next;
				} //end while (pTmpEdge != nullptr)
			} //end while(toppos >= 0)

			//pvl结果应该为： 0  6  6  8  7  10  16  14  18

			//(3)活动的最早开始时间ee计算，该值需要通过ve求得。
			//(4)活动的最晚开始时间el计算，该值需要通过vl求得。
			//这里不用分配内存并进行计算，只需要求得ee和el临时值，比较他们是否相等就可以得到关键路径了
			int tmpee, tmpel;
			cout << "关键路径如下：" << endl;
			for (int fromVexIdx = 0; fromVexIdx < m_numVertices; ++fromVexIdx) //遍历所有顶点
			{
				EdgeNode* pTmpEdge = m_VertexArray[fromVexIdx].point; //该顶点指向的边信息
				while (pTmpEdge != nullptr)
				{
					int toVexIdx = pTmpEdge->curridx;
					int toWeight = pTmpEdge->weight;

					tmpee = pve[fromVexIdx];  //活动最早开始时间
					tmpel = pvl[toVexIdx] - toWeight;  //活动最晚开始时间

					//活动最早开始时间和活动最晚开始时间相等，这属于关键路径上的活动
					if (tmpee == tmpel)
					{
						//用“<顶点1,顶点2>(权值=?)”形式表示顶点之间的弧
						//结果形如：<A,B>(权值=6) <B,E>(权值=1) <E,H>(权值=7) <E,G>(权值=9) <G,I>(权值=2) <H,I>(权值=4)
						cout << "<" << m_VertexArray[fromVexIdx].data << "," << m_VertexArray[toVexIdx].data << ">(权值=" << toWeight << ") ";
					}
					pTmpEdge = pTmpEdge->next;
				}//end while
			}//end for		

			//释放内存
			delete[] pve;
			delete[] pvl;
			delete[] pPopResult;
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

	_nmsp1::GraphLink<char> gm;
	gm.InsertVertex('A');
	gm.InsertVertex('B');
	gm.InsertVertex('C');
	gm.InsertVertex('D');
	gm.InsertVertex('E');
	gm.InsertVertex('F');
	gm.InsertVertex('G');
	gm.InsertVertex('H');
	gm.InsertVertex('I');

	//向图中插入边
	gm.InsertEdge('A', 'B', 6);
	gm.InsertEdge('A', 'C', 4);
	gm.InsertEdge('A', 'D', 5);
	gm.InsertEdge('B', 'E', 1);
	gm.InsertEdge('C', 'E', 1);
	gm.InsertEdge('D', 'F', 2);
	gm.InsertEdge('E', 'G', 9);
	gm.InsertEdge('E', 'H', 7);
	gm.InsertEdge('F', 'H', 4);
	gm.InsertEdge('G', 'I', 2);
	gm.InsertEdge('H', 'I', 4);
	gm.DispGraph();
	gm.CriticalPath();
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




