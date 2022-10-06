#include<iostream>
#include<cstdlib>
using namespace std;

#define MAXSIZE 20000

template<class type>
class my_stack
{
	int top;
	type* op;
	int maxsize;

public:
	my_stack() :top(-1), maxsize(MAXSIZE)
	{
		op = new type[maxsize];
	}
	my_stack(int size) :top(-1), maxsize(size)
	{
		op = new type[maxsize];
	}
	//�Ƿ�Ϊ��
	bool Empty();
	//ѹջ
	void Push(type tp);
	//����ջ��Ԫ��
	type Top();
	//��ջ
	void Pop();
	//ջ��С
	int Size();
	//��ӡջ
	void List();
};

template<class type>
bool my_stack<type>::Empty()
{
	if (top == -1)
	{
		return true;
	}
	else
		return false;
}

template<class type>
type my_stack<type>::Top()
{
	if (top != -1)
	{
		return op[top];
	}
}

template<class type>
void my_stack<type>::Push(type tp)
{
	if (top + 1 < maxsize)
	{
		top++;
		op[top] = tp;
		cout << "ѹ��" << op[top] << endl;
	}
}

template<class type>
void my_stack<type>::Pop()
{
	if (top >= 0)
	{
		cout << "����" << op[top] << endl;
		top--;
	}
}

template<class type>
int my_stack<type>::Size()
{
	return top + 1;
}
template<class type>
void my_stack<type>::List()
{
	cout << "��ǰջ״̬                  " << endl;
	for (int i = top; i > -1; i--)
	{
		cout << op[i] << "   ";
	}
	cout << endl << "top->-------------------" << endl;
}