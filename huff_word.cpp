#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <fstream>
#include <bitset>

using namespace std;
double total_words = 0;
class huffnode {
public:
	double weight{ 0 };
	int lc{ -1 };
	int rc{ -1 };
	int pa{ -1 };
	string ch{};
};
//定义哈夫曼编码
class huffcode {
public:
	char* code{};
	string ch{};
};
map<string, double>divideString(string str)
{
	//将字符串拆分为单词
	string s = str;
	string ch{};
	map<string, double> word_count;
	stringstream ss;
	int to_min = 0;
	ss << s;
	while (ss >> s) {
		if (s.back() == ',' || s.back() == '"' || s.back() == '.' || s.back() == '!' || s.back() == '?' || s.back() == ':') {
			ch += s.back();
			s.pop_back();
		}
		if (word_count.end() == word_count.find(s)) {
			word_count.insert(make_pair(s, 1));
		}

		else {
			++word_count[s];
		}

		if (!ch.empty()) {
			if (word_count.end() == word_count.find(ch)) {
				word_count.insert(make_pair(ch, 1));
			}
			else {
				++word_count[ch];
			}
			ch = {};
			to_min++;
		}
	}

	for (auto& w : word_count) {
		total_words += w.second;
	}
	double space_num = word_count.size() - to_min - 1;
	string space = { " " };
	word_count[space] = space_num;
	total_words += space_num;
	return word_count;
}

string readFile() {
	ifstream in("input.txt", ios::in);
	string str{}, temp{};
	map<char, double> words;
	while (getline(in, temp))
	{
		str += temp;
	}
	return str;
}
void printFrequency(const map<string, double>& word_count) {
	cout << "单词\t出现频率" << endl;
	for (const auto& i : word_count)
	{
		cout << i.first << '\t' << i.second / total_words << endl;
	}
}
void initHT(huffnode* T, const map<string, double>& word_count, int n) {
	int i = 0;
	for (const auto& j : word_count) {
		T[i].weight = j.second / total_words;
		T[i].ch = j.first;
		++i;
	}
}
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
void printCode(huffcode* HC, int n) {
	cout << "单词\t编码" << endl;
	for (int i = 0; i < n; i++) {
		cout << HC[i].ch;
		cout << "\t"; puts(HC[i].code); cout << endl;
	}
}
void compress(huffcode* HC, int n) {
	ifstream in("input.txt", ios::in);
	ofstream out("output.dat", ios::out | ios::binary);
	string str{}, temp{}, ch{};
	string space = { " " };
	string codestream{};
	while (getline(in, temp))
	{
		str += temp;
	}

	int i_space = 0;
	for (i_space = 0; i_space < n; i_space++) {
		if (HC[i_space].ch == space) {
			break;
		}
	}
	stringstream ss;
	ss << str;
	temp = {};
	while (ss >> temp) {
		if (temp.back() == ',' || temp.back() == '"' || temp.back() == '.' || temp.back() == '!' || temp.back() == '?' || temp.back() == ':') {
			ch += temp.back();
			temp.pop_back();
		}
		for (int i = 0; i < n; i++) {
			if (HC[i].ch == temp) {
				codestream += HC[i].code;
			}
		}
		if (!ch.empty()) {
			for (int i = 0; i < n; i++) {
				if (HC[i].ch == ch) {
					codestream += HC[i].code;
					ch = {};
				}
			}
		}
		codestream += HC[i_space].code;
	}
	cout << codestream;
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
void compressRate() {
	ifstream in("input.txt", ios::in);
	ifstream out("output.dat", ios::in | ios::binary);
	in.seekg(0, ios::end);
	out.seekg(0, ios::end);
	cout << "压缩率为：    " << out.tellg() * 1.0 / in.tellg() << endl;

	in.close();
	out.close();
}
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
	for (int i = 0; i < str1.size(); i++) {
		if (str1[i] != str2[i]) {
			cout << "解码错误" << endl;
			return;
		}
	}
	cout << "解码正确" << endl;
	in.close();
	out.close();
}
int main() {
	string str;
	map<string, double> word_count;
	str = readFile();
	word_count = divideString(str);
	int types = word_count.size();//字符总数
	auto* T = new huffnode[2 * types - 1];
	initHT(T, word_count, types);
	auto* HC = new huffcode[types];
	//printFrequency(word_count);
	creatHT(T, types);
	huffEncode(T, HC, types);
	printCode(HC, types);
	compress(HC, types);
	decode(T, types);
	compressRate();
	compare();
	delete[] T;
	delete[] HC;
	return 0;
}