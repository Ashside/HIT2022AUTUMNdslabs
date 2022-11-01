#include<iostream>
#include<vector>
#include <stack>
#include <queue>
#include<fstream>
#include <map>

using namespace std;
constexpr auto MAX_LEN = 100;
// 用邻接矩阵表示图
class matrix {
public:
    int ver_nums{ 0 }, edge_nums{ 0 };
    vector<char>vexlist{};
    int edge[MAX_LEN][MAX_LEN]{ INT_MAX };
    void printMatrix()
    {
        cout << "打印邻接矩阵: " << endl;
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
                    cout << "\t" << "∞";
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
    //dijkstra算法
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
                    cout << "从" << vexlist[v0] << "到" << vexlist[i] << "不存在最短路径"<< endl;
                    continue;
                }
                cout << "从" << vexlist[v0] << "到" << vexlist[i] << "的最短路径为: " << dist[i] << endl;
                cout << "路径为: ";
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
    //floyd算法
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
                        cout << "从" << vexlist[i] << "到" << vexlist[j] << "不存在最短路径" << endl;
                        continue;
                    }
                    cout << "从" << vexlist[i] << "到" << vexlist[j] << "的最短路径为: " << dist[i][j] << endl;
                    cout << "路径为: ";
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
        cout << "输入目标点序号: " << endl;
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

        cout << "从" << vexlist[min.first] << "到" << vexlist[c] << "的最短路径为: " << dist[min.first][c] << endl;
        cout << "路径为: ";
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
        cout << "输入目标点序号对: " << endl;
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
            cout << "从" << vexlist[min.first] << "到" << vexlist[v] << "的最短路径为: " << dist[min.first][v] << endl;
            cout << "路径为: ";
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
            cout << "不存在" <<"从" << vexlist[min.first] << "到" << vexlist[v] << "的路径"<<endl;
        }
        cout << endl;

        min.first = v;
        min.second = dist[v][u];
        if(min.second!= INT_MAX)
        {
            cout << "从" << vexlist[min.first] << "到" << vexlist[u] << "的最短路径为: " << dist[min.first][u] << endl;
            cout << "路径为: ";
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
            cout << "不存在" << "从" << vexlist[min.first] << "到" << vexlist[u] << "的路径" << endl;
        }
    }
};
int main() {
    matrix m;
    m.readFile();
    m.printMatrix();
    cout << "dijkstra算法:" << endl; m.dijkstra(0);
    cout << endl << "floyd算法:" << endl; m.floyd(); cout << endl;
    cout << endl; m.dest_sig();
    cout << endl; m.dest_pair();
    return 0;
}