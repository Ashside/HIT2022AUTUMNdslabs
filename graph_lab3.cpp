#include<iostream>
#include<vector>
#include <stack>
#include <queue>
#include<fstream>
#include <map>

using namespace std;
constexpr auto MAX_LEN = 100;
// ���ڽӾ����ʾͼ
class matrix {
public:
    int ver_nums{ 0 }, edge_nums{ 0 };
    vector<char>vexlist{};
    int edge[MAX_LEN][MAX_LEN]{ INT_MAX };
    void printMatrix()
    {
        cout << "��ӡ�ڽӾ���: " << endl;
        cout << '\t';
        for (auto ch : vexlist) {
            cout << "\t" << ch;
        }
        cout << endl;
        for (int i = 0; i < ver_nums; i++) {
            cout << "\t" << vexlist[i];
            for (int j = 0; j < ver_nums; j++) {
                if (edge[i][j]==INT_MAX)
                {
                    cout << "\t" << "��";
                }
                else cout << "\t" << edge[i][j];
            }
            cout << endl;
        }
    }
    void readFile()
    {
        for (int i = 0; i < MAX_LEN; i++) {
            for (int j = 0; j < MAX_LEN; j++) {
                edge[i][j] = INT_MAX;
            }
        }
        char ch{};
        ifstream in("data.txt");
        in >> ver_nums;
        in >> edge_nums;
        for (int i = 0; i < ver_nums; i++)
        {
            in >> ch;
            vexlist.push_back(ch);
        }

        int ver1{}, ver2{};
        int w{};
        for (int k = 0; k < edge_nums; k++)
        {
            in >> ver1 >> ver2 >> w;
            if (w == 0) {
                k--;
                continue;
            }
            edge[ver1][ver2] = w;
        }
    }
    //dijkstra�㷨
    void dijkstra(int v0)
    {
        int dist[MAX_LEN]{ 0 };
        int path[MAX_LEN]{ 0 };
        bool s[MAX_LEN]{ false };
        for (int i = 0; i < ver_nums; i++)
        {
            dist[i] = edge[v0][i];
            s[i] = false;
            path[i] = v0;
        }
        s[v0] = true;
        dist[v0] = 0;
        for (int i = 0; i < ver_nums; i++)
        {
            int min = INT_MAX;
            int u = v0;
            for (int j = 0; j < ver_nums; j++)
            {
                if (!s[j] && dist[j] < min) {
                    u = j;
                    min = dist[j];
                }
            }
            s[u] = true;
            for (int j = 0; j < ver_nums; j++)
            {
                if (!s[j] && edge[u][j] != INT_MAX && (dist[u] + edge[u][j] < dist[j])) {
                    dist[j] = dist[u] + edge[u][j];
                    path[j] = u;
                }
            }
        }
        for (int i = 0; i < ver_nums; i++)
        {
            if (i != v0)
            {
                if (dist[i] == INT_MAX) {
                    cout << "��" << vexlist[v0] << "��" << vexlist[i] << "���������·��"<< endl;
                    continue;
                }
                cout << "��" << vexlist[v0] << "��" << vexlist[i] << "�����·��Ϊ: " << dist[i] << endl;
                cout << "·��Ϊ: ";
                int j = i;
                stack<char> st;
                while (j != v0) {
                    st.push(vexlist[j]);
                    j = path[j];
                }
                st.push(vexlist[v0]);
                while (!st.empty()) {
                    cout << st.top() << " ";
                    st.pop();
                }
                cout << endl;
            }
        }
    }
    //floyd�㷨
    void floyd()
    {
        int dist[MAX_LEN][MAX_LEN]{ 0 };
        int path[MAX_LEN][MAX_LEN]{ 0 };
        for (int i = 0; i < ver_nums; i++)
        {
            for (int j = 0; j < ver_nums; j++)
            {
                dist[i][j] = edge[i][j];
                path[i][j] = i;
            }
        }
        for (int k = 0; k < ver_nums; k++)
        {
            for (int i = 0; i < ver_nums; i++)
            {
                for (int j = 0; j < ver_nums; j++)
                {
                    if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX && (dist[i][k] + dist[k][j] < dist[i][j])) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                        path[i][j] = k;
                    }
                }
            }
        }
        for (int i = 0; i < ver_nums; i++)
        {
            for (int j = 0; j < ver_nums; j++)
            {
                if (i != j)
                {
                    if (dist[i][j] == INT_MAX) {
                        cout << "��" << vexlist[i] << "��" << vexlist[j] << "���������·��" << endl;
                        continue;
                    }
                    cout << "��" << vexlist[i] << "��" << vexlist[j] << "�����·��Ϊ: " << dist[i][j] << endl;
                    cout << "·��Ϊ: ";
                    int k = j;
                    stack<char> st;
                    while (k != i) {
                        char temp = vexlist[k];
                        st.push(temp);
                        k = path[i][k];
                    }
                    st.push(vexlist[i]);
                    while (!st.empty()) {
                        cout << st.top() << " ";
                        st.pop();
                    }
                    cout << endl;
                }
            }
        }
    }
    void dest_sig() {
        int dist[MAX_LEN][MAX_LEN]{ 0 };
        int path[MAX_LEN][MAX_LEN]{ 0 };
        cout << "����Ŀ������: " << endl;
        int c;
        cin >> c;
        for (int i = 0; i < ver_nums; i++)
        {
            for (int j = 0; j < ver_nums; j++)
            {
                dist[i][j] = edge[i][j];
                path[i][j] = i;
            }
        }
        for (int k = 0; k < ver_nums; k++)
        {
            for (int i = 0; i < ver_nums; i++)
            {
                for (int j = 0; j < ver_nums; j++)
                {
                    if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX && (dist[i][k] + dist[k][j] < dist[i][j])) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                        path[i][j] = k;
                    }
                }
            }
        }
        pair<int, int>min;
        min.first = 0;
        min.second = dist[0][c];
        for (int i = 0; i < ver_nums; i++)
        {
            if (i != c && dist[i][c] != INT_MAX)
            {
                if (min.second > dist[i][c]) {
                    min.first = i;
                    min.second = dist[i][c];
                }
            }
        }

        cout << "��" << vexlist[min.first] << "��" << vexlist[c] << "�����·��Ϊ: " << dist[min.first][c] << endl;
        cout << "·��Ϊ: ";
        int k = c;
        stack<char> st;
        while (k != min.first) {
            char temp = vexlist[k];
            st.push(temp);
            k = path[min.first][k];
        }
        st.push(vexlist[min.first]);
        while (!st.empty()) {
            cout << st.top() << " ";
            st.pop();
        }
        cout << endl;
    }
    void dest_pair() {
        int dist[MAX_LEN][MAX_LEN]{ 0 };
        int path[MAX_LEN][MAX_LEN]{ 0 };
        cout << "����Ŀ�����Ŷ�: " << endl;
        int u, v;
        cin >> u >> v;
        for (int i = 0; i < ver_nums; i++)
        {
            for (int j = 0; j < ver_nums; j++)
            {
                dist[i][j] = edge[i][j];
                path[i][j] = i;
            }
        }
        for (int k = 0; k < ver_nums; k++)
        {
            for (int i = 0; i < ver_nums; i++)
            {
                for (int j = 0; j < ver_nums; j++)
                {
                    if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX && (dist[i][k] + dist[k][j] < dist[i][j])) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                        path[i][j] = k;
                    }
                }
            }
        }
        pair<int, int>min;
        int k;
        stack<char> st;
        min.first = u;
        min.second = dist[u][v];
        if(min.second!= INT_MAX)
        {
            cout << "��" << vexlist[min.first] << "��" << vexlist[v] << "�����·��Ϊ: " << dist[min.first][v] << endl;
            cout << "·��Ϊ: ";
            k = v;
            
            while (k != min.first) {
                char temp = vexlist[k];
                st.push(temp);
                k = path[min.first][k];
            }
            st.push(vexlist[min.first]);
            while (!st.empty()) {
                cout << st.top() << " ";
                st.pop();
            }
        }
        else {
            cout << "������" <<"��" << vexlist[min.first] << "��" << vexlist[v] << "��·��"<<endl;
        }
        cout << endl;

        min.first = v;
        min.second = dist[v][u];
        if(min.second!= INT_MAX)
        {
            cout << "��" << vexlist[min.first] << "��" << vexlist[u] << "�����·��Ϊ: " << dist[min.first][u] << endl;
            cout << "·��Ϊ: ";
            k = u;
            while (k != min.first) {
                char temp = vexlist[k];
                st.push(temp);
                k = path[min.first][k];
            }
            st.push(vexlist[min.first]);
            while (!st.empty()) {
                cout << st.top() << " ";
                st.pop();
            }
            cout << endl;
        }
        else {
            cout << "������" << "��" << vexlist[min.first] << "��" << vexlist[u] << "��·��" << endl;
        }
    }
};
int main() {
    matrix m;
    m.readFile();
    m.printMatrix();
    cout << "dijkstra�㷨:" << endl; m.dijkstra(0);
    cout << endl << "floyd�㷨:" << endl; m.floyd(); cout << endl;
    cout << endl; m.dest_sig();
    cout << endl; m.dest_pair();
    return 0;
}