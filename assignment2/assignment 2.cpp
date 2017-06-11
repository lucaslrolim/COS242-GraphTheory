//#include "containers.h"
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
#include <utility>
#include "myHeap.h"
using namespace std;

class adjacency_matrix {
private:
	int order;
	int *degrees;
	bool **myArray;
public:
	adjacency_matrix(){

	}
	~adjacency_matrix() {
		for (int i=0; i<order; i++){
			delete [] myArray[i];
		}
		delete [] myArray;
		delete [] degrees;
	}
	void fill(int v_number) {
		order=v_number;
		degrees=new int[order];
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
	int getNeighbor(int vertex, int neighborOrder){
		int neighborNumber=1;
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
	void insertEdge(int r, int c) {
        myArray[r-1][c-1] = 1;
        myArray[c-1][r-1] = 1;
        degrees[r-1]+=1;
        degrees[c-1]+=1;
	}
	void show(){
		for (int i = 0; i < order; i++){
            for (int j = 0; j < order; j++){
                cout << myArray[i][j] << " ";
            }
            cout << endl;
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
	bool comp(int i, int j){
		return i<j;
	}
public:
	adjacency_vector(){

	}
	~adjacency_vector(){
		delete [] myArray;
	}
	void fill(int v_number) {
		size=v_number;
		myArray=new vector<int> [size];
		for (int i=0; i<size; i++){
			vector<int> v;
			myArray[i]=v;
		}
	}
	void insertEdge(int v1, int v2) {
        myArray[v1 - 1].push_back(v2);
        myArray[v2 - 1].push_back(v1);
	}
	int getNeighbor(int vertex, int neighborOrder){
		return myArray[vertex-1][neighborOrder-1];
	}
	void show() {
		for (int i = 0; i<size; i++){
            for (int j = 0; j<myArray[i].size(); j++){
                cout << myArray[i][j] << " ";
            }
            cout << endl;
        }
	}
	int getDegree(int vertex){
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
	map <pair<int ,int>, float> weight;
	map <int, string>  vertex_name;
	vector <float> min_dist;
	vector <int> parent;
	int factorial(int n) {
		int ret=1;
		for (int i=1; i<=n; i++) {
			ret*=i;
		}
		return ret;
	}
	void insertWeight(int a, int b, float w){
		pair<int, int> edge(max(a,b),min(a,b)); 
		weight[edge]= w;
	}
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
		container.fill(verticesNumber);
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
				insertWeight(a,b,w);
			}
			container.insertEdge(a,b);
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
		vector<int> level(verticesNumber);
		parent.clear();
		for (int i=0; i<verticesNumber; i++){
			marking[i]=0;
			min_dist.push_back(INT_MAX);
			level[i]=-1;
			parent.push_back(-1);
		}
		queue<int> vqueue;
		marking[s_vertex-1]=1;
		parent[s_vertex-1]=0;
		level[s_vertex-1]=0;
		min_dist[s_vertex-1] = 0;
		vqueue.push(s_vertex);
		while(!vqueue.empty()){
			int v=vqueue.front();
			vqueue.pop();
			for (int i=0; i<container.getDegree(v); i++){
				int x=container.getNeighbor(v,i+1);
				if (marking[x-1]==0){
					marking[x-1]=1;
					level[x-1]=level[v-1]+1;
					parent[x-1]=v;
					vqueue.push(x);
					min_dist[x-1] = level[x-1];
				}
			}
		}
		if (createFile){
			ofstream myFile;
			myFile.open("spanning_tree.txt");
			myFile << "Vertice Pai Nivel\n";
			for (int i=0; i<verticesNumber; i++){
				myFile << i+1 << " " << parent[i] << " " << level[i];
				if (i<verticesNumber-1){
					myFile << "\n";
				}
			}
			myFile.close();
		}
	}
	void dfs(int s_vertex, bool createFile=1){
		vector<bool> marking(verticesNumber);
		vector<int> level(verticesNumber);
		parent.clear();
		for (int i=0; i<verticesNumber; i++){
			marking[i]=0;
			level[i]=-1;
			parent.push_back(-1);
		}
		stack<int> vstack;
		parent[s_vertex-1]=0;
		level[s_vertex-1]=0;
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
						level[x-1]=level[u-1]+1;
					}
				}
			}
		}
		if (createFile){
			ofstream myFile;
			myFile.open("spanning_tree.txt");
			myFile << "Vertice Pai Nivel\n";
			for (int i=0; i<verticesNumber; i++){
				myFile << i+1 << " " << parent[i] << " " << level[i];
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
		int smallestComponent=INT_MAX;
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
		for (int vt=0; vt<verticesNumber; vt++){
			vector<bool> marking(verticesNumber);
			vector<int> level(verticesNumber);
			for (int i=0; i<verticesNumber; i++){
	    		level[i]=0;
	    		marking[i]=0;
	    	}
    		queue<int> vqueue;
			marking[vt]=1;
			vqueue.push(vt+1);
			while(!vqueue.empty()){
				int v=vqueue.front();
				vqueue.pop();
				if (level[v-1]>ret){
    				ret=level[v-1];
    			}
				for (int i=0; i<container.getDegree(v); i++){
					int x=container.getNeighbor(v,i+1);
					if (marking[x-1]==0){
						marking[x-1]=1;
						level[x-1]=level[v-1]+1;
						vqueue.push(x);
					}
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
			min_dist.push_back(INT_MAX);
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
				pair<int, int> tempEdge(max(u,v),min(u,v));
				if (min_dist[v-1] > min_dist[u-1] + weight[tempEdge]){
						min_dist[v-1] = min_dist[u-1] + weight[tempEdge];
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
				if (min_dist[j]!=INT_MAX)
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
		vector<int> parent(verticesNumber);
		vector<bool> marking(verticesNumber);
		min_dist.clear();
		for(int i = 0; i < verticesNumber;i++){
				min_dist.push_back(INT_MAX);
				parent[i]=-1;
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
				pair<int, int> tempEdge(max(u,v),min(u,v));
				if (min_dist[v-1] > weight[tempEdge] && marking[v-1] == 0){
						parent[v-1] = u;
						min_dist[v-1] = weight[tempEdge];
						heap.update(v,min_dist[v-1]);
				}	
			}
		}
		ofstream myFile;
		myFile.open("mst.txt");
		myFile << verticesNumber << "\n";
		for (int i = 0; i < parent.size();i++) {
		//	cout << parent[i] << " ";
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
	 		if(min_dist[t_vertex-1] == INT_MAX){
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
	 void search(const char* person1, const char* person2){
	 	ifstream myNetMap;
		myNetMap.open("rede_colaboracao_vertices.txt");
		stringstream iss;
		string line;
		string line2;
		string name[2];
		vector <int> path(2);
		while(getline(myNetMap,line,'\n')) {
		 	iss << line;
		 	int i = 0;
		 	while(getline(iss,line2,',')){
		 		name[i] = line2;
		 		i += 1;
		 	}
		 	vertex_name[atoi(name[0].c_str())]=name[1];
		 	iss.clear();
		 	if (string(name[1])== string(person1)) {
		 		path[0]=atoi(name[0].c_str());
		 	}
		 	else if (string(name[1]) == string(person2)) {
		 		path[1]=atoi(name[0].c_str());
		 	}
		}
		myNetMap.close();
		dijkstra(path[0]);
		vector<int> spath;
		int currentVertex=path[1];
		cout << currentVertex << endl;
		while (currentVertex!=0) {
			spath.push_back(currentVertex);
			currentVertex=parent[currentVertex-1];		
		}
		for (int i=spath.size()-1; i>=0; i--) {
			cout << vertex_name[spath[i]] << endl;
		}
	 }
	void printNeighbors(const char * pName) {
		ifstream myNetMap;
		myNetMap.open("rede_colaboracao_vertices.txt");
		stringstream iss;
		string line;
		string line2;
		string name[2];
		int path;
		bool empty=vertex_name.empty();
		while(getline(myNetMap,line,'\n')) {
		 	iss << line;
		 	int i = 0;
		 	while(getline(iss,line2,',')){
		 		name[i] = line2;
		 		i += 1;
		 	}
		 	if (string(name[1])== string(pName)) {
	 			path=atoi(name[0].c_str());
	 		}
	 		if (empty) {
		 		vertex_name[atoi(name[0].c_str())]=name[1];
		 	}
		 	iss.clear();
		}
		myNetMap.close();
		for (int i=0; i<container.getDegree(path); i++) {
			cout << vertex_name[container.getNeighbor(path,i+1)] << endl;
		}
	}
	void greatestDegrees() {
		int first=0, second=0, third=0;
		bool empty=vertex_name.empty();
	 	if (empty) {
	 		ifstream myNetMap;
			myNetMap.open("rede_colaboracao_vertices.txt");
			string line;
			string line2;
			stringstream iss;
			string name[2];
			while(getline(myNetMap,line,'\n')) {
			 	iss << line;
			 	int i = 0;
			 	while(getline(iss,line2,',')){
			 		name[i] = line2;
			 		i += 1;
			 	}
			 	vertex_name[atoi(name[0].c_str())]=name[1];
			 	iss.clear();
			}
			myNetMap.close();
		}
		for (int i=1; i<=verticesNumber; i++) {
			if (container.getDegree(i)>container.getDegree(first)) {
				third=second;
				second=first;
				first=i;
			}
			else if (container.getDegree(i)>container.getDegree(second)) {
				third=second;
				second=i;
			}
			else if (container.getDegree(i)>container.getDegree(third)) {
				third=i;
			}
		}
		cout << vertex_name[first] << " " << container.getDegree(first) << endl;
		cout << vertex_name[second] << " " << container.getDegree(second) << endl;
		cout << vertex_name[third] << " " << container.getDegree(third) << endl;
		
	}
};
int main(){
setlocale(LC_ALL, "Portuguese");
	setlocale(LC_ALL, "Portuguese");
	//Graph<adjacency_vector> teste("rede_colaboracao.txt");
	Graph<adjacency_vector> teste("grafo_2.txt");
	std::clock_t start;
    double duration;
    start = std::clock();
    cout << "Distância média: "<< teste.mean_dist() << endl;
	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	cout << "Tempo de Execução: " << duration << endl;
	return 0;
}
