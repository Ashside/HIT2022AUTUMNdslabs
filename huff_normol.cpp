#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <cstring>
#include <bitset>
using namespace std;
//所有字符总数
double total_words = 0;
//定义哈夫曼树节点
class huffnode {
public:
	double weight{ 0 };
	int lc{ -1 };
	int rc{ -1 };
	int pa{ -1 };
	char ch{ '#' };
};
//定义哈夫曼编码
class huffcode {
public:
	char* code{};
	char ch{ '#' };
};
//读取文件并统计字符频率
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
//打印字符频率
void printFrequency(const map<char, double>& words) {
	cout << "字符\t出现频率" << endl;
	for (const auto& i : words)
	{
		cout << i.first << '\t' << i.second / total_words << endl;
	}
}
//初始化哈夫曼树
void initHT(huffnode* T, const map<char, double>& words, int n) {
	int i = 0;
	for (const auto& j : words) {
		T[i].weight = j.second / total_words;
		T[i].ch = j.first;
		++i;
	}
}
//选择权值最小的两个节点
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
	//求双亲节点为0且为倒数第二小的元素，将其下标传给s1
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
//创建哈夫曼树
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
//哈夫曼编码
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
//打印哈夫曼编码
void printCode(huffcode* HC, int n) {
	cout << "字符\t编码" << endl;
	for (int i = 0; i < n; i++) {
		cout << HC[i].ch;
		cout << "\t"; puts(HC[i].code); cout << endl;
	}
}
//压缩
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
	cout << "压缩成功！" << endl;
	cout << codestream << endl;
	while (codestream.size() >= 8)   //01串转换为字符并输出
	{
		bitset<8> bits(codestream, 0, 8);
		codestream.erase(0, 8);
		out << static_cast<char>(bits.to_ulong());
	}
	if (!codestream.empty())   //结尾长度不够八位，用0补齐，记录剩余的长度将其放在文件末尾，在读取时转换为下标位置还原本来的字符串
	{
		unsigned long loc = codestream.size();
		codestream.append(8 - loc, '0');
		bitset<8> bits1(codestream, 0, 8);

		out << static_cast<char>(bits1.to_ulong());
		out << static_cast<char>(8 - loc);
	}
	else {//结尾长度刚好为八位，用0补齐，记录剩余的长度将其放在文件末尾，在读取时转换为下标位置还原本来的字符串
		codestream.append(8, '0');
		bitset<8> bits(codestream, 0, 8);
		out << static_cast<char>(bits.to_ulong());
	}
	in.close();
	out.close();
}
//解压
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
	cout << "解压文件为: " << endl << endl;
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
//计算文件压缩率
void compressRate() {
	ifstream in("input.txt", ios::in);
	ifstream out("output.dat", ios::in | ios::binary);
	in.seekg(0, ios::end);
	out.seekg(0, ios::end);
	cout << "压缩率为：    " << out.tellg() * 1.0 / in.tellg() << endl;

	in.close();
	out.close();
}
//与原文比较
void compare() {
	ifstream in("input.txt", ios::in);
	ifstream out("decode.txt", ios::in);
	string str1{};
	string str2{};
	string temp{};
	while (!in.eof()) {//
		getline(in, temp);//读取一行
		str1 += temp;//将读取的一行加入到str1中
	}
	while (!out.eof()) {
		getline(out, str2);//这里测试的时候发现，decode.txt文件没有换行，所以这里不用temp
	}
	if (str1 == str2)cout << "解码正确" << endl;
	else {
		cout << "解码错误" << endl;
		cout << str1 << endl;
		cout << str2 << endl;
	}
	in.close();
	out.close();
}
//主函数
int main() {
	vector<double>weight;//权值
	map<char, double>words;//存储字符和权值
	words = readFile(weight);//读取文件，统计字符出现次数
	//printFrequency(words);//打印频率
	int types = words.size();//字符总数
	auto* T = new huffnode[2 * types - 1];
	initHT(T, words, types);//初始化哈夫曼树
	creatHT(T, types);//创建哈夫曼树
	auto* HC = new huffcode[types];
	huffEncode(T, HC, types);//哈夫曼编码
	printCode(HC, types);//打印编码
	compress(HC, types);//压缩
	decode(T, types);//解压
	compressRate();//计算压缩率
	compare();//与原文比较
}