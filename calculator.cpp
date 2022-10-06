#include<iostream>
#include<string>
#include <fstream>
#include <sstream>
#include"my_stack.cpp"
using namespace std;
// 定义变量类
class vars
{
public:
	char name[20]{};
	double value[20]{};
	int size = 0;
};
// 从文件中读入字符串
string readFile(const char* filename)
{
	ifstream myfile(filename);
	ostringstream buf;
	char ch;
	while (buf && myfile.get(ch))
	{
		buf.put(ch);
	}
	return buf.str();
}
// 运算符优先级
int optPrio(char opt)
{
	int prio{};
	if (opt == '^')
	{
		prio = 3;
	}
	if (opt == '*' || opt == '/')
	{
		prio = 2;
	}

	if (opt == '+' || opt == '-')
	{
		prio = 1;
	}

	if (opt == '(')
	{
		prio = 0;
	}

	return prio;
}
// 判断是否为数字，变量也认为是数字
bool isNum(char ch)
{
	if ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
	{
		return true;
	}
	else return false;
}
// 判断是否为操作符
bool isOpt(char ch)
{
	if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^')
	{
		return true;
	}
	else return false;
}
// 判断是否为变量（仅支持单字变量）
bool isVar(char ch)
{
	if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
	{
		return true;
	}
	return false;
}
// 判断是否输入#
bool isWrite(string& infix)
{
	char infixStart = infix[0];
	char infixEnd = infix.back();
	if (infixStart != '#' || infixEnd != '#')
	{
		cout << "错误输入" << endl;
		cout << "需要 #" << endl;
		return false;
	}
	return true;
}
// 计算两个数字
double calChar(double num1, double num2, char opt)
{
	switch (opt)
	{
	case '+':
		return num1 + num2;
	case '-':
		return num1 - num2;
	case '*':
		return num1 * num2;
	case '/':
		return num1 / num2;
	case '^':
		return pow(num1, num2);
	}
}
// 格式化中缀表达式,处理负数，传回格式化的中缀表达式
string infixFormat(string& infix)//这里传进来的是去掉#的表达式
{
	for (int i = 0; i < infix.length(); i++)
	{
		if (infix[i] == '-' && i == 0)//如果负数在第一个
		{
			int cnt;
			string sub = infix.substr(1, infix.length());
			cnt = sub.find_first_of("+-*/^");
			infix.insert(cnt + 1, ")");//在n+1位置插入右括号
			infix.insert(i, "0");//在0处插入0
			infix.insert(i, "(");//在0处插入（
			//把所有负数都做成0-x的形式
			//最开始的负数会被做成（0-x）防止乘除产生歧义
			i = cnt + 1;
		}
		if (infix[i] == '-' && infix[i - 1] == '(' && i != 0)
		{
			infix.insert(i, "0");//把所有负数都做成0-x的形式
			i++;
		}
	}
	string infix_format{};
	for (int i = 0; i < infix.length(); i++)
	{
		infix_format += infix[i];
	}
	return infix_format;
}
// 中缀转后缀，传回存放变量操作数的类
vars Infix2Postfix(string& infix, string& postfix)     //引用传递
{
	my_stack<char> opt;                   //定义一个char类型的栈opt
	int i = 0;
	int cnt = 0;
	vars var;
	for (i = 0; i < infix.size(); i++)
	{
		if (isVar(infix[i]))
		{
			var.name[cnt] = infix[i];
			cnt++;
		}
	}
	var.size = cnt;
	for (i = 0; i < infix.size(); i++)
	{
		if (isNum(infix[i]) || infix[i] == '.')//如果是数字，直接写入postfix
		{
			postfix += infix[i];

			if (isOpt(infix[i + 1]) || infix[i + 1] == ')' || i == infix.size() - 1)//如果下一个是操作符、右括号、结束，就添加一个空白分隔符
			{
				postfix += ' ';
			}
		}
		else
		{
			//原则：栈空则入,优先级大于栈顶则入, 优先级小于栈顶则出栈
			if (opt.Empty())
			{
				opt.Push(infix[i]);
			}

			else if (infix[i] == '(')
			{
				opt.Push(infix[i]);
			}//左括号入

			else if (infix[i] == ')')    //如果是右括号，只要栈顶不是左括号，就弹出并输出
			{						     //说明前面有一个高优先级表达式（）该要计算了
				while (opt.Top() != '(') //只要栈顶不是左括号，就弹出并输出，写入后缀表达式，先计算
				{
					postfix += opt.Top();//符号写入表达式
					postfix += ' ';      //写入空格分隔符

					opt.Pop();           //将写入的符号弹出
				}
				opt.Pop();                 //弹出左括号不输出
			}
			else if (isOpt(infix[i]))
			{
				while (optPrio(infix[i]) <= optPrio(opt.Top()))   //栈顶优先级大于等于当前运算符,输出
				{
					postfix += opt.Top();                         //先算，写在前边
					postfix += ' ';

					opt.Pop();
					if (opt.Empty())      //栈为空，停止
						break;
				}
				opt.Push(infix[i]);   //把当前运算符入栈，后算
			}
		}
	}
	while (!opt.Empty())        //弹出剩余符号，剩余一定是后算
	{
		postfix += opt.Top();
		postfix += ' ';

		opt.Pop();
	}
	return var;
}
// 计算后缀表达式
double calPost(string& postfix, vars var)
{
	my_stack<double> opn;
	double num1, num2, res, topush, topop;
	int i = 0;
	cout << endl << "操作数压栈操作: " << endl;
	for (i = 0; i < postfix.length(); i++)
	{
		if (isNum(postfix[i]))
		{
			if (isVar(postfix[i]))//判断是否为变量操作数
			{
				for (int j = 0; j < var.size; j++)
				{
					if (postfix[i] == var.name[j])
					{
						opn.Push(var.value[j]);//将原本要压入的字符换为对应的value
					}
				}
				continue;
			}
			double temp = 0;
			int cnt = 0;//计数小数点后位数
			while (isNum(postfix[i]))
			{
				temp = temp * 10 + (postfix[i] - '0');//正常读入
				i++;
			}
			if (postfix[i] == '.')//记录小数点
			{
				i++;
				while (isNum(postfix[i]))
				{
					temp = temp * 10 + (postfix[i] - '0');//还是正常读入
					i++;
					cnt++;
				}
			}
			while (cnt)//将小数点从最右端向左移动
			{
				temp /= 10;
				cnt--;
			}
			opn.Push(temp);
		}
		else if (postfix[i] == ' ')
		{
			continue;//空格分隔符跳过
		}
		else if (isOpt(postfix[i]))
		{
			num2 = opn.Top();//取数
			opn.Pop();
			num1 = opn.Top();//取数
			opn.Pop();
			cout << "取数计算" << endl;
			cout << "num1: " << num1 << "  ";
			cout << "num2: " << num2 << "  method: " << postfix[i];
			topush = calChar(num1, num2, postfix[i]);
			cout << "  topush: " << topush << endl;
			opn.Push(topush);//计算结果重新压进栈中
			opn.List();//打印栈
		}
	}

	res = opn.Top();//最后剩下的数就是结果
	return res;
}
int main()
{
	string infix;//中缀，保证正确
	string postfix;//后缀
	vars var;//存放单字变量
	bool input_flag;//判断是否有#，不必要
	double res;//存放后缀表达式结果
	infix = readFile("input.txt");//读入
	cout << "输入表达式为：" << infix << endl;
	input_flag = isWrite(infix);
	if (input_flag == false) return 0;//判断输入
	string infix_ed = infix.substr(1, infix.size() - 2);//除去前后两个#
	infix_ed = infixFormat(infix_ed);//格式化中缀
	cout << "中缀表达式为：" << infix_ed << endl;
	var = Infix2Postfix(infix_ed, postfix);//中缀转后缀
	if (var.size != 0)
	{
		cout << endl << "变量为: ";
		for (int i = 0; i < var.size; i++)
		{
			cout << var.name[i] << ' ';
		}
		cout << endl << "赋值为: ";
		for (int i = 0; i < var.size; i++)
		{
			cin >> var.value[i];
		}
	}
	cout << endl << "后缀表达式为：" << postfix << endl;
	res = calPost(postfix, var);
	cout << "计算结果为：" << res << endl;//计算后缀表达式
}