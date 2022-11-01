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
	huffnode* pa{ nullptr };
	char ch{ '#' };
	bool operator>(const huffnode& a) const {
		return weight > a.weight;
	}
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
void creatHT(huffnode* T, int n) {
	if (n <= 2) {
		T[3].lc = &T[0];
		T[3].rc = &T[1];
		T[3].weight = T[0].weight + T[1].weight;
		T[0].pa = &T[3];
		T[1].pa = &T[3];
		return;
	}
	int m = 2 * n - 1;
	priority_queue<huffnode*, vector<huffnode*>, Cmp> q;//���ȶ��У������滻�ѣ�ԭ����ͬ

	for (int i = 0; i < n; ++i) {
		q.push(&T[i]);//������Ҷ�ӽڵ����
	}
	int i = n;
	while (q.size() != 1 && i < 2 * n - 1) {//
		huffnode* s1 = q.top();
		q.pop();
		huffnode* s2 = q.top();
		q.pop();
		T[i].weight = s1->weight + s2->weight;
		T[i].lc = s1;
		T[i].rc = s2;
		s1->pa = &T[i];
		s2->pa = &T[i];
		q.push(&T[i]);
		i++;
	}
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

			else if (father->rc == current) {
				cd[start] = '1';
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
	ofstream out("output.dat", ios::out | ios::binary);
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
	while (codestream.size() >= 8)   //01��ת��Ϊ�ַ������
	{
		bitset<8> bits(codestream, 0, 8);
		codestream.erase(0, 8);
		out << static_cast<char>(bits.to_ulong());
	}
	if (!codestream.empty())   //��β���Ȳ�����λ����0���룬��¼ʣ��ĳ��Ƚ�������ļ�ĩβ���ڶ�ȡʱת��Ϊ�±�λ�û�ԭ�������ַ���
	{
		unsigned long loc = codestream.size();
		codestream.append(8 - loc, '0');
		bitset<8> bits1(codestream, 0, 8);

		out << static_cast<char>(bits1.to_ulong());
		out << static_cast<char>(8 - loc);
	}
	else {//��β���ȸպ�Ϊ��λ����0���룬��¼ʣ��ĳ��Ƚ�������ļ�ĩβ���ڶ�ȡʱת��Ϊ�±�λ�û�ԭ�������ַ���
		codestream.append(8, '0');
		bitset<8> bits(codestream, 0, 8);
		out << static_cast<char>(bits.to_ulong());
	}
	in.close();
	out.close();
	cout << "ѹ���ɹ���" << endl;
}
//��ѹ
void decode(huffnode* T, int n) {
	ifstream in("output.dat", ios::in | ios::binary);
	ofstream out("decode.txt", ios::out);
	string codestream{};
	char ch{};
	int root = 2 * n - 2;
	while (true) {
		if (in.peek() == EOF) {
			break;
		}
		in.read(&ch, 1);
		bitset<8> bits(ch);
		codestream += bits.to_string();
	}
	string side = codestream.substr(codestream.size() - 8, 8);
	bitset<8> bits(side);
	unsigned long loc = bits.to_ulong();
	codestream.erase(codestream.size() - 8, 8);
	if (loc != 0) {
		codestream.erase(codestream.size() - loc, loc);
	}
	cout << "��ѹ�ļ�Ϊ: " << endl << endl;
	huffnode* p = &T[root];
	for (char& i : codestream) {
		if (i == '0') {
			p = p->lc;
		}
		else {
			p = p->rc;
		}
		if (p->lc == nullptr && p->rc == nullptr) {
			cout << p->ch;
			out << p->ch;
			p = &T[root];
		}
	}
	cout << endl << endl;
}
//�����ļ�ѹ����
void compressRate() {
	ifstream in("input.txt", ios::in);
	ifstream out("output.dat", ios::in | ios::binary);
	in.seekg(0, ios::end);
	out.seekg(0, ios::end);
	cout << "ѹ����Ϊ��    " << out.tellg() * 1.0 / in.tellg() << endl;

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
	creatHT(T, types);//������������
	auto* HC = new huffcode[types]; cout << "����ɹ�" << endl;
	huffEncode(T, HC, types);//����������
	printCode(HC, types);//��ӡ����
	compress(HC, types);//ѹ��
	decode(T, types);//��ѹ
	compressRate();//����ѹ����
	compare();//��ԭ�ıȽ�
}