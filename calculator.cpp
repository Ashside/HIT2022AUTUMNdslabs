#include<iostream>
#include<string>
#include <fstream>
#include <sstream>
#include"my_stack.cpp"
using namespace std;
// ���������
class vars
{
public:
	char name[20]{};
	double value[20]{};
	int size = 0;
};
// ���ļ��ж����ַ���
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
// ��������ȼ�
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
// �ж��Ƿ�Ϊ���֣�����Ҳ��Ϊ������
bool isNum(char ch)
{
	if ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
	{
		return true;
	}
	else return false;
}
// �ж��Ƿ�Ϊ������
bool isOpt(char ch)
{
	if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^')
	{
		return true;
	}
	else return false;
}
// �ж��Ƿ�Ϊ��������֧�ֵ��ֱ�����
bool isVar(char ch)
{
	if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
	{
		return true;
	}
	return false;
}
// �ж��Ƿ�����#
bool isWrite(string& infix)
{
	char infixStart = infix[0];
	char infixEnd = infix.back();
	if (infixStart != '#' || infixEnd != '#')
	{
		cout << "��������" << endl;
		cout << "��Ҫ #" << endl;
		return false;
	}
	return true;
}
// ������������
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
// ��ʽ����׺���ʽ,�����������ظ�ʽ������׺���ʽ
string infixFormat(string& infix)//���ﴫ��������ȥ��#�ı��ʽ
{
	for (int i = 0; i < infix.length(); i++)
	{
		if (infix[i] == '-' && i == 0)//��������ڵ�һ��
		{
			int cnt;
			string sub = infix.substr(1, infix.length());
			cnt = sub.find_first_of("+-*/^");
			infix.insert(cnt + 1, ")");//��n+1λ�ò���������
			infix.insert(i, "0");//��0������0
			infix.insert(i, "(");//��0�����루
			//�����и���������0-x����ʽ
			//�ʼ�ĸ����ᱻ���ɣ�0-x����ֹ�˳���������
			i = cnt + 1;
		}
		if (infix[i] == '-' && infix[i - 1] == '(' && i != 0)
		{
			infix.insert(i, "0");//�����и���������0-x����ʽ
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
// ��׺ת��׺�����ش�ű�������������
vars Infix2Postfix(string& infix, string& postfix)     //���ô���
{
	my_stack<char> opt;                   //����һ��char���͵�ջopt
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
		if (isNum(infix[i]) || infix[i] == '.')//��������֣�ֱ��д��postfix
		{
			postfix += infix[i];

			if (isOpt(infix[i + 1]) || infix[i + 1] == ')' || i == infix.size() - 1)//�����һ���ǲ������������š������������һ���հ׷ָ���
			{
				postfix += ' ';
			}
		}
		else
		{
			//ԭ��ջ������,���ȼ�����ջ������, ���ȼ�С��ջ�����ջ
			if (opt.Empty())
			{
				opt.Push(infix[i]);
			}

			else if (infix[i] == '(')
			{
				opt.Push(infix[i]);
			}//��������

			else if (infix[i] == ')')    //����������ţ�ֻҪջ�����������ţ��͵��������
			{						     //˵��ǰ����һ�������ȼ����ʽ������Ҫ������
				while (opt.Top() != '(') //ֻҪջ�����������ţ��͵����������д���׺���ʽ���ȼ���
				{
					postfix += opt.Top();//����д����ʽ
					postfix += ' ';      //д��ո�ָ���

					opt.Pop();           //��д��ķ��ŵ���
				}
				opt.Pop();                 //���������Ų����
			}
			else if (isOpt(infix[i]))
			{
				while (optPrio(infix[i]) <= optPrio(opt.Top()))   //ջ�����ȼ����ڵ��ڵ�ǰ�����,���
				{
					postfix += opt.Top();                         //���㣬д��ǰ��
					postfix += ' ';

					opt.Pop();
					if (opt.Empty())      //ջΪ�գ�ֹͣ
						break;
				}
				opt.Push(infix[i]);   //�ѵ�ǰ�������ջ������
			}
		}
	}
	while (!opt.Empty())        //����ʣ����ţ�ʣ��һ���Ǻ���
	{
		postfix += opt.Top();
		postfix += ' ';

		opt.Pop();
	}
	return var;
}
// �����׺���ʽ
double calPost(string& postfix, vars var)
{
	my_stack<double> opn;
	double num1, num2, res, topush, topop;
	int i = 0;
	cout << endl << "������ѹջ����: " << endl;
	for (i = 0; i < postfix.length(); i++)
	{
		if (isNum(postfix[i]))
		{
			if (isVar(postfix[i]))//�ж��Ƿ�Ϊ����������
			{
				for (int j = 0; j < var.size; j++)
				{
					if (postfix[i] == var.name[j])
					{
						opn.Push(var.value[j]);//��ԭ��Ҫѹ����ַ���Ϊ��Ӧ��value
					}
				}
				continue;
			}
			double temp = 0;
			int cnt = 0;//����С�����λ��
			while (isNum(postfix[i]))
			{
				temp = temp * 10 + (postfix[i] - '0');//��������
				i++;
			}
			if (postfix[i] == '.')//��¼С����
			{
				i++;
				while (isNum(postfix[i]))
				{
					temp = temp * 10 + (postfix[i] - '0');//������������
					i++;
					cnt++;
				}
			}
			while (cnt)//��С��������Ҷ������ƶ�
			{
				temp /= 10;
				cnt--;
			}
			opn.Push(temp);
		}
		else if (postfix[i] == ' ')
		{
			continue;//�ո�ָ�������
		}
		else if (isOpt(postfix[i]))
		{
			num2 = opn.Top();//ȡ��
			opn.Pop();
			num1 = opn.Top();//ȡ��
			opn.Pop();
			cout << "ȡ������" << endl;
			cout << "num1: " << num1 << "  ";
			cout << "num2: " << num2 << "  method: " << postfix[i];
			topush = calChar(num1, num2, postfix[i]);
			cout << "  topush: " << topush << endl;
			opn.Push(topush);//����������ѹ��ջ��
			opn.List();//��ӡջ
		}
	}

	res = opn.Top();//���ʣ�µ������ǽ��
	return res;
}
int main()
{
	string infix;//��׺����֤��ȷ
	string postfix;//��׺
	vars var;//��ŵ��ֱ���
	bool input_flag;//�ж��Ƿ���#������Ҫ
	double res;//��ź�׺���ʽ���
	infix = readFile("input.txt");//����
	cout << "������ʽΪ��" << infix << endl;
	input_flag = isWrite(infix);
	if (input_flag == false) return 0;//�ж�����
	string infix_ed = infix.substr(1, infix.size() - 2);//��ȥǰ������#
	infix_ed = infixFormat(infix_ed);//��ʽ����׺
	cout << "��׺���ʽΪ��" << infix_ed << endl;
	var = Infix2Postfix(infix_ed, postfix);//��׺ת��׺
	if (var.size != 0)
	{
		cout << endl << "����Ϊ: ";
		for (int i = 0; i < var.size; i++)
		{
			cout << var.name[i] << ' ';
		}
		cout << endl << "��ֵΪ: ";
		for (int i = 0; i < var.size; i++)
		{
			cin >> var.value[i];
		}
	}
	cout << endl << "��׺���ʽΪ��" << postfix << endl;
	res = calPost(postfix, var);
	cout << "������Ϊ��" << res << endl;//�����׺���ʽ
}