#include<cmath>
#include<vector>
#include <iostream>
using namespace std;
class Vertex {
public:
	int v_index;
	float v_distance;
	Vertex (int i, float d) {
		v_index=i;
		v_distance=d;
	}
	bool operator<(const Vertex & x) {
		return v_distance<x.v_distance;
	}
	bool operator>(const Vertex & x) {
		return v_distance>x.v_distance;
	}
	void operator=(const Vertex & x) {
		v_index=x.v_index;
		v_distance=x.v_distance;
	}
	friend ostream & operator <<(ostream & s, const Vertex & z) {
		s << z.v_index;
		return s;
	}
};

class myHeap {
public:
	vector<Vertex> vqueue;
	vector<int> index;
	myHeap (int n) {
		for (int i=0; i<n; i++) {
			index.push_back(-1);
		}
	}
	void push(Vertex v) {
		vqueue.push_back(v);
		int i=vqueue.size();
		index[v.v_index-1]=i-1;
		if (i!=1) {
			while (vqueue[i-1]<vqueue[floor(i/2)-1]) {
				Vertex temp=vqueue[floor(i/2)-1];
				vqueue[floor(i/2)-1]=vqueue[i-1];
				vqueue[i-1]=temp;
				index[vqueue[i-1].v_index-1]=i-1;
				index[vqueue[floor(i/2)-1].v_index-1]=floor(i/2)-1;
				i=floor(i/2);
				if (i==1) {break;}
			}
		}
//		for (int i=0; i<index.size(); i++) {
//			cout << index[i] << " ";
//		}
//		cout << endl;
//		cout << vqueue[index[4]].v_index << endl;
	}
	Vertex pop() {
		Vertex ret=vqueue[0];
		vqueue[0]=vqueue.back();
		vqueue.pop_back();
		int i=1;
		index[vqueue[0].v_index-1]=0;
		while ((2*i-1)<vqueue.size()) {
				Vertex temp=vqueue[i-1];
				if ((2*i)<vqueue.size()) {
					if (vqueue[2*i-1]<vqueue[2*i] && vqueue[i-1]>vqueue[2*i-1]) {
						vqueue[i-1]=vqueue[2*i-1];
						vqueue[2*i-1]=temp;
						index[vqueue[2*i-1].v_index-1]=2*i-1;
						index[vqueue[i-1].v_index-1]=i-1;
						i=2*i;
					}
					else if (vqueue[i-1]>vqueue[2*i]){
						vqueue[i-1]=vqueue[2*i];
						vqueue[2*i]=temp;
						index[vqueue[2*i].v_index-1]=2*i;
						index[vqueue[i-1].v_index-1]=i-1;
						i=2*i+1;
					}
					else {break;}
				}
				else {
					if (vqueue[i-1]>vqueue[2*i-1]) {
						vqueue[i-1]=vqueue[2*i-1];
						vqueue[2*i-1]=temp;
						index[vqueue[2*i-1].v_index-1]=2*i-1;
						index[vqueue[i-1].v_index-1]=i-1;
						i=2*i;
					}
					else {break;}
				}
				
			}
		return ret;
//		for (int i=0; i<index.size(); i++) {
//			cout << index[i] << " ";
//		}
//		cout << endl;
//		cout << vqueue[index[4]].v_index << endl;
	}
	void update(int v, float d) {
		int j=index[v-1];
		vqueue[j].v_distance=d;
		int i=j+1;
		if (vqueue[i-1]<vqueue[floor(i/2)-1]) {
			if (i!=1) {
				while (vqueue[i-1]<vqueue[floor(i/2)-1]) {
					Vertex temp=vqueue[floor(i/2)-1];
					vqueue[floor(i/2)-1]=vqueue[i-1];
					vqueue[i-1]=temp;
					index[vqueue[i-1].v_index-1]=i-1;
					index[vqueue[floor(i/2)-1].v_index-1]=floor(i/2)-1;
					i=floor(i/2);
					if (i==1) {break;}
				}
			}
		}
		else {
			while ((2*i-1)<vqueue.size()) {
				Vertex temp=vqueue[i-1];
				if ((2*i)<vqueue.size()) {
					if (vqueue[2*i-1]<vqueue[2*i] && vqueue[i-1]>vqueue[2*i-1]) {
						vqueue[i-1]=vqueue[2*i-1];
						vqueue[2*i-1]=temp;
						index[vqueue[2*i-1].v_index-1]=2*i-1;
						index[vqueue[i-1].v_index-1]=i-1;
						i=2*i;
					}
					else if (vqueue[i-1]>vqueue[2*i]){
						vqueue[i-1]=vqueue[2*i];
						vqueue[2*i]=temp;
						index[vqueue[2*i].v_index-1]=2*i;
						index[vqueue[i-1].v_index-1]=i-1;
						i=2*i+1;
					}
					else {break;}
				}
				else {
					if (vqueue[i-1]>vqueue[2*i-1]) {
						vqueue[i-1]=vqueue[2*i-1];
						vqueue[2*i-1]=temp;
						index[vqueue[2*i-1].v_index-1]=2*i-1;
						index[vqueue[i-1].v_index-1]=i-1;
						i=2*i;
					}
					else {break;}
				}
				
			}
		}
	}
	void show() {
		for (int i=0; i<vqueue.size(); i++) {
			cout << vqueue[i].v_index << " ";
		}
		cout << endl;
	}
	bool empty() {
		return vqueue.size()==0;
	}
};