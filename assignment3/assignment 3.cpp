#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <queue>
#include <stack>
#include <ctime>
#include <climits>
#include <algorithm>
#include <map>
#include <limits>
#include <utility>
#include "myHeap.h"
#define FLOAT_MAX numeric_limits<float>::infinity()
using namespace std;
 
class adjacency_matrix {
private:
    int order;
    int *degrees;
    bool **myArray;
    float **myWArray;
    bool weighted;
public:
    adjacency_matrix(){
 
    }
    ~adjacency_matrix() {
        delete [] degrees;
        if (weighted) {
            for (int i=0; i<order; i++){
                delete [] myWArray[i];
            }
            delete [] myWArray;
        }
        else {
            for (int i=0; i<order; i++){
                delete [] myArray[i];
            }
            delete [] myArray;
        }
    }
    void fill(int v_number, bool w) {
        weighted=w;
        order=v_number;
        degrees=new int[order];
        if (weighted) {
            myWArray=new float* [order];
            for (int i=0; i<order; i++) {
                myWArray[i]=new float [order];
            }
            for (int i=0; i<order; i++){
                for (int j=0; j<order; j++){
                    myWArray[i][j]=FLOAT_MAX;
                }
            }
        }
        else {
            myArray=new bool* [order];
            for (int i=0; i<order; i++){
                degrees[i]=0;
                myArray[i]=new bool [order];
            }
            for (int i=0; i<order; i++){
                for (int j=0; j<order; j++){
                    myArray[i][j]=0;
                }
            }
        }
    }
    int getNeighbor(int vertex, int neighborOrder){
        int neighborNumber=1;
        if (weighted) {
            for (int i=0; i<order; i++){
                if (myWArray[vertex-1][i]!=FLOAT_MAX){
                    if (neighborNumber==neighborOrder){
                        return i+1;
                    }
                    else {
                        ++neighborNumber;
                    }
                }
            }
        }
        else {
            for (int i=0; i<order; i++){
                if (myArray[vertex-1][i]==1){
                    if (neighborNumber==neighborOrder){
                        return i+1;
                    }
                    else {
                        ++neighborNumber;
                    }
                }
            }
        }
    }
    void insertEdge(int r, int c) {
        myArray[r-1][c-1] = 1;
        myArray[c-1][r-1] = 1;
        degrees[r-1]+=1;
        degrees[c-1]+=1;
    }
    void insertEdge(int r, int c, float w) {
        degrees[r-1]+=1;
        degrees[c-1]+=1;
        myWArray[r-1][c-1] = w;
        myWArray[c-1][r-1] = w;
    }
    float getWeight(int i, int j, int k) {
        return myWArray[i-1][j-1];
    }
    void show(){
        if (weighted) {
            for (int i = 0; i < order; i++){
                for (int j = 0; j < order; j++){
                    bool edge=myWArray[i][j]!=FLOAT_MAX;
                    cout << edge << " ";
                }
                cout << endl;
            }
        }
        else {
            for (int i = 0; i < order; i++){
                for (int j = 0; j < order; j++){
                    cout << myArray[i][j] << " ";
                }
                cout << endl;
            }
        }
    }
    int getDegree(int vertex){
        return degrees[vertex-1];
    }
};
 
class adjacency_vector {
private:
    int size;
    vector<int> *myArray;
    vector<pair<int, float> > *myWArray;
    bool weighted;
public:
    adjacency_vector(){
 
    }
    ~adjacency_vector(){
        if (weighted) {
            delete [] myWArray;
        }
        else {
            delete [] myArray;
        }
    }
    void fill(int v_number, bool w) {
        size=v_number;
        weighted=w;
        if (weighted) {
            myWArray=new vector<pair<int, float> > [size];
            for (int i=0; i<size; i++){
                vector<pair<int, float> > v;
                myWArray[i]=v;
            }
        }
        else {
            myArray=new vector<int> [size];
            for (int i=0; i<size; i++){
                vector<int> v;
                myArray[i]=v;
            }
        }
    }
    void insertEdge(int v1, int v2) {
        myArray[v1 - 1].push_back(v2);
        myArray[v2 - 1].push_back(v1);
    }
    void insertEdge (int v1, int v2, float w) {
        pair<int, float> edge(v2, w);
        myWArray[v1-1].push_back(edge);
        edge=make_pair(v1, w);
        myWArray[v2-1].push_back(edge);
    }
    float getWeight(int i, int j, int k=-1) {
        if (k!=-1) {
            return myWArray[i-1][k-1].second;
        }
        for (int l=0; l<getDegree(i); l++) {
            if (myWArray[i-1][l].first==j) {
                return myWArray[i-1][l].second;
            }
        }
    }
    int getNeighbor(int vertex, int neighborOrder){
        if (weighted) {
            return myWArray[vertex-1][neighborOrder-1].first;
        }
        return myArray[vertex-1][neighborOrder-1];
    }
    void show() {
        if (weighted) {
            for (int i = 0; i<size; i++){
                for (int j = 0; j<myWArray[i].size(); j++){
                    cout << myWArray[i][j].first << " ";
                }
                cout << endl;
            }
        }
        else {
            for (int i = 0; i<size; i++){
                for (int j = 0; j<myArray[i].size(); j++){
                    cout << myArray[i][j] << " ";
                }
                cout << endl;
            }
        }
    }
    int getDegree(int vertex){
        if (weighted) {
            return myWArray[vertex-1].size();
        }
        return myArray[vertex-1].size();
    }
};
 
template <typename T>
class Graph {
public:
    int verticesNumber;
    int edgesNumber;
    float averageDegree;
    T container;
    bool negativeWeight;
    bool weightedGraph;
    map <int, string>  vertex_name;
    vector <float> min_dist;
    vector <int> parent;
    vector <int> color;
    int colorsNumber;
    string itos(int n){
        ostringstream buffer;
        buffer << n;
        return buffer.str();
    }
    string ftos(float n){
        ostringstream buffer;
        buffer << n;
        return buffer.str();
    }
    struct myclass {
        bool operator() (vector<int> i,vector<int> j) { return (i.size()>j.size());}
    } myobject;
public:
    Graph(const char* fileName) {
        weightedGraph=0;
        ifstream myTest(fileName);
        string line;
        myTest >> verticesNumber;
        getline(myTest,line);
        getline(myTest,line);
        int spaces = count(line.begin(), line.end(), ' ');
        if (spaces >= 2){
            weightedGraph = 1;
        }
        myTest.close();
        ifstream myFile(fileName);
        getline(myFile,line);
        container.fill(verticesNumber, weightedGraph);
        edgesNumber = 0;
        int a = 0, b = 0;
        float w = 0;
        negativeWeight = false;
        while(!myFile.eof()) {
            myFile >> a >> b;
            if(weightedGraph){
                myFile >> w;
                if (w < 0){
                    negativeWeight = true;
                }
                container.insertEdge(a,b,w);
            }
            else {
                container.insertEdge(a,b);
            }
            ++edgesNumber;
        }
        myFile.close();
        averageDegree=(float)2*edgesNumber/verticesNumber;
    }
    ~Graph() {
 
    }
    void show(){
        container.show();
    }
    void createFile(){
        ofstream myFile;
        myFile.open("graph_info.txt");
        myFile << "# n = " + itos(verticesNumber) + "\n";
        myFile << "# m = " + itos(edgesNumber) + "\n";
        myFile << "# d_medio = " + ftos(averageDegree) + "\n";
        int *distribution;
        distribution=new int [verticesNumber-1];
        for (int i=0; i<verticesNumber-1; i++){
            distribution[i]=0;
        }
        for (int i=0; i<verticesNumber; i++){
            distribution[container.getDegree(i+1)-1]+=1;
        }
        for (int i=0; i<verticesNumber-1; i++){
            myFile << i+1 << " " << (float)distribution[i]/verticesNumber;
            if (i!=verticesNumber-2){
                myFile << "\n";
            }
        }
        myFile.close();
        delete [] distribution;
    }
    void bfs(int s_vertex, bool createFile=1){
        vector<bool> marking(verticesNumber);
        parent.clear();
        min_dist.clear();
        for (int i=0; i<verticesNumber; i++){
            marking[i]=0;
            min_dist.push_back(FLOAT_MAX);
            parent.push_back(-1);
        }
        queue<int> vqueue;
        marking[s_vertex-1]=1;
        parent[s_vertex-1]=0;
        min_dist[s_vertex-1]=0;
        vqueue.push(s_vertex);
        while(!vqueue.empty()){
            int v=vqueue.front();
            vqueue.pop();
            for (int i=0; i<container.getDegree(v); i++){
                int x=container.getNeighbor(v,i+1);
                if (marking[x-1]==0){
                    marking[x-1]=1;
                    min_dist[x-1]=min_dist[v-1]+1;
                    parent[x-1]=v;
                    vqueue.push(x);
                }
            }
        }
        if (createFile){
            ofstream myFile;
            myFile.open("spanning_tree.txt");
            myFile << "Vertice Pai Nivel\n";
            for (int i=0; i<verticesNumber; i++){
                myFile << i+1 << " " << parent[i] << " " << min_dist[i];
                if (i<verticesNumber-1){
                    myFile << "\n";
                }
            }
            myFile.close();
        }
    }
    void dfs(int s_vertex, bool createFile=1){
        vector<bool> marking(verticesNumber);
        parent.clear();
        for (int i=0; i<verticesNumber; i++){
            marking[i]=0;
            min_dist.push_back(FLOAT_MAX);
            parent.push_back(-1);
        }
        stack<int> vstack;
        parent[s_vertex-1]=0;
        min_dist[s_vertex-1]=0;
        vstack.push(s_vertex);
        while(!vstack.empty()){
            int u=vstack.top();
            vstack.pop();
            if (marking[u-1]==0){
                marking[u-1]=1;
                for (int i=container.getDegree(u)-1; i>=0; i--){
                    int x=container.getNeighbor(u, i+1);
                    vstack.push(x);
                    if (marking[x-1]==0){
                        parent[x-1]=u;
                        min_dist[x-1]=min_dist[u-1]+1;
                    }
                }
            }
        }
        if (createFile){
            ofstream myFile;
            myFile.open("spanning_tree.txt");
            myFile << "Vertice Pai Nivel\n";
            for (int i=0; i<verticesNumber; i++){
                myFile << i+1 << " " << parent[i] << " " << min_dist[i];
                if (i<verticesNumber-1){
                    myFile << "\n";
                }
            }
            myFile.close();
        }
    }
    void connectedComponents(bool print=1){
        vector<int> marking(verticesNumber);
        int currentVertex=1;
        int cComponentsNumber=0;
        int largestComponent=0;
        int smallestComponent=FLOAT_MAX;
        for (int i=0; i<verticesNumber; i++){
            marking[1]=0;
        }
        while(currentVertex>0){
            int componentSize=0;
            int vertex=currentVertex;
            queue<int> vqueue;
            marking[vertex-1]=cComponentsNumber+1;
            vqueue.push(vertex);
            while(!vqueue.empty()){
                int v=vqueue.front();
                vqueue.pop();
                ++componentSize;
                for (int i=0; i<container.getDegree(v); i++){
                    int x=container.getNeighbor(v,i+1);
                    if (marking[x-1]==0){
                        marking[x-1]=cComponentsNumber+1;
                        vqueue.push(x);
                    }
                }
            }
            ++cComponentsNumber;
            if (largestComponent<componentSize){
                largestComponent=componentSize;
            }
            if (smallestComponent>componentSize){
                smallestComponent=componentSize;
            }
            if (currentVertex!=verticesNumber) {
                for (int i=currentVertex; i<verticesNumber; i++){
                    if(marking[i]==0){
                        currentVertex=i+1;
                        break;
                    }
                    else if (i==verticesNumber-1){
                        currentVertex=0;
                    }
                }
            }
            else {
                currentVertex=0;
            }
        }
        cout << "Numero de Componentes Conexos: " << cComponentsNumber << endl;
        cout << "Tamanho do Maior Componente: " << largestComponent << endl;
        cout << "Tamanho do Menor Componente: " << smallestComponent << endl;
        if (print){
            vector<vector<int> > cComponents(cComponentsNumber);
            vector<int> indexOrder;
            for (int i=0; i<cComponentsNumber; i++){
                vector<int> v;
                cComponents[i]=v;
            }
            for (int i=0; i<verticesNumber; i++){
                cComponents[marking[i]-1].push_back(i+1);
            }
            sort (cComponents.begin(), cComponents.end(), myobject);
            for (int i=0; i<cComponentsNumber; i++){
                for (vector<int>::iterator it=cComponents[i].begin(); it<cComponents[i].end(); it++){
                    cout << *it << " ";
                }
                cout << endl;
            }
        }
    }
    int diameter(){
        int ret=0;
        for (int i=1; i<=verticesNumber; i++){
            if (weightedGraph) {
                dijkstra(i);
            }
            else {
                bfs(i);
            }
            for (int j=0; j<verticesNumber; j++) {
                if (min_dist[j]>ret) {
                    ret=min_dist[j];
                }
            }
        }
        return ret;
    }
    void dijkstra(int s_vertex){
        if (negativeWeight)
        {
            cout << "Todos os pesos precisam ser não negativos" << endl;
            return;
        }
        vector<int> v;
        min_dist.clear();
        parent.clear();
        for(int i = 0; i < verticesNumber;i++){
            min_dist.push_back(FLOAT_MAX);
            parent.push_back(-1);
        }
        min_dist[s_vertex-1] = 0;
        parent[s_vertex-1]=0;
        myHeap heap(verticesNumber);
        for(int i = 1; i <= verticesNumber; i++){
            Vertex vertex(i,min_dist[i-1]);
            heap.push(vertex);
        }
        while(!heap.empty()){
            Vertex min_vertex = heap.pop();
            int u = min_vertex.v_index;
            for (int i = 1; i <= container.getDegree(u); i++) {
                int v= container.getNeighbor(u,i);
                if (min_dist[v-1] > min_dist[u-1] + container.getWeight(u,v,i)){
                        min_dist[v-1] = min_dist[u-1] + container.getWeight(u,v,i);
                        heap.update(v,min_dist[v-1]);
                        parent[v-1]=u;
                }
            }
 
        }
    }
    float mean_dist(){
        if (negativeWeight)
        {
            cout << "Todos os pesos precisam ser não negativos" << endl;
            return 0;
        }
        float total_dist = 0;
        int c=pow(verticesNumber,2)-verticesNumber;
        for (int i = 0; i < verticesNumber; i++){
            dijkstra(i+1);
            for (int j=0; j<verticesNumber; j++) {
                if (min_dist[j]!=FLOAT_MAX)
                {
                    total_dist+=min_dist[j];
                }
                else {
                    --c;
                }
            }
        }
        return total_dist/c;
    }
    void prim(int s_vertex){
        float total_weight = 0;
        vector<bool> marking(verticesNumber);
        min_dist.clear();
        for(int i = 0; i < verticesNumber;i++){
                min_dist.push_back(FLOAT_MAX);
                parent.push_back(-1);
                marking[i]=0;
        }
        min_dist[s_vertex-1] = 0;
        parent[s_vertex-1]=0;
        myHeap heap(verticesNumber);
        for(int i = 1; i <= verticesNumber; i++){
            Vertex vertex(i,min_dist[i-1]);
            heap.push(vertex);
        }
        while(!heap.empty()){
            Vertex min_vertex = heap.pop();
            int u = min_vertex.v_index;
            marking[u-1]=1;
            for (int i = 1; i <= container.getDegree(u); i++) {
                int v= container.getNeighbor(u,i);
                if (min_dist[v-1] > container.getWeight(u,v,i) && marking[v-1] == 0){
                        parent[v-1] = u;
                        min_dist[v-1] = container.getWeight(u,v,i);
                        heap.update(v,min_dist[v-1]);
                }
            }
        }
        ofstream myFile;
        myFile.open("mst.txt");
        myFile << verticesNumber << "\n";
        for (int i = 0; i < parent.size();i++) {
        //  cout << parent[i] << " ";
            if(parent[i] > 0){
                myFile << i + 1 << " " << parent[i] << " " << min_dist[i] << "\n";
                total_weight += min_dist[i];
            }
        }
        cout << endl;
        myFile << total_weight;
        myFile.close();
    }
     void search(int s_vertex, int t_vertex = 0){
        if(weightedGraph){
            dijkstra(s_vertex);
        }
        else{
            bfs(s_vertex);
        }
        if(t_vertex){
            if(min_dist[t_vertex-1] == FLOAT_MAX){
                cout << "Não há caminho entre os vértices "<< s_vertex << " e " << t_vertex << "."<< endl;
            }
            else{
                cout << "Distância entre os vértices " << s_vertex << " e " << t_vertex << ": " << min_dist[t_vertex-1] << endl;
                cout << "Caminho entre os vértices " << s_vertex << " e " << t_vertex << ": " << endl;
                vector<int> path;
                int currentVertex=t_vertex;
                while (currentVertex!=0) {
                    path.push_back(currentVertex);
                    currentVertex=parent[currentVertex-1];
                }
                for (int i=path.size()-1; i>=0; i--) {
                    cout << path[i] << " ";
                }
                cout << endl;
            }
        }
     }
     bool validColoring() {
        for (int i=1; i<=verticesNumber; i++) {
            for (int j=1; j<=container.getDegree(i); j++) {
                int x=container.getNeighbor(i,j);
                if (color[i-1]==color[x-1]) {
                    return 0;
                }
            }
        }
        return 1;
     }
     int getMaxDegreeVertex(){
        int ret=1;
        for (int i=2; i<=verticesNumber; i++) {
            if (container.getDegree(i)>container.getDegree(ret)) {
                ret=i;
            }
        }
        return ret;
     }
     int getMostRestrictedVertex(){
        int ret = 1;
        int maxRest = 0;
        vector<int> colorVector;
        for (int i = 1; i <=container.getDegree(1); ++i)
        {
            bool temp=true;
            for (int j=0; j<colorVector.size(); j++) {
                if (color[container.getNeighbor(1,i)-1]==colorVector[j]) {
                    temp=false;
                }
            }
            if (temp) {
                maxRest+=1;
                colorVector.push_back(color[container.getNeighbor(1,i)-1]);
            }
        }
        if (color[0] != 0)
        {
            maxRest = 0;
        }
        for (int i = 0; i < verticesNumber; ++i){
            colorVector.clear();
            int tempRest;
            int neighborColor;
            for (int j = 0; j < container.getDegree(i+1); ++j)
            {
                neighborColor = color[container.getNeighbor(i+1,j+1)-1];;
                bool temp=true;
                for (int k=0; k<colorVector.size(); k++) {
                    if (neighborColor ==colorVector[k]) {
                        temp=false;
                    }
                }
                if (temp) {
                    tempRest+=1;
                    colorVector.push_back(neighborColor);
                }
            }
            if (tempRest>=maxRest) {
                if((tempRest!=maxRest || container.getDegree(i+1)>container.getDegree(ret)) && color[i] == 0) {
                        ret=i+1;
                        maxRest=tempRest;
                }
            }
        }
        return ret;
     }
     int colorGraph(bool prob = false, int i = 0) {
        int s_vertex;
        if(prob=false){
            s_vertex = getMaxDegreeVertex();
        }
        else{
            s_vertex = i;
        }
        int cNumber = 1;
        color.clear();
        for (int i=0; i<verticesNumber; i++) {
            color.push_back(0);
        }
        color[s_vertex-1] = 1;
        vector <int> colorVector;
        for (int i = 1; i < verticesNumber; ++i){
            int mostRest = getMostRestrictedVertex();
            int mincolor = 1;
            colorVector.clear();
            int tempRest;
            for (int j = 0; j < container.getDegree(mostRest); ++j)
            {
                bool temp=true;
                int neighborColor = color[container.getNeighbor(mostRest,j+1)-1];
                for (int k=0; k<colorVector.size(); k++) {
                    if (neighborColor ==colorVector[k]) {
                        temp=false;
                    }
                }
                if (temp) {
                    colorVector.push_back(neighborColor);
                }
            }
            bool stop;
            while (true) {
                stop = true;
                for (int i = 0; i < colorVector.size();++i){
                    if(mincolor == colorVector[i]){
                        mincolor += 1;
                        stop =  false;
                        break;
                    }
                }
                if(stop){
                    break;
                }
            }
             color[mostRest-1] = mincolor;
             if (mincolor > cNumber)
             {
                cNumber = mincolor;
             }
     }
     return cNumber;
    }
    int probColor(int sample){
        int colorsNumber = colorGraph(false,getMaxDegreeVertex());
        int ncolor;
        for(int i = 1; i < sample; i++){
            ncolor = colorGraph(true,i);
            if(colorsNumber > ncolor){
                colorsNumber = ncolor;
            }
        }
        return colorsNumber;
    }
};
int main(){
    setlocale(LC_ALL, "Portuguese");
    ofstream myFile;
    myFile.open("Coloracao.txt");
    int hard_test = 5;
    bool prob_test = true;
    static const char* const grafos[20] = {"G1.txt", "G2.txt","G3.txt","G4.txt","G5.txt","G6.txt","G7.txt","G8.txt","G9.txt","G10.txt","G11.txt","G12.txt","G13.txt","G14.txt","G15.txt","G16.txt","G17.txt","G18.txt","G19.txt","G20.txt"};
    for (int i = 14; i < 16; i++){
        cout << grafos[i] << endl;
        myFile  << grafos[i] << "| ";
        Graph<adjacency_vector> teste(grafos[i]);
        myFile  << "vértices " << teste.verticesNumber << "| ";
         myFile  <<  "arestas " << teste.edgesNumber << "\n";
        std::clock_t start;
        double duration;
        if(teste.verticesNumber < 200 && prob_test == true){
            start = std::clock();
            teste.colorGraph(false,teste.getMaxDegreeVertex());
            duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
            myFile  << "Menor cor: "<<teste.probColor(teste.verticesNumber) << "\n";
            myFile<< "Tempo de Execução: " << duration << "\n";
        }
        if(teste.verticesNumber > 200 && teste.verticesNumber < 600 && prob_test == true){
            cout << "oi" << endl;
            start = std::clock();
            teste.colorGraph(false,teste.getMaxDegreeVertex());
            duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
            myFile  << "Menor cor: "<<teste.probColor(hard_test) << "\n";
            myFile<< "Tempo de Execução: " << duration << "\n";
        }
        if((teste.verticesNumber > 600 && teste.edgesNumber < 90000) || prob_test == false){
            start = std::clock();   
            duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
            myFile << "Menor cor: "<< teste.probColor(hard_test) << "\n";
            duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
            myFile << "Tempo de Execução: " << duration << "\n";
        }
        else{
            start = std::clock();   
            duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
            myFile << "Menor cor: "<< teste.colorGraph(false,1) << "\n";
            duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
            myFile << "Tempo de Execução: " << duration << "\n";
        }
    }
    myFile.close();
    return 0;
}
