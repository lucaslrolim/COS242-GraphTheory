#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <queue>
#include <stack>
#include <set>
#include <sstream>
using namespace std;
string IntToString(int a)
{
    ostringstream temp;
    temp << a;
    return temp.str();
}
string FloatToString(float a)
{
    ostringstream temp;
    temp << a;
    return temp.str();
}
class adjMatrix
{
private:
    int vertices;
    bool **p;
public:
    adjMatrix() {
    }
void fill(int vertices){
        int m_row = vertices;
        int m_col = vertices;
        p = new bool*[m_row];
        for (int i = 0; i < m_row; i++){
            p[i] = new bool[m_col];
        }
        for (int i = 0; i < m_row; i++){
            for (int j = 0; j < m_col; j++){
                p[i][j] = 0;
            }
        }
    }
    void insert(string edge){ 
        istringstream iss(edge);
        string s1, s2;
        iss >> s1;
        iss >> s2;
        int r = atoi(s1.c_str()) - 1;
        int c = atoi(s2.c_str()) - 1;
        p[r][c] = 1;
        p[c][r] = 1;
    }
    void display()
    {
        int i, j;
        for (i = 0; i < vertices; i++)
        {
            for (j = 0; j < vertices; j++)
                cout << p[i][j] << "  ";
            cout << endl;
        }
    }
    bool getEdge(int row, int col) {
        return p[row][col];
    }
    ~adjMatrix() {
    }
};
class adjVector {
public:
    int v_vertices;
    vector <int> *p;
    vector <int> v_empty;
    adjVector() {
    }
    void fill(int vertices) {
        v_vertices = vertices;
        p = new vector <int>[v_vertices];
        for (int i = 0; i < v_vertices; i++) {
            p[i] = v_empty;
        }
    }
    vector <int> *getEdgesList(int vertice) {
        return &p[vertice - 1];
    }
    void insert(string edge){
        istringstream iss(edge);
        string s1, s2;
        iss >> s1;
        iss >> s2;
        int v1 = atoi(s1.c_str());
        int v2 = atoi(s2.c_str());
        p[v1 - 1].push_back(v2);
        p[v2 - 1].push_back(v1);
    }
    void display() {
        for (int i = 0; i<v_vertices; i++) {
            for (int j = 0; j<p[i].size(); j++) {
                cout << p[i][j] << " ";
            }
            cout << endl;
        }
    }
    ~adjVector() {
    }
};
class graph {
public:
    int g_verticesNumber;
    int g_edgesNumber;
    const char* g_representation;
    adjMatrix my_graphm;
    adjVector my_graphv;
    graph(const char* file, const char* representation) {
        g_representation = representation;
        string line;
        ifstream myfile(file);
        myfile >> g_verticesNumber;
        if (string(g_representation) == string("matrix")){
            my_graphm.fill(g_verticesNumber);
        }
        if (string(g_representation) == string("vector")) {
            my_graphv.fill(g_verticesNumber);
        }
        g_edgesNumber = 0;
        int a = 0, b = 0;
        if (myfile.is_open()) {
            while (!myfile.eof()) {
              //  myfile >> a >> b;
                getline(myfile, line);
                if (string(g_representation) == string("matrix")){
                    my_graphm.insert(line);
                }
                if (string(g_representation) == string("vector")) {
                    my_graphv.insert(line);
                }
                ++g_edgesNumber;
            }
            myfile.close();
        }
    };
    void createFile() {
        ofstream output_file;
        output_file.open("graph_info.txt");
        output_file << "# n = " + IntToString(g_verticesNumber) + "\n";
        output_file << "# m = " + IntToString(g_edgesNumber) + "\n";
        output_file << "# d_medio = " + FloatToString((float)2 * (g_edgesNumber) / g_verticesNumber) + "\n";
        vector <int> incidence(g_verticesNumber);

        if (string(g_representation) == string("matrix")) {
            int degree;
            for (int i = 0; i < g_verticesNumber; i++) {
                degree = 0;
                for (int j = 0; j < g_verticesNumber; j++) {
                    degree += my_graphm.getEdge(i, j);
                }
                incidence[degree - 1] += 1;
            }
            for (int line = 1; line < g_verticesNumber; line++) {
                output_file << IntToString(line) + " " + FloatToString((float)incidence[line - 1] / g_verticesNumber) + "\n"; 
            }
        }
        if (string(g_representation) == string("vector")) {
            for (int i = 0; i < g_verticesNumber; i++) {
                incidence[my_graphv.getEdgesList(i + 1)->size() - 1] += 1;
            }
            for (int line = 1; line < g_verticesNumber; line++) {
                output_file << IntToString(line) + " " + FloatToString((float)incidence[line - 1] / g_verticesNumber) + "\n";
            }
       }
        output_file << endl;
        output_file.close();
    }
    void bfs(int vertice) {
        vector<bool> marking;
        vector<int>   level;
        vector<int> parent;
        for (int i = 0; i<g_verticesNumber; i++) {
            level.push_back(0);
            marking.push_back(0);
            parent.push_back(0);
        }
        queue<int> vqueue;
        marking[vertice - 1] = 1;
        vqueue.push(vertice);
        while (!vqueue.empty()) {
            int v = vqueue.front();
            vqueue.pop();
            if (string(g_representation) == string("matrix")) {
                for (int i = 0; i<g_verticesNumber; i++) {
                    if (my_graphm.getEdge(v - 1, i) == 1 && marking[i] == 0) {
                        marking[i] = 1;
                        level[i] = level[v - 1] + 1;
                        parent[i] = v;
                        vqueue.push(i + 1);
                    }
                }
            }
            if (string(g_representation) == string("vector")) {
                for (int i = 0; i<my_graphv.getEdgesList(v)->size(); i++) {
                    int x = my_graphv.p[v - 1][i];
                    if (marking[x - 1] == 0) {
                        marking[x - 1] = 1;
                        level[x - 1] = level[v - 1] + 1;
                        parent[x - 1] = v;
                        vqueue.push(x);
                    }
                }
            }
        }
        ofstream o_file;
        o_file.open("spanning_tree.txt");
        o_file << "V P N\n";
        for (int i = 0; i<g_verticesNumber; i++) {
            o_file << IntToString(i + 1) + " " + IntToString(parent[i]) + " " + IntToString(level[i]);
            if (i<g_verticesNumber - 1) {
                o_file << "\n";
            }
        }
        o_file.close();
    }
    void dfs(int vertice) {
        vector<bool> marking;
        vector<int> level;
        vector<int> parent;
        for (int i = 0; i<g_verticesNumber; i++) {
            level.push_back(0);
            marking.push_back(0);
            parent.push_back(0);
        }
        stack<int> vstack;
        vstack.push(vertice);
        while (!vstack.empty()) {
            int u = vstack.top();
            vstack.pop();
            if (marking[u - 1] == 0) {
                marking[u - 1] = 1;
                if (string(g_representation) == string("matrix")) {
                    for (int i = g_verticesNumber - 1; i >= 0; i--) {
                        if (my_graphm.getEdge(u - 1, 1) == 1) {
                            vstack.push(i + 1);
                            if (marking[i] == 0) {
                                parent[i] = u;
                                level[i] = level[u - 1] + 1;
                            }
                        }
                    }
                }
                if (string(g_representation) == string("vector")) {
                    for (int i = my_graphv.getEdgesList(u)->size() - 1; i >= 0; i--) {
                        int x = my_graphv.p[u - 1][i];
                        vstack.push(x);
                        if (marking[x - 1] == 0) {
                            parent[x - 1] = u;
                            level[x - 1] = level[u - 1] + 1;
                        }
                    }
                }
            }
        }
        ofstream o_file;
        o_file.open("spanning_tree.txt");
        o_file << "V P N\n";
        for (int i = 0; i<g_verticesNumber; i++) {
            o_file << IntToString(i + 1) + " " + IntToString(parent[i]) + " " + IntToString(level[i]);
            if (i<g_verticesNumber - 1) {
                o_file << "\n";
            }
        }
        o_file.close();
    }
    void connectedComponents() {
        vector<int> marking;
        int currentVertice = 1;
        vector<vector<int> > cComponents;
        for (int i = 0; i<g_verticesNumber; i++) {
            marking.push_back(0);
        }
        int cComponentsNumber = 0;
        while (currentVertice>0) {
            int vertice = currentVertice;
            queue<int> vqueue;
            marking[vertice - 1] = cComponentsNumber + 1;
            vqueue.push(vertice);
            while (!vqueue.empty()) {
                int v = vqueue.front();
                vqueue.pop();
                if (string(g_representation) == string("matrix")) {
                    for (int i = 0; i<g_verticesNumber; i++) {
                        if (my_graphm.getEdge(v - 1, i) == 1 && marking[i] == 0) {
                            marking[i] = cComponentsNumber + 1;
                            vqueue.push(i + 1);
                        }
                    }
                }
                if (string(g_representation) == string("vector")) {
                    for (int i = 0; i<my_graphv.getEdgesList(v - 1)->size(); i++) {
                        int x = my_graphv.p[v - 1][i];
                        if (marking[x - 1] == 0) {
                            marking[x - 1] = cComponentsNumber + 1;
                            vqueue.push(x);
                        }
                    }
                }
            }
            ++cComponentsNumber;
            if (currentVertice != g_verticesNumber) {
                for (int i = currentVertice; i<g_verticesNumber; i++) {
                    if (marking[i] == 0) {
                        currentVertice = i + 1;
                    }
                    else if (i == g_verticesNumber - 1) {
                        currentVertice = 0;
                    }
                }
            }
            else {
                currentVertice = 0;
            }
        }
        for (int i = 0; i<cComponentsNumber; i++) {
            vector<int> v;
            cComponents.push_back(v);
        }
        for (int i = 0; i<g_verticesNumber; i++) {
            cComponents[marking[i] - 1].push_back(i + 1);
        }
        for (int i = 0; i<cComponentsNumber; i++) {
            for (vector<int>::iterator it = cComponents[i].begin(); it<cComponents[i].end(); it++) {
                cout << *it << " ";
            }
            cout << endl;
        }
    }
    int diameter() {
        int ret = 0;
        for (int vertice = 1; vertice <= g_verticesNumber; vertice++) {
            vector<bool> marking;
            vector<int> level;
            for (int i = 0; i<g_verticesNumber; i++) {
                level.push_back(0);
                marking.push_back(0);
            }
            queue<int> vqueue;
            marking[vertice - 1] = 1;
            vqueue.push(vertice);
            while (!vqueue.empty()) {
                int v = vqueue.front();
                vqueue.pop();
                if (string(g_representation) == string("matrix")) {
                    for (int i = 0; i<g_verticesNumber; i++) {
                        if (my_graphm.getEdge(v - 1, i) == 1 && marking[i] == 0) {
                            marking[i] = 1;
                            if (level[v - 1] + 1>ret) {
                                ret = level[v - 1] + 1;
                            }
                            level[i] = level[v - 1] + 1;
                            vqueue.push(i + 1);
                        }
                    }
                }
                if (string(g_representation) == string("vector")) {
                    for (int i = 0; i<my_graphv.getEdgesList(v - 1)->size(); i++) {
                        int x = my_graphv.p[v - 1][i];
                        if (marking[x - 1] == 0) {
                            marking[x - 1] = 1;
                            if (level[v - 1] + 1>ret) {
                                ret = level[v - 1] + 1;
                            }
                            level[x - 1] = level[v - 1] + 1;
                            vqueue.push(x);
                        }
                    }
                }
            }
        }
        return ret;
    }
};
int main() {

    return 0;
};
