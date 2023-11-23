// MyProject.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。


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
	void StrCopy(const MySString& tmpstr)
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
	//结果大于返回1，等于返回0，小于返回-1
	int StrCmp(const MySString& tmpstr)
	{
		if (length == 0 && tmpstr.length == 0) //两个字符串都是空的，相等
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
		if (pos < 0 || (pos + 1) > length || len <= 0)//pos位置要合法，len长度值要合法
			return;

		int icount = 0;
		while (true)
		{
			resultstr.ch[icount] = ch[pos + icount];
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


//采用堆中分配内存的存储结构
class MyHString//H表示Heap（堆）
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

		if (i >= length && j >= tmpstr.length)//长度相同且内容相等
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

		char* tmp = new char[length + tmpstr.length];
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
	char* ch;  //空间用new动态分配
	size_t  length;  //串实际长度
};


////字符节点（串的链式存储）
//struct StringNode
//{
//	char ch;
//	StringNode* next;  //指针域，指向下一个同类型（和本节点类型相同）节点
//};
//字符节点（串的链式存储）
struct StringNode
{
	char ch[4];
	StringNode* next;  //指针域，指向下一个同类型（和本节点类型相同）节点
};

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);//程序退出时检测内存泄漏并显示到“输出”窗口


	////串赋值
	//MySString mys;
	//mys.StrAssign("我爱你中国！");
	//mys.DispContent();

	////串拷贝（串复制）、判断空串
	//MySString mys2;
	//cout << "mys2为空吗？" << mys2.IfStrEmpty() << endl;
	//mys2.StrCopy(mys);
	//mys2.DispContent();
	//cout << "mys2为空吗？" << mys2.IfStrEmpty() << endl;

	////串比较
	//MySString mys3, mys4;
	//mys3.StrAssign("abc");
	//mys4.StrAssign("xyz");
	//cout << "mys3和mys4字符串的比较结果为：" << mys3.StrCmp(mys4) << endl;

	////串连接
	//MySString mys5;
	//mys5.StrAssign("Hello China!");
	//MySString mys6;
	//mys6.StrAssign("Hello this World!");
	//mys6.StrCat(mys5);
	//cout << "mys6和mys5连接的结果为：" << mys6.ch << endl;

	////获取串的一部分内容（求子串）
	//MySString mys7;
	//mys6.SubString(mys7, 0, 12); //子串放入mys7中
	//cout << "子串mys7的内容是：" << mys7.ch << endl;

	////串插入（在当前串的pos位置（从0开始计算），插入子串substr）
	//MySString mys8;
	//mys8.StrAssign("我爱北京，我爱中国！");
	//mys5.StrInsert(12, mys8);
	//cout << "插入新内容后的mys5串内容是：" << mys5.ch << endl;

	////串删除，在当前串的pos位置（从0开始计算），删除len个字符
	//MySString mys9;
	//mys9.StrAssign("Hello China!");
	//mys9.StrDelete(1, 10);
	//cout << "删除部分内容后的mys9串内容是：" << mys9.ch << endl;

	////串清空
	//mys9.StrClear();
	//cout << "清空内容后的mys9串内容是：" << mys9.ch << endl;

	//串赋值
	MyHString mys;
	mys.StrAssign("我爱你中国！");
	mys.DispContent();

	//串拷贝（串复制）、判断空串
	MyHString mys2;
	cout << "mys2为空吗？" << mys2.IfStrEmpty() << endl;
	mys2.StrCopy(mys);
	mys2.DispContent();
	cout << "mys2为空吗？" << mys2.IfStrEmpty() << endl;
	mys.StrAssign("我爱你中国，我爱你中国人！");
	mys2.StrCopy(mys);
	mys2.DispContent();

	//串比较
	MyHString mys3, mys4;
	mys3.StrAssign("abc");
	mys4.StrAssign("xyz");
	cout << "mys3和mys4字符串的比较结果为：" << mys3.StrCmp(mys4) << endl;

	//串连接
	MyHString mys5;
	mys5.StrAssign("Hello China!");
	MyHString mys6;
	mys6.StrAssign("Hello this World!");
	mys6.StrCat(mys5);
	cout << "mys6和mys5连接的结果为：";
	mys6.DispContent();

	//获取串的一部分内容（求子串）
	MyHString mys7;
	mys7.StrAssign("abcdefghijklmnopqrstuvwxyz!");
	mys6.SubString(mys7, 2, 12); //子串放入mys7中
	cout << "子串mys7的内容是：";
	mys7.DispContent();

	//串插入（在当前串的pos位置（从0开始计算），插入子串substr）
	MyHString mys8, mys82;
	mys8.StrAssign("我爱北京，我爱中国！");
	mys82.StrInsert(0, mys8);
	cout << "插入新内容后的mys82串内容是：";
	mys82.DispContent();

	//串删除，在当前串的pos位置（从0开始计算），删除len个字符
	MyHString mys9;
	mys9.StrAssign("Hello China!");
	mys9.StrDelete(1, 11);
	cout << "删除部分内容后的mys9串内容是：";
	mys9.DispContent();

	//串清空
	mys9.StrClear();
	cout << "清空内容后的mys9串内容是：";
	mys9.DispContent();

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

















//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);//程序退出时检测内存泄漏并显示到“输出”窗口