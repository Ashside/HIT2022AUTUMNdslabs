#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <cstring>
#include <bitset>
#include <queue>

using namespace std;
//�����ַ�����
double total_chars = 0;
//������������ڵ�
class huffnode {
public:
	double weight{ 0 };
	huffnode* lc{ nullptr };
	huffnode* rc{ nullptr };
	huffnode* mc{ nullptr };
	huffnode* pa{ nullptr };
	char ch{ '#' };
};
//�������������
class huffcode {
public:
	char* code{};
	char ch{ '#' };
};

struct Cmp {
	bool operator()(huffnode* A, huffnode* B) {
		return A->weight > B->weight;
	}
};

//��ȡ�ļ���ͳ���ַ�Ƶ��
map<char, double> readFile(vector<double>& weight) {
	ifstream in("input.txt", ios::in);
	string str{};
	map<char, double> words;
	while (getline(in, str))
	{
		for (char& i : str)
		{
			if (words.end() == words.find(i))
				words.insert(make_pair(i, 1));
			else
				++words[i];
		}
	}
	for (const auto& i : words) {
		total_chars += i.second;
	}
	for (const auto& i : words) {
		weight.push_back(i.second / total_chars);
	}
	return words;
}
//��ʼ����������
void initHT(huffnode* T, const map<char, double>& words) {
	int i = 0;
	for (const auto& j : words) {
		T[i].weight = j.second / total_chars;
		T[i].ch = j.first;
		++i;
	}
}

//������������
huffnode* creatHT(huffnode* T, int n) {
	if (n <= 3) {
		T[4].lc = &T[0];
		T[4].rc = &T[1];
		T[4].mc = &T[2];
		T[4].weight = T[0].weight + T[1].weight + T[2].weight;
		T[0].pa = &T[3];
		T[1].pa = &T[3];
		T[2].pa = &T[3];
		return &T[4];
	}
	priority_queue<huffnode*, vector<huffnode*>, Cmp> q;//���ȶ��У������滻�ѣ�ԭ����ͬ

	for (int i = 0; i < n; ++i) {
		q.push(&T[i]);//������Ҷ�ӽڵ����
	}
	int i = n;
	while (q.size() != 1 ) {//
		huffnode* s1 = q.top();
		q.pop();
		huffnode* s2 = q.top();
		q.pop();
		huffnode* s3 = q.top();
		q.pop();
		T[i].weight = s1->weight + s2->weight + s3->weight;
		T[i].lc = s1;
		T[i].mc = s2;
		T[i].rc = s3;
		s1->pa = &T[i];
		s2->pa = &T[i];
		s3->pa = &T[i];
		q.push(&T[i]);
		i++;
	}
	i--;
	return &T[i];
}
//����������
void huffEncode(huffnode* T, huffcode* HC, int n) {
	huffnode* father, * current;
	int i;
	char* cd = new char[n] {};
	int start;
	for (i = 0; i < n; i++) {
		start = n - 1;
		cd[start] = '\0';
		current = &T[i];
		father = current->pa;
		while (father != nullptr && start >= 0) {
			start--;
			if (father->lc == current) {
				cd[start] = '0';
			}
			else if (father->mc == current) {
				cd[start] = '1';
			}
			else if (father->rc == current) {
				cd[start] = '2';
			}

			current = father;
			father = current->pa;
		}
		HC[i].code = new char[n - start];
		memcpy_s(HC[i].code, (n - start) * sizeof(char), &cd[start], (n - start) * sizeof(char));
		HC[i].ch = T[i].ch;
	}
}
//��ӡ����������
void printCode(huffcode* HC, int n) {
	cout << "�ַ�\t����" << endl;
	for (int i = 0; i < n; i++) {
		cout << HC[i].ch;
		cout << "\t"; puts(HC[i].code); cout << endl;
	}
}
//ѹ��
void compress(huffcode* HC, int n) {
	ifstream in("input.txt", ios::in);
	ofstream out("output.txt", ios::out);
	string str{};
	string codestream{};
	while (getline(in, str))
	{
		for (char& i : str)
		{
			for (int j = 0; j < n; ++j) {
				if (i == HC[j].ch) {
					codestream += HC[j].code;
					break;
				}
			}
		}
	}
	out << codestream;
	cout << codestream;
	in.close();
	out.close();
	cout << endl << "ѹ���ɹ���" << endl;
}
//��ѹ
void decode(huffnode* T, int n,huffnode* root) {
	ifstream in("output.txt", ios::in);
	ofstream out("decode.txt", ios::out);
	string codestream{}, temp{};
	char ch{};
	while (getline(in, temp)) {
		codestream += temp;
	}
	huffnode* p = root;
	for (char& i : codestream) {
		if (i == '0') {
			p = p->lc;
		}
		else if (i == '1') {
			p = p->mc;
		}
		else if(i=='2') {
			p = p->rc;
		}
		if (p->lc==nullptr&&p->mc==nullptr&&p->rc==nullptr) {
			cout << p->ch;
			out << p->ch;
			p = root;
		}
	}
	in.close();
	cout << endl << endl;
}
//�����ļ�ѹ����
void compressRate() {
	ifstream in("input.txt", ios::in);
	ifstream out("output.txt", ios::in);
	in.seekg(0, ios::end);
	out.seekg(0, ios::end);
	cout << "ѹ����Ϊ��    " << out.tellg() * 1.0 / in.tellg() / 8 << endl;

	in.close();
	out.close();
}
//��ԭ�ıȽ�
void compare() {
	ifstream in("input.txt", ios::in);
	ifstream out("decode.txt", ios::in);
	string str1{};
	string str2{};
	string temp{};
	while (!in.eof()) {//
		getline(in, temp);//��ȡһ��
		str1 += temp;//����ȡ��һ�м��뵽str1��
	}
	while (!out.eof()) {
		getline(out, str2);//������Ե�ʱ���֣�decode.txt�ļ�û�л��У��������ﲻ��temp
	}
	if (str1 == str2)cout << "������ȷ" << endl;
	else {
		cout << "�������" << endl;
		cout << str1 << endl;
		cout << str2 << endl;
	}
	in.close();
	out.close();
}

int main() {
	vector<double>weight;//Ȩֵ
	map<char, double>words;//�洢�ַ���Ȩֵ
	words = readFile(weight);//��ȡ�ļ���ͳ���ַ����ִ���
	//printFrequency(words);//��ӡƵ��
	int types = words.size();//�ַ�����
	auto* T = new huffnode[2 * types - 1];
	initHT(T, words);//��ʼ����������
	huffnode* root{};
	root=creatHT(T, types);//������������
	auto* HC = new huffcode[types]; cout << "����ɹ�" << endl;
	huffEncode(T, HC, types);//����������
	printCode(HC, types);//��ӡ����
	compress(HC, types);//ѹ��
	decode(T, types,root);//��ѹ
	compressRate();//����ѹ����
	compare();//��ԭ�ıȽ�
}