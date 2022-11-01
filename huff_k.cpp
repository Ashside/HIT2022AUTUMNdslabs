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
	huffnode* mc{ nullptr };
	huffnode* pa{ nullptr };
	char ch{ '#' };
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
	priority_queue<huffnode*, vector<huffnode*>, Cmp> q;//优先队列，用来替换堆，原理相同

	for (int i = 0; i < n; ++i) {
		q.push(&T[i]);//将所有叶子节点入队
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
	cout << endl << "压缩成功！" << endl;
}
//解压
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
//计算文件压缩率
void compressRate() {
	ifstream in("input.txt", ios::in);
	ifstream out("output.txt", ios::in);
	in.seekg(0, ios::end);
	out.seekg(0, ios::end);
	cout << "压缩率为：    " << out.tellg() * 1.0 / in.tellg() / 8 << endl;

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
	huffnode* root{};
	root=creatHT(T, types);//创建哈夫曼树
	auto* HC = new huffcode[types]; cout << "编码成功" << endl;
	huffEncode(T, HC, types);//哈夫曼编码
	printCode(HC, types);//打印编码
	compress(HC, types);//压缩
	decode(T, types,root);//解压
	compressRate();//计算压缩率
	compare();//与原文比较
}