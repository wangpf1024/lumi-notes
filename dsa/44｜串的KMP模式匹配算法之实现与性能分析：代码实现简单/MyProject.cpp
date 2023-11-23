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
	#define MAX_LEN 250 //最大字符串长度（定长）
		
	//采用定长数组存储结构
	class MySString
	{
	public:
		MySString()//构造函数
		{
			ch[0] = '\0'; //字符串结束标记，其实'\0'就是数字0，所以写成ch[0] = 0;也没问题
			length = 0; //字符串长度
		}

		//串赋值
		void StrAssign(const char* pcontent)
		{
			size_t iLen = strlen(pcontent);
			if (iLen >= MAX_LEN) //内容太长，容纳不下，字符串存储中要给字符串结束标记'\0'留出位置
				return; 				

			for (int i = 0; i < iLen; ++i)
			{
				ch[i] = pcontent[i];
			} //end for
			ch[iLen] = '\0'; //设置字符串结束标记，该标记不计入字符串长度中
			length = iLen; //记录字符串长度
		}

		//串拷贝（串复制）
		void StrCopy(const MySString &tmpstr)
		{
			for (int i = 0; i < tmpstr.length; ++i)
			{
				ch[i] = tmpstr.ch[i];
			} //end for			
			length = tmpstr.length;
			ch[length] = '\0';
		}

		//判断空串
		bool IfStrEmpty()
		{
			if (ch[0] == '\0')
				return true;
			return false;
		}
		
		//串比较，比较其实就是逐个比较两个字符串中每个字符的ASCII码
		//结果 大于返回1，等于返回0，小于返回-1
		int StrCmp(const MySString& tmpstr)
		{
			if (length == 0 && tmpstr.length ==0) //两个字符串都是空的，相等
				return 0;

			const char* p1 = ch;
			const char* p2 = tmpstr.ch;

			int result = 0;
			while (*p1 != '\0' && *p2 != '\0')
			{
				result = (*p1) - (*p2);
				if (result != 0)
				{
					if (result > 0)
						return 1;
					else
						return -1;
				}
				p1++;
				p2++;
			} //end while
						
			if (*p1 == '\0' && *p2 == '\0') //长度相同且内容相等
				return 0;

			//能走到下边流程的都是两个字符串一个长一个短，但长的和短的字符串的前面内容相同，比如字符串"ab"和"abc"
			if (*p1 == '\0')  //p1小，因为长度少
				return -1; 
			
			//else if (*p2 == '\0')
			return 1;
		}

		//串连接
		bool StrCat(const MySString& tmpstr)
		{
			if (length + tmpstr.length >= MAX_LEN) //空间不够保存不下，这里直接返回false以通知开发者
				return false;

			int idx = 0;
			size_t i;
			for (i = length; i < (length + tmpstr.length); ++i)
			{
				ch[i] = tmpstr.ch[idx++];
			}
			ch[i] = '\0'; //字符串结束标记
			length += tmpstr.length;
			return true;
		}

		//获取串的一部分内容（求子串）
		void SubString(MySString& resultstr, int pos, int len) //求得的子串给到resultstr。pos：从该位置开始[注意位置从0开始计算]，截取len个字符
		{
			//注意pos位置从0开始计算
			if(pos < 0 || (pos + 1) > length || len <= 0)//pos位置要合法，len长度值要合法
				return;

			int icount = 0; 
			while(true)
			{
				resultstr.ch[icount] = ch[pos+ icount];
				icount++;
				if (icount == len) //截取够数量了
					break;
				if (ch[pos + icount] == '\0') //到主串末尾了，不够截取，截取多少算多少，直接跳出循环
					break;
			} //end while
			resultstr.length = icount;
			resultstr.ch[resultstr.length] = '\0';
			return;
		}

		//串插入
		//在当前串的pos位置（从0开始计算），插入子串substr
		void StrInsert(int pos, const MySString& substr)
		{
			if (pos < 0 || pos > length) //插入位置不合法
				return;

			if (length + substr.length >= MAX_LEN) //容纳不下插入的新内容，则直接返回
				return;

			//把原来的必须的内容向后挪动
			int i = (int)(length - 1); //i为int类型，这样就可以为负数，保证下面这个for循环可以正确结束
			for (; i >= pos; --i) 
			{
				ch[i + substr.length] = ch[i];
			}
			//把子串插入进来			
			for (size_t i = 0; i < substr.length; ++i)
			{			
				ch[pos + i] = substr.ch[i];
			}
			length += substr.length;
			ch[length] = '\0';
			return;
		}

		//串删除
		//在当前串的pos位置（从0开始计算），删除len个字符
		void StrDelete(int pos, int len)
		{
			//注意pos位置从0开始计算
			if (pos < 0 || (pos + 1) > length || len <= 0)//pos位置要合法，len长度值要合法
				return;

			if (pos + len > length)
			{
				//要删除的字符太多，串中没那么多可删的字符
				len = int(length - pos);  //只能删除这么多
			}

			//把剩余的字符串搬位置（向左搬）			
			for (int i = pos; i < length; ++i)
			{
				ch[i] = ch[i + len];
			} //end for

			length = length - len;
			ch[length] = '\0';
			return;
		}

		//串清空
		void StrClear()
		{
			ch[0] = '\0';
			length = 0;
			return;
		}

		//朴素模式匹配算法接口，返回子串中第一个字符在主串中的下标，如果没找到子串，则返回-1
		int StrIndex(const MySString& substr,int pos = 0) //默认从位置0开始匹配子串
		{
			if (length < substr.length) //主串还没子串长，那不可能找到
				return -1;

			int point1 = pos, point2 = 0;
			while (ch[point1] != '\0' && substr.ch[point2] != '\0')
			{
				if (ch[point1] == substr.ch[point2])
				{
					//两个指针都向后走
					point1++;
					point2++;
				}
				else //两者不同
				{
					//两个指针都恢复到该恢复的位置
					point1 = point1 - point2 + 1;
					point2 = 0; //子串指针恢复到0，不管原来是多少。
				}
			} //end while

			if (substr.ch[point2] == '\0')
			{
				//找到了子串
				return point1 - point2;
			}
			return -1;
		}

		//串替换，主串中遇到内容为substr1的子串，全部替换成substr2子串
		void StrReplace(const MySString& substr1, const MySString& substr2)
		{
			int idx = -1;
			int pos = 0;
			int addlength = int(substr2.length) - int(substr1.length); 

			while (pos < length)
			{
				idx = StrIndex(substr1, pos);
				if (idx == -1)
					return; //不存在substr1这样的子串，无需替换直接返回

				//能否替换，取决于空间是否够
				if (addlength > 0)
				{
					//被替换成的子串更长，要判断保存空间是否够
					if (length + addlength >= MAX_LEN)
						return; //空间不够，不能替换成新内容
				}

				StrDelete(idx, int(substr1.length));  //删除老子串
				StrInsert(idx, substr2); //插入新子串

				//位置应该向后走，跳过被替换的子串部分内容
				pos = idx + (int)(substr2.length);
			} //end while
			return;
		}

		//求本串的next数组
		void getNextArray(int next[])
		{
			//next数组下标为0和为1的元素值固定为0和1。其实next[0]里的值并没有用到
			if (length < 1)
				return;

			//next数组的前两个元素肯定是0和1
			if (length == 1) //只有一个字符
			{
				next[0] = 0;
				return;
			}

			next[0] = 0;
			next[1] = 1;
			if (length == 2) //只有二个字符
			{
				return;
			}

			//至少三个字符
			int nextarry_idx = 2; //当前要处理的next数组下标
			int max_pub_zhui = 0; //max_pub_zhui：最大公共前后缀包含的字符数量

			//循环的目的是给next数组赋值
			while (nextarry_idx < length)
			{
				int left_RMC_count = nextarry_idx; //left_RMC_count：如果当前字符与主串的字符不匹配，当前字符左侧有多少个字符
				int max_pub_zhui = left_RMC_count - 1; //max_pub_zhui：最大公共前后缀包含的字符数量

				int start1idx = 0;
				int start2idx = left_RMC_count - max_pub_zhui;

				int xhtimes = max_pub_zhui; //循环次数

				//本循环的目的是获取最长公共前后缀长度，代码写法无固定方式，选择自己容易理解的方式写即可
				while (xhtimes > 0)
				{
					if (ch[start1idx] != ch[start2idx])
					{
						max_pub_zhui--;
						start1idx = 0;
						start2idx = left_RMC_count - max_pub_zhui;
						xhtimes = max_pub_zhui;
						continue; //要回去重新循环
					}
					else
					{
						start1idx++;
						start2idx++;
					}
					xhtimes--;
				} //end while (xhtimes > 0)
				next[nextarry_idx] = max_pub_zhui + 1; //如果公共前后缀长度为n，那么就需要用子串的第n+1个字符与主串当前位做比较。
				nextarry_idx++;
			} //end while
			return;
		}

		//求本串的next数组——典型的KMP算法求解next数组的代码写法
		void getNextArray_Classic(int next[])
		{			
			if (length < 1)
				return;

			//next数组的前两个元素肯定是0和1
			if (length == 1) //只有一个字符
			{
				next[0] = 0;
				return;
			}

			next[0] = 0;
			next[1] = 1;
			if (length == 2) //只有二个字符
			{
				return;
			}

			//至少三个字符			
			int next_idx = 2;    //需要求的next数组中下标为2的元素值
			int qz_tail_idx = 0; //前缀末尾位置
			
			while (next_idx < length)
			{
				if (ch[qz_tail_idx] == ch[next_idx - 1])   //next_idx - 1代表后缀末尾位置
				{
					next[next_idx] = (qz_tail_idx + 1) + 1;   //qz_tail_idx+1就是前缀的宽度
					next_idx++; 					
					qz_tail_idx++; //前缀末尾位置：其实这样写也OK：qz_tail_idx = next[next_idx - 1] - 1; 
				}
				else  
				{	
					qz_tail_idx = next[qz_tail_idx] - 1; //这句是最难理解的代码

					//qz_tail_idx允许等于0，等于0有机会下次while时再比较一次 ，所以下面只判断qz_tail_idx < 0 的情形
					if (qz_tail_idx < 0)
					{
						//没找到前缀
						qz_tail_idx = 0;
						next[next_idx] = 1;
						++next_idx;						
					}
				}
			} //end while (next_idx < length)
			return;
		}

		//KMP模式匹配算法接口，返回子串中第一个字符在主串中的下标，如果没找到子串，则返回-1
		//nextornextval：下一步数组（前缀表/前缀数组）
		//pos：从主串的什么位置开始匹配子串，默认从位置0开始匹配子串 
		int StrKMPIndex(const MySString& substr, int nextornextval[], int pos = 0)
		{
			if (length < substr.length) //主串还没子串长，那不可能找到
				return -1;		

			int point1 = pos; //指向主串
			int point2 = 0;  //指向子串

			while (ch[point1] != '\0' && substr.ch[point2] != '\0')
			{
				if (ch[point1] == substr.ch[point2])
				{
					//两个指针都向后走
					point1++;
					point2++;
				}
				else //两者不同
				{
					//point1和point2两个指针的处理
					
					if (point2 == 0) //图7.5.3_1可以看到，下标0号位置子串的字符如果与主串字符不匹配则后续就要用子串的第1个字符（字符a）与主串下一位（1号下标位）字符做比较
					{
						point1++; //主串指针指向下一位
					}
					else
					{
						//新增对nextval数组的处理
						if (nextornextval[point2] == 0) //因为nextval数组的任何元素都可能等于0
						{
							point1++; //主串指针指向下一位
							point2 = 0; //子串的point2值会指向子串的开始位置
						}
						else
						{ 
							//走到这个分支的，主串指针point1不用动，只动子串指针point2
							point2 = nextornextval[point2] - 1; //第这些个子串中的字符与主串当前位字符做比较
						}
					}
				}
			}//end while

			if (substr.ch[point2] == '\0')
			{
				//找到了子串
				return point1 - point2;
			}
			return -1;
		}


		//根据给的nextval的下标值，求nextval[idx]的值
		void getNextValArray(int i, int next[], int nextval[])
		{
			//数组nextval下标i为0的元素值固定为0
			if (i == 0)
			{
				nextval[i] = 0;
			}
			else //数组nextval下标i > 0时，分两种情况：
			{				
				if (ch[i] != ch[next[i] - 1])
				{
					//如果T[i]≠T[next[i]-1]，则nextval[i] 等于next[i]
					nextval[i] = next[i];
				}
				else
				{
					//如果T[i] = T[next[i] - 1]，则nextval[i]等于nextval[next[i] - 1]
					nextval[i] = nextval[next[i] - 1];
				}
			}
			return;
		}

		//求本串的next和nextval数组
		void getNextAndNextValArray_Classic(int next[],int nextval[])
		{
			if (length < 1)
				return;

			//next数组的前两个元素肯定是0和1
			if (length == 1) //只有一个字符
			{
				next[0] = 0;
				return;
			}

			next[0] = 0;
			next[1] = 1;

			getNextValArray(0, next, nextval);
			getNextValArray(1, next, nextval);

			if (length == 2) //只有二个字符
			{
				return;
			}

			//至少三个字符			
			int next_idx = 2;    //需要求的next数组中下标为2的元素值
			int qz_tail_idx = 0; //前缀末尾位置

			while (next_idx < length)
			{
				if (ch[qz_tail_idx] == ch[next_idx - 1])   //next_idx - 1代表后缀末尾位置
				{
					next[next_idx] = (qz_tail_idx + 1) + 1;   //qz_tail_idx+1就是前缀的宽度

					//这里求nextval元素值：求next元素值后就可以求nextval元素值
					getNextValArray(next_idx, next, nextval);

					next_idx++;
					qz_tail_idx++; //前缀末尾位置：其实这样写也OK：qz_tail_idx = next[next_idx - 1] - 1; 
				}
				else
				{
					//qz_tail_idx = next[qz_tail_idx] - 1; //这句是最难理解的代码
					qz_tail_idx = nextval[qz_tail_idx] - 1; //注意可以用这句替换上一句，其实不替换也可以

					//qz_tail_idx允许等于0，等于0有机会下次while时再比较一次 ，所以下面只判断qz_tail_idx < 0 的情形
					if (qz_tail_idx < 0)
					{
						//没找到前缀
						qz_tail_idx = 0;
						next[next_idx] = 1;

						//这里求nextval元素值：求next元素值后就可以求nextval元素值
						getNextValArray(next_idx, next, nextval);

						++next_idx;
					}
				}
			} //end while (next_idx < length)			
			return;
		}

	public:
		//显示字符串内容
		void DispContent()
		{
			cout << ch << endl;
		}
	public:
		char  ch[MAX_LEN]; //串内容。每个位置保存一个字符
		size_t  length; //串实际长度，专门引入该变量保存，提高程序运行效率
	};
	

	/*
	//采用堆中分配内存的存储结构
	class MyHString  //H表示Help（堆）
	{
	public:
		MyHString()//构造函数
		{
			ch = nullptr;
			length = 0;
		}
		~MyHString()//析构函数
		{
			if (length > 0)
				delete[] ch;
		}

		//串赋值
		void StrAssign(const char* pcontent)
		{
			size_t iLen = strlen(pcontent);		

			if (length > 0)
				delete[] ch;

			ch = new char[iLen];
			
			//拷贝字符串
			for (int i = 0; i < iLen; ++i)
			{
				ch[i] = pcontent[i];
			} //end for
			length = iLen;
		}

		//串拷贝（串复制）
		void StrCopy(const MyHString& tmpstr)
		{		
			if (length > 0)
			{
				delete[] ch;
			}
			ch = new char[tmpstr.length];
			for (int i = 0; i < tmpstr.length; ++i)
			{
				ch[i] = tmpstr.ch[i];
			}
			length = tmpstr.length;
			return;
		}

		//判断空串
		bool IfStrEmpty()
		{
			if (length == 0)
				return true;
			return false;
		}

		//串比较，比较其实就是逐个比较两个字符串中每个字符的ASCII码
		//结果 大于返回1，等于返回0，小于返回-1
		int StrCmp(const MyHString& tmpstr)
		{
			if (length == 0 && tmpstr.length == 0) //两个字符串都是空的，相等
				return 0;

			const char* p1 = ch;
			const char* p2 = tmpstr.ch;

			int result = 0;
			int i = 0;
			int j = 0;
			while (i < length && j < tmpstr.length)
			{
				result = ch[i] - tmpstr.ch[j];
				if (result != 0)
				{
					if (result > 0)
						return 1;
					else
						return -1;
				}
				i++;
				j++;
			} //end while

			if(i >= length && j >= tmpstr.length)//长度相同且内容相等
				return 0; 

			//能走到下边流程的都是两个字符串一个长一个短，但长的和短的字符串的前面内容相同，比如字符串"ab"和"abc"
			if (i >= length)  //p1小，因为长度少
				return -1;
			
			return 1;
		}

		//串连接
		void StrCat(const MyHString& tmpstr)
		{
			if (tmpstr.length <= 0) //目标是空串，无须连接
				return;

			char* tmp = new char [length + tmpstr.length];
			for (int i = 0; i < length; ++i)
			{
				tmp[i] = ch[i];
			}
			for (int i = 0; i < tmpstr.length; ++i)
			{
				tmp[i + length] = tmpstr.ch[i];
			}
			if (length > 0) //原来内存释放掉
				delete[] ch;
			ch = tmp;
			length = length + tmpstr.length;
			return;
		}

		//获取串的一部分内容（求子串）
		//求得的子串给到resultstr。pos：从该位置开始[注意位置从0开始计算]，截取len个字符
		void SubString(MyHString& resultstr, int pos, int len) 
		{
			//注意pos位置从0开始计算
			if (pos < 0 || (pos + 1) > length || len <= 0)//pos位置要合法，len长度值要合法
				return;

			if (resultstr.length > 0)
				delete[] resultstr.ch;

			resultstr.ch = new char[len];				
			int icount = 0;
			while (true)
			{
				resultstr.ch[icount] = ch[pos + icount];
				icount++;
				if (icount == len) //截取够数量了
					break;
				if (pos + icount >= length) //到主串末尾了，不够截取，截取多少算多少，直接跳出循环
					break;
			} //end while
			resultstr.length = icount;
			return;
		}

		//串插入
		//在当前串的pos位置（从0开始计算），插入子串substr
		void StrInsert(int pos, const MyHString& substr)
		{
			if (pos < 0 || pos > length) //插入位置不合法
				return;
						
			char* tmp = new char[length + substr.length];
			for (int i = 0; i < length; ++i) //把原来的数据先拷贝到新位置去
			{
				tmp[i] = ch[i];
			}

			if (length > 0) //先把原来的内存释放了
				delete[] ch;

			ch = tmp;
		
			//把原来的必须的内容向后挪动
			int i = (int)(length - 1); //i为int类型，这样就可以为负数，保证下面这个for循环可以正确结束
			for (; i >= pos; --i)
			{
				ch[i + substr.length] = ch[i];
			}
			//把子串插入进来			
			for (size_t i = 0; i < substr.length; ++i)
			{
				ch[pos + i] = substr.ch[i];
			}
			length += substr.length;			
			return;
		}

		//串删除
		//在当前串的pos位置（从0开始计算），删除len个字符
		void StrDelete(int pos, int len)
		{
			//注意pos位置从0开始计算
			if (pos < 0 || (pos + 1) > length || len <= 0)//pos位置要合法，len长度值要合法
				return;

			if (pos + len > length)
			{
				//要删除的字符太多，串中没那么多可删的字符
				len = int(length - pos);  //只能删除这么多
			}

			//把剩余的字符串搬位置（向左搬）			
			for (int i = pos; i < length; ++i)
			{
				ch[i] = ch[i + len];
			} //end for

			length = length - len;			
			return;
		}

		//串清空
		void StrClear()
		{
			if (length > 0)
				delete[] ch;
			length = 0;
			return;
		}

	public:
		//显示字符串内容
		void DispContent()
		{
			for (int i = 0; i < length; ++i)
			{
				cout << ch[i];				
			}			
			cout << endl; //这里可以换一下行
		}		

	public:
		char  *ch;  //空间用new动态分配
		size_t  length;  //串实际长度
	};


	//字符节点（串的链式存储）
	struct StringNode
	{
		char ch[4];
		StringNode* next;  //指针域，指向下一个同类型（和本节点类型相同）节点
	};
	*/
}

int main()
{	
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);//程序退出时检测内存泄漏并显示到“输出”窗口
	
	
	//串赋值
	_nmsp1::MySString mys;
	mys.StrAssign("我爱你中国！");
	mys.DispContent();

	 //串拷贝（串复制）、判断空串
	_nmsp1::MySString mys2;
	cout << "mys2为空吗？" << mys2.IfStrEmpty() << endl;
	mys2.StrCopy(mys);
	mys2.DispContent();
	cout << "mys2为空吗？" << mys2.IfStrEmpty() << endl;

	//串比较
	_nmsp1::MySString mys3,mys4;
	mys3.StrAssign("abc");
	mys4.StrAssign("xyz");
	cout << "mys3和mys4字符串的比较结果为：" << mys3.StrCmp(mys4) << endl;

	//串连接
	_nmsp1::MySString mys5;
	mys5.StrAssign("Hello China!");	
	_nmsp1::MySString mys6;
	mys6.StrAssign("Hello this World!");
	mys6.StrCat(mys5);
	cout << "mys6和mys5连接的结果为：" << mys6.ch << endl;
	
	//获取串的一部分内容（求子串）
	_nmsp1::MySString mys7;
	mys6.SubString(mys7, 0, 12); //子串放入mys7中
	cout << "子串mys7的内容是：" << mys7.ch << endl;

	//串插入（在当前串的pos位置（从0开始计算），插入子串substr）
	_nmsp1::MySString mys8;
	mys8.StrAssign("我爱北京，我爱中国！");
	mys5.StrInsert(12, mys8);
	cout << "插入新内容后的mys5串内容是：" << mys5.ch << endl;
	
	//串删除，在当前串的pos位置（从0开始计算），删除len个字符
	_nmsp1::MySString mys9;
	mys9.StrAssign("Hello China!");
	mys9.StrDelete(1, 10);
	cout << "删除部分内容后的mys9串内容是：" << mys9.ch << endl;

	//串清空
	mys9.StrClear();
	cout << "清空内容后的mys9串内容是：" << mys9.ch << endl;

	//朴素模式匹配算法接口，返回子串中第一个字符在主串中的下标，如果没找到子串，则返回-1
	_nmsp1::MySString mys10, mys11;
	mys10.StrAssign("abdecdefg");//主串
	mys11.StrAssign("def"); //子串
	cout << "StrIndex()结果为" << mys10.StrIndex(mys11) << endl;
	

	//串替换，主串中遇到内容为substr1的，全部替换成substr2
	_nmsp1::MySString mys12, substr1, substr2;
	mys12.StrAssign("abcdeabcde");
	cout << "mys12()替换前的结果为" << mys12.ch << endl;
	substr1.StrAssign("cd");
	substr2.StrAssign("mno");
	mys12.StrReplace(substr1, substr2);
	cout << "mys12()替换后的结果为" << mys12.ch << endl;

	//KMP模式匹配算法接口，返回子串中第一个字符在主串中的下标，如果没找到子串，则返回-1	
	_nmsp1::MySString mys13sub; //子串
	mys13sub.StrAssign("ababaaababaa");
	int* mynextarray = new int[mys13sub.length];
	mys13sub.getNextArray(mynextarray); //获取next数组
	_nmsp1::MySString mys13master; //主串
	mys13master.StrAssign("abbabbababaaababaaa");	
	cout << "StrKMPIndex()结果为" << mys13master.StrKMPIndex(mys13sub, mynextarray) << endl;
	delete[]mynextarray; //释放资源


	//求本串的next数组——典型的KMP算法求解next数组的代码写法
	_nmsp1::MySString mys14sub; //子串
	mys14sub.StrAssign("ababaaababaa");
	int* mynextarray14 = new int[mys14sub.length];
	cout << "本次采用典型的KMP算法求解next数组：----" << endl;
	mys14sub.getNextArray_Classic(mynextarray14);
	_nmsp1::MySString mys14master; //主串
	mys14master.StrAssign("abbabbababaaababaaa");
	cout << "StrKMPIndex()结果为" << mys14master.StrKMPIndex(mys14sub, mynextarray14) << endl;
	delete[]mynextarray14; //释放资源

	//求本串的next和nextval数组——典型的KMP算法求解next数组的代码写法
	_nmsp1::MySString mys15sub; //子串
	mys15sub.StrAssign("ababaaababaa");
	int* mynextarray15 = new int[mys15sub.length];	 //next数组
	int* mynextvalarray15 = new int[mys15sub.length]; //nextval数组
	cout << "本次采用典型的KMP算法求解next和nextval数组然后利用nextval数组进行模式串匹配查找：----" << endl;
	mys15sub.getNextAndNextValArray_Classic(mynextarray15, mynextvalarray15); //求next数组和nextval数组
	_nmsp1::MySString mys15master; //主串
	mys15master.StrAssign("ababaaabababa"); 	
	cout << "StrKMPIndex()结果为" << mys15master.StrKMPIndex(mys15sub, mynextvalarray15) << endl;
	delete[]mynextarray15;
	delete[]mynextvalarray15;

	/*
	//串赋值
	_nmsp1::MyHString mys;
	mys.StrAssign("我爱你中国！");
	mys.DispContent();

	//串拷贝（串复制）、判断空串
	_nmsp1::MyHString mys2;
	cout << "mys2为空吗？" << mys2.IfStrEmpty() << endl;
	mys2.StrCopy(mys);
	mys2.DispContent();
	cout << "mys2为空吗？" << mys2.IfStrEmpty() << endl;
	mys.StrAssign("我爱你中国，我爱你中国人！");
	mys2.StrCopy(mys);
	mys2.DispContent();

	//串比较
	_nmsp1::MyHString mys3, mys4;
	mys3.StrAssign("abc");
	mys4.StrAssign("xyz");
	cout << "mys3和mys4字符串的比较结果为：" << mys3.StrCmp(mys4) << endl;

	//串连接
	_nmsp1::MyHString mys5;
	mys5.StrAssign("Hello China!");
	_nmsp1::MyHString mys6;
	mys6.StrAssign("Hello this World!");
	mys6.StrCat(mys5);
	cout << "mys6和mys5连接的结果为：";
	mys6.DispContent();

	//获取串的一部分内容（求子串）
	_nmsp1::MyHString mys7;
	mys7.StrAssign("abcdefghijklmnopqrstuvwxyz!");
	mys6.SubString(mys7, 2, 12); //子串放入mys7中
	cout << "子串mys7的内容是：";
	mys7.DispContent();

	//串插入（在当前串的pos位置（从0开始计算），插入子串substr）
	_nmsp1::MyHString mys8, mys82;
	mys8.StrAssign("我爱北京，我爱中国！");
	mys82.StrInsert(0, mys8);
	cout << "插入新内容后的mys82串内容是：";
	mys82.DispContent();


	//串删除，在当前串的pos位置（从0开始计算），删除len个字符
	_nmsp1::MyHString mys9;
	mys9.StrAssign("Hello China!");
	mys9.StrDelete(1, 11);
	cout << "删除部分内容后的mys9串内容是：";
	mys9.DispContent();

	//串清空
	mys9.StrClear();
	cout << "清空内容后的mys9串内容是：";
	mys9.DispContent();
	*/
	

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




