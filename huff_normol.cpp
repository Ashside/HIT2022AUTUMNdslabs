#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <cstring>
#include <bitset>
using namespace std;
//�����ַ�����
double total_words = 0;
//������������ڵ�
class huffnode {
public:
	double weight{ 0 };
	int lc{ -1 };
	int rc{ -1 };
	int pa{ -1 };
	char ch{ '#' };
};
//�������������
class huffcode {
public:
	char* code{};
	char ch{ '#' };
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
		total_words += i.second;
	}
	for (const auto& i : words) {
		weight.push_back(i.second / total_words);
	}
	return words;
}
//��ӡ�ַ�Ƶ��
void printFrequency(const map<char, double>& words) {
	cout << "�ַ�\t����Ƶ��" << endl;
	for (const auto& i : words)
	{
		cout << i.first << '\t' << i.second / total_words << endl;
	}
}
//��ʼ����������
void initHT(huffnode* T, const map<char, double>& words, int n) {
	int i = 0;
	for (const auto& j : words) {
		T[i].weight = j.second / total_words;
		T[i].ch = j.first;
		++i;
	}
}
//ѡ��Ȩֵ��С�������ڵ�
void select(huffnode* T, int n, int& p1, int& p2) {
	double min1 = 1, min2 = 1;
	for (int i = 0; i <= n; i++)
	{
		if (T[i].pa == -1 && min1 > T[i].weight)
		{
			min1 = T[i].weight;
			p1 = i;
		}
	}
	//��˫�׽ڵ�Ϊ0��Ϊ�����ڶ�С��Ԫ�أ������±괫��s1
	for (int i = 0; i <= n; i++)
	{
		if (i != p1 && T[i].pa == -1)
		{
			if (T[i].weight < min2)
			{
				min2 = T[i].weight;
				p2 = i;
			}
		}
	}
}
//������������
void creatHT(huffnode* T, int n) {
	int i;
	int p1, p2;

	int m = 2 * n - 1;
	for (i = n; i < m; i++) {
		select(T, i - 1, p1, p2);
		T[p1].pa = i;
		T[p2].pa = i;
		T[i].lc = p1;
		T[i].rc = p2;
		T[i].weight = T[p1].weight + T[p2].weight;
	}
}
//����������
void huffEncode(huffnode* T, huffcode* HC, int n) {
	int i, c, p;
	char* cd = new char[n];
	int start;
	for (i = 0; i < n; i++) {
		start = n - 1;
		cd[start] = '\0';
		c = i;
		p = T[i].pa;
		while (p != -1) {
			--start;
			if (T[p].lc == c)
				cd[start] = '0';
			else if (T[p].rc == c)
				cd[start] = '1';
			c = p;
			p = T[p].pa;
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
	cout << "ѹ���ɹ���" << endl;
	cout << codestream << endl;
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
	int p = root;
	for (char& i : codestream) {
		if (i == '0') {
			p = T[p].lc;
		}
		else {
			p = T[p].rc;
		}
		if (T[p].lc == -1 && T[p].rc == -1) {
			cout << T[p].ch;
			out << T[p].ch;
			p = root;
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
//������
int main() {
	vector<double>weight;//Ȩֵ
	map<char, double>words;//�洢�ַ���Ȩֵ
	words = readFile(weight);//��ȡ�ļ���ͳ���ַ����ִ���
	//printFrequency(words);//��ӡƵ��
	int types = words.size();//�ַ�����
	auto* T = new huffnode[2 * types - 1];
	initHT(T, words, types);//��ʼ����������
	creatHT(T, types);//������������
	auto* HC = new huffcode[types];
	huffEncode(T, HC, types);//����������
	printCode(HC, types);//��ӡ����
	compress(HC, types);//ѹ��
	decode(T, types);//��ѹ
	compressRate();//����ѹ����
	compare();//��ԭ�ıȽ�
}