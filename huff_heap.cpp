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
//所有字符总数
double total_chars = 0;
//定义哈夫曼树节点
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
//定义哈夫曼编码
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
		total_chars += i.second;
	}
	for (const auto& i : words) {
		weight.push_back(i.second / total_chars);
	}
	return words;
}
//初始化哈夫曼树
void initHT(huffnode* T, const map<char, double>& words) {
	int i = 0;
	for (const auto& j : words) {
		T[i].weight = j.second / total_chars;
		T[i].ch = j.first;
		++i;
	}
}

//创建哈夫曼树
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
	priority_queue<huffnode*, vector<huffnode*>, Cmp> q;//优先队列，用来替换堆，原理相同

	for (int i = 0; i < n; ++i) {
		q.push(&T[i]);//将所有叶子节点入队
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
//哈夫曼编码
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
	cout << "压缩成功！" << endl;
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

int main() {
	vector<double>weight;//权值
	map<char, double>words;//存储字符和权值
	words = readFile(weight);//读取文件，统计字符出现次数
	//printFrequency(words);//打印频率
	int types = words.size();//字符总数
	auto* T = new huffnode[2 * types - 1];
	initHT(T, words);//初始化哈夫曼树
	creatHT(T, types);//创建哈夫曼树
	auto* HC = new huffcode[types]; cout << "编码成功" << endl;
	huffEncode(T, HC, types);//哈夫曼编码
	printCode(HC, types);//打印编码
	compress(HC, types);//压缩
	decode(T, types);//解压
	compressRate();//计算压缩率
	compare();//与原文比较
}