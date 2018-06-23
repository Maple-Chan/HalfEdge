#pragma once
#ifndef HE_EDGE_H
#define HE_EDGE_H
#include <iostream>
#include <vector>
#include "ReadFile.h"
using namespace std;
#define PAIRED 1
#define NOTPAIRED 0
class Solid;
class Face;
class HalfEdge;
class Vertex;
class Edge;

typedef class Vertex
{
	float coordinate[3];//coordinate of the vertex (x, y, z)

public:
	int id_v;
	
	Vertex *next;
	Vertex *pre;
	Vertex() : id_v(0), next(NULL), pre(NULL)
	{
		coordinate[0] = 0.0f;
		coordinate[1] = 0.0f;
		coordinate[2] = 0.0f;
	}
	Vertex(int i) : id_v(i), next(NULL), pre(NULL)
	{
		coordinate[0] = 0.0f;
		coordinate[1] = 0.0f;
		coordinate[2] = 0.0f;
	}
	Vertex(float x, float y, float z) : id_v(0), next(NULL), pre(NULL)
	{
		coordinate[0] = x;
		coordinate[1] = y;
		coordinate[2] = z;
	}
	//以下定义对节点的操作：
	void setCondition(float x, float y, float z) {
		coordinate[0] = x;
		coordinate[1] = y;
		coordinate[2] = z;
	}
	void printCondition() {
		cout << "x：" << coordinate[0];
		cout << "y：" << coordinate[1];
		cout << "z：" << coordinate[2];
	}
	float getX() {return coordinate[0];}
	float getY() {return coordinate[1];}
	float getZ() {return coordinate[2];}

	Edge* edge;

}Vertex;


typedef class  Edge
{
public:
	Vertex* vertex1;
	Vertex* vertex2;
	//HalfEdge *half_l; //the edge's left halfedge
	//HalfEdge *half_r; //the edge's right halfedge
	Edge *pre;
	Edge *next;
	Face *face;
	int faceID;

	HalfEdge *inhalfedge;
	HalfEdge *outhalfedge;


	Edge() : vertex1(NULL), vertex2(NULL), next(NULL), pre(NULL) {}



}Edge;

class Face
{
public:
	int id;
	Edge * e1;
	Edge * e2;
	Edge * e3;

	Face *next;
	Face *pre;
	///int innum;//the count of inner loops
	Face() : id(0), e1(NULL), e2(NULL), e3(NULL), next(NULL), pre(NULL) {}

	Face(Edge * &e01, Edge * &e02, Edge * &e03) {
		id = 0;
		e1 = e01;
		e2 = e02;
		e3 = e03;
		e1->next = e2;
		e2->next = e3;
		e3->next = e1;
		e1->pre = e3;
		e2->pre = e1;
		e3->pre = e2;	
	}
};

class HalfEdge
{//Pair 表示时候匹配，Brother是对边有关的存放与索取
public:
	Edge *edge; //this halfedge belong to which edge
	Face *face;
	Vertex *sv; //the start vertex of this halfedge
	Vertex *ev; //the end vertex of this halfedge
	HalfEdge *brother;
	bool isPiared;



	HalfEdge *next;
	HalfEdge *pre;
	
	HalfEdge() : edge(NULL), sv(NULL), ev(NULL), next(NULL), pre(NULL), brother(NULL), isPiared(NOTPAIRED) {}
	void setStartV(Vertex *startV) {
		sv = startV;
	}
	void setEndV(Vertex *endV) {
		ev = endV;
	}
	Vertex * getStartV() {
		return sv;
	}
	Vertex * getEndV() {
		return ev;
	}
	void setEdge(Edge *Belong_edge) {
		edge = Belong_edge;
	}
	Edge* getEdge() {
		return this->edge;
	}
	void setBrother(HalfEdge* BroHalf) {
		/*static int ct = 0;
		ct++;
		cout <<"ct++ :"<<ct<<":"<< this << endl;
		//在ct == 156 的时候this==nullptr？？？？？
		*/
		brother = BroHalf;
	}
	HalfEdge* getBrother() {
		return brother;
	}


	void setPair() { 
		isPiared = PAIRED; 
	}
	int getPair() {
		return isPiared;
	}

};
class Solid
{
public:
	int id;
	vector<Vertex> vertexs_list;
	vector<Face> faces_list; // list of all faces to conclass this solid
	vector<Edge> edges_list; // list of all edges to conclass this solid->to build the frame

	vector<HalfEdge> halfedges_list;


	int vnum;//the count of all vertexs
	int fnum;//the count of all faces

	Solid() : id(0), faces_list(NULL), edges_list(NULL), halfedges_list(NULL), fnum(0), vnum(0) {}
	~Solid();
	void make_Solid();
	int load_Vertex(readFile & file, int fn);
	int load_Edges(readFile & file);
	int load_Face(readFile &file, int fn);
	int Solid::load_Halfedge();

	int Solid::findEdge(Vertex v1, Vertex v2);
	void FindBro(vector<HalfEdge>& list, int fn);//根据当前半边所属的边查找他的Pair
	void draw();

};




#endif // HE_EDGE_H*/