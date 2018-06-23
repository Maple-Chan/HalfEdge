#include "prefix.h"
#include "He_edge.h"


//类内方法实现
/*建立实体的函数：
	拥有一组点集、面集

*/
int findExistEdge(Edge e, vector<Edge> list);

void Solid::make_Solid()
{
	readFile Solid_5obj; //完成文件读入，拥有点与面的数组
	this->fnum = Solid_5obj.Fnum;
	this->vnum = Solid_5obj.Vnum;

	load_Vertex(Solid_5obj,vnum);//初始化点集

	load_Edges(Solid_5obj);//初始化边集

	load_Face(Solid_5obj, fnum);//-初始化面---------End


	//debug------------
	/*for (int i = 0; i < vnum; ++i)
	{
		cout << vertexs_list[i].getX() << " " 
			<< vertexs_list[i].getY() << " " 
			<< vertexs_list[i].getZ() << endl;
	}
	for (int i = 0; i < fnum; ++i)
	{
		cout << faces_list[i].e1->vertex1->id_v << " "
			<< faces_list[i].e2->vertex1->id_v << " "
			<< faces_list[i].e3->vertex1->id_v << endl;
	}*/

	//-初始化半边--------Start
	load_Halfedge();
	//为半边匹配对边://HalfCur3->setBrother();

	//思路：遍历HalfEdge ，查找对应，使对称两边标志置isPaired = 1，如果遍历到已经匹配那么跳过
	
	//===================================


}
int Solid::load_Halfedge()
{
	halfedges_list.resize(3*fnum);
	for (int i = 0; i < fnum; ++i)
	{
		faces_list[i];
	}
}
int Solid::load_Vertex(readFile& file, int vn)
{
	vertexs_list.resize(vn);
	for (int i = 0; i < vnum; ++i)
	{
		vertexs_list[i].id_v = i;//0  -> (vnum-1)
		float x = file.fverts[i].x;
		float y = file.fverts[i].y;
		float z = file.fverts[i].z;
		//cout << "v:" << i << ": x:" << x<<" y:" << y <<" z:"<< z << endl;
		vertexs_list[i].setCondition(x, y, z);
	}
	return 0;
}
int Solid::load_Edges(readFile& file)
{/*建立边，并赋值对应的两个点，存入边向量中*/

	Edge e1;
	Edge e2;
	Edge e3;
	for (int i = 0; i < fnum; ++i)
	{
		file.ffaces[i].v[0];//构建面的三个点
		e1.faceID = i;

		Vertex& v1 = vertexs_list[file.ffaces[i].v[0]];
		Vertex& v2 = vertexs_list[file.ffaces[i].v[1]];
		Vertex& v3 = vertexs_list[file.ffaces[i].v[2]];

		e1.vertex1 = &v1;
		e1.vertex2 = &v2;
		e2.vertex1 = &v2;
		e2.vertex2 = &v3;
		e3.vertex1 = &v3;
		e3.vertex2 = &v1;

		//==
		e1.next = &e2;
		e2.next = &e3;
		e3.next = &e1;
		e1.pre = &e3;
		e2.pre = &e1;
		e3.pre = &e2;

		//if(findExistEdge(e1,edges_list)==0)//保证不会有重复边
			edges_list.push_back(e1);
		//if (findExistEdge(e2, edges_list) == 0)
			edges_list.push_back(e2);
		//if (findExistEdge(e3, edges_list) == 0)
			edges_list.push_back(e3);
	}
	return 0;
}
int findExistEdge(Edge e,vector<Edge> list)
{
	for (int i = 0; i < list.size(); ++i)
	{
		if ((e.vertex1->id_v == list[i].vertex1->id_v && e.vertex2->id_v == list[i].vertex2->id_v)||
			(e.vertex2->id_v == list[i].vertex1->id_v && e.vertex1->id_v == list[i].vertex2->id_v))
		{
			return 1;
		}
	}
	return 0;
}

int Solid::load_Face(readFile& file, int fn)
{

	Face ftemp;
	faces_list.resize(fnum);
	for (int i = 0; i < fnum; ++i)
	{
		int n1, n2, n3;

			n1 = findEdge(vertexs_list[file.ffaces[i].v[0]], vertexs_list[file.ffaces[i].v[1]]);


			n2 = findEdge(vertexs_list[file.ffaces[i].v[1]], vertexs_list[file.ffaces[i].v[2]]);
	

			n3 = findEdge(vertexs_list[file.ffaces[i].v[2]], vertexs_list[file.ffaces[i].v[0]]);
		
		if (n1 == -1 || n2 == -1 || n3 == -1)continue;

		faces_list[i].id = i;
		faces_list[i].e1 = &edges_list[n1];
		faces_list[i].e2 = &edges_list[n2];
		faces_list[i].e3 = &edges_list[n3];

	}

	return 0;
}
int Solid::findEdge(Vertex v1,Vertex v2)
{
	for (int i = 0; i < edges_list.size(); ++i)
	{
		if ((v1.id_v == edges_list[i].vertex1->id_v && v2.id_v == edges_list[i].vertex2->id_v))
		{
			return i;
		}
	}
	return -1;
}


HalfEdge* findPair(Edge* e,int fn, vector<HalfEdge*> list) 
{
	HalfEdge* Ptr;
	for (int i = 0; i < fn; ++i)
	{
		Ptr = list[i];
		if (Ptr->getPair()==NOTPAIRED && Ptr->getEdge()->vertex1 == e->vertex2 && Ptr->getEdge()->vertex2 == e->vertex1)
		{
			return Ptr;
		}
		Ptr = Ptr->next;
		if (Ptr->getPair() == NOTPAIRED && Ptr->getEdge()->vertex1 == e->vertex2 && Ptr->getEdge()->vertex2 == e->vertex1)
		{
			return Ptr;
		}		
		Ptr = Ptr->next;
		if (Ptr->getPair() == NOTPAIRED && Ptr->getEdge()->vertex1 == e->vertex2 && Ptr->getEdge()->vertex2 == e->vertex1)
		{
			return Ptr;
		}
		
	}
	return NULL;
}
/*
函数名：FindBro
	参数：			list 半边集、fn 面的个数
	内部变量：		e，遍历到当前的边
					Ptr，当前指针指向的半边
					Pair，当前边对应的半边
	调用函数：		getPair(),判断当前边有没有设置对边
					setPair(),设置当前面已经配对了
					setBrother(),设置当前边的对边

	[Date：5/4]问题：		setBrother()函数中存在了this指针为nullptr的情况

*/



Solid :: ~Solid() {

}
void Solid::draw()
{
	/*unsigned int findex;
	//glBegin(GL_QUADS);
	glBegin(GL_LINES);
	//对点面进行连接
	for (findex = 0; findex < (size_t)fnum; ++findex)
	{
		if (faces_list[findex].e1->vertex1->id_v == 0 ||
			faces_list[findex].e1->vertex1->id_v == 1 ||
			faces_list[findex].e1->vertex1->id_v == 23 ||
			faces_list[findex].e1->vertex1->id_v == 24 ||
			faces_list[findex].e1->vertex1->id_v == 47 ||
			faces_list[findex].e1->vertex1->id_v == 48)
			glColor3f(0.0, 0.0, 1.0);
		else
			glColor3f(1.0, 0.0, 0.0);

		glVertex3f(faces_list[findex].e1->vertex1->getX(), faces_list[findex].e1->vertex1->getY(), faces_list[findex].e1->vertex1->getZ());
		//cout <<findex<<"： "<< " e1x: " << faces_list[findex].e1->vertex1->getX() << " e1y: " << faces_list[findex].e1->vertex1->getY() << " e1z: " << faces_list[findex].e1->vertex1->getZ() << endl;
		//glColor3f(1.0, 0.0, 0.0);
		glVertex3f(faces_list[findex].e1->vertex2->getX(), faces_list[findex].e2->vertex1->getY(), faces_list[findex].e2->vertex1->getZ());
		//cout <<findex<<"： "<< " e1x: " << faces_list[findex].e1->vertex1->getX() << " e1y: " << faces_list[findex].e1->vertex1->getY() << " e1z: " << faces_list[findex].e1->vertex1->getZ() << endl;
		
		
		//glColor3f(1.0, 0.0, 0.0);
		glVertex3f(faces_list[findex].e2->vertex1->getX(), faces_list[findex].e2->vertex1->getY(), faces_list[findex].e2->vertex1->getZ());
		//cout << findex << "： " << " e1x: " << faces_list[findex].e2->vertex1->getX() << " e1y: " << faces_list[findex].e2->vertex1->getY() << " e1z: " << faces_list[findex].e2->vertex1->getZ() << endl;
		//glColor3f(1.0, 0.0, 0.0);
		glVertex3f(faces_list[findex].e2->vertex2->getX(), faces_list[findex].e2->vertex2->getY(), faces_list[findex].e2->vertex2->getZ());
		//cout << findex << "： " << " e1x: " << faces_list[findex].e2->vertex1->getX() << " e1y: " << faces_list[findex].e2->vertex1->getY() << " e1z: " << faces_list[findex].e2->vertex1->getZ() << endl;
		
		
		//glColor3f(1.0, 0.0, 0.0);
		glVertex3f(faces_list[findex].e3->vertex2->getX(), faces_list[findex].e3->vertex2->getY(), faces_list[findex].e3->vertex2->getZ());
		//cout << findex << "： " << " e1x: " << faces_list[findex].e3->vertex1->getX() << " e1y: " << faces_list[findex].e3->vertex1->getY() << " e1z: " << faces_list[findex].e3->vertex1->getZ() << endl;
		//glColor3f(1.0, 0.0, 0.0);
		glVertex3f(faces_list[findex].e3->vertex2->getX(), faces_list[findex].e3->vertex2->getY(), faces_list[findex].e3->vertex2->getZ());
		//cout << findex << "： " << " e1x: " << faces_list[findex].e3->vertex1->getX() << " e1y: " << faces_list[findex].e3->vertex1->getY() << " e1z: " << faces_list[findex].e3->vertex1->getZ() << endl;

	}

	
	glEnd();*/
}