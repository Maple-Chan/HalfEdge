#include "prefix.h"
#include "He_edge.h"


//类内方法实现
/*建立实体的函数：
	拥有一组点集、面集

*/
void Solid::make_Solid()
{
	readFile Solid_5obj; //完成文件读入，拥有点与面的数组
	this->fnum = Solid_5obj.Fnum;
	this->vnum = Solid_5obj.Vnum;

	//-初始化点集-----------Start
	vertexs_list = new Vertex[vnum];
	for (int i = 0; i < vnum; ++i)
	{
		vertexs_list[i].id_v = i;
		float x = Solid_5obj.fverts[i].x;
		float y = Solid_5obj.fverts[i].y;
		float z = Solid_5obj.fverts[i].z;
		//cout << "v:" << i << ": x:" << x<<" y:" << y <<" z:"<< z << endl;
		vertexs_list[i].setCondition(x,y,z);
	}
	//-初始化点集-----------End

	//初始化边集


	//-初始化面---------Start
	load_Face(Solid_5obj, fnum);
	//-初始化面---------End


	//-初始化半边--------Start
	//初步思路，将一个环中的半边作为不连续的空间，不同环之间用数组来检索
	
	for (int i = 0; i < fnum; ++i)
	{
		//1.***
		HalfEdge *newHalfEdge = new HalfEdge;
		HalfEdge * HalfCur1 = newHalfEdge;//申请一个新的节点

		HalfCur1->setStartV((faces_list+i)->e1->vertex1);//设置半边列表的头结点的起点
		HalfCur1->setEndV((faces_list + i)->e1->vertex2);//设置终点
		HalfCur1->setEdge((faces_list + i)->e1);
		//HalfCur1->setBrother(); ？？？Pair边应该再所有的HalfEdge实例化完了再匹配？？？
		(faces_list + i)->e1->inhalfedge = HalfCur1;

		
		//2.***
		newHalfEdge = new HalfEdge;
		HalfEdge *HalfCur2 = newHalfEdge;

		HalfCur2->setStartV((faces_list+i)->e2->vertex1);
		HalfCur2->setEndV((faces_list + i)->e2->vertex2);
		HalfCur2->setEdge((faces_list + i)->e2);

		(faces_list + i)->e2->inhalfedge = HalfCur2;


		//3.***
		newHalfEdge = new HalfEdge;
		HalfEdge *HalfCur3 = newHalfEdge;

		HalfCur3->setStartV((faces_list+i)->e3->vertex1);
		HalfCur3->setEndV((faces_list + i)->e3->vertex2);
		HalfCur3->setEdge((faces_list + i)->e3);

		(faces_list + i)->e2->inhalfedge = HalfCur3;

		//构建半边的链接关系
		HalfCur1->next = HalfCur2;
		HalfCur2->next = HalfCur3;
		HalfCur3->next = HalfCur1;

		HalfCur1->pre = HalfCur3;
		HalfCur3->pre = HalfCur2;
		HalfCur2->pre = HalfCur1;

		halfedges_list.push_back(HalfCur1);
		halfedges_list.push_back(HalfCur2);
		halfedges_list.push_back(HalfCur3);
	}
	
	//为半边匹配对边://HalfCur3->setBrother();
	//思路：遍历HalfEdge ，查找对应，使对称两边标志置isPaired = 1，如果遍历到已经匹配那么跳过
	
	//===================================
	FindBro(halfedges_list,fnum); 
	//这个函数中存在this = nullptr的问题

	//-初始化半边--------End
	
	//完善半边数据结构中额外需要的指针，eg1:点指向边
	//...

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
void Solid::FindBro(vector<HalfEdge*>& list,int fn)
{//找到半边对应的半边
	Edge* e;
	HalfEdge* Ptr;
	HalfEdge* pair;
	
	for (int i = 0; i < fnum * 3; ++i)
	{
		Ptr = list[i];
		
		//一个数组元素存在三个半边
		if (Ptr->getPair() == NOTPAIRED)
		{
			e = Ptr->getEdge();
			Ptr->setPair();//Pair是判断是否已经有对边的
			pair = findPair(e, fn, list);  
			//cout <<i <<"：Ptr ";
			Ptr->setBrother(pair);//Bro是为了设置对边
			if (pair) {
				//cout << "pair"<<pair<<endl;
				pair->setBrother(Ptr);
				pair->setPair();
			}
			
		}
	
	}

}


int Solid::load_Face(readFile& file, int fn)
{

	return 0;
}

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