#include "prefix.h"
#include "He_edge.h"


//���ڷ���ʵ��
/*����ʵ��ĺ�����
	ӵ��һ��㼯���漯

*/
int findExistEdge(Edge e, vector<Edge> list);

void Solid::make_Solid()
{
	readFile Solid_5obj; //����ļ����룬ӵ�е����������
	this->fnum = Solid_5obj.Fnum;
	this->vnum = Solid_5obj.Vnum;

	load_Vertex(Solid_5obj,vnum);//��ʼ���㼯

	load_Edges(Solid_5obj);//��ʼ���߼�

	load_Face(Solid_5obj, fnum);//-��ʼ����---------End


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

	//-��ʼ�����--------Start
	load_Halfedge();
	//Ϊ���ƥ��Ա�://HalfCur3->setBrother();

	//˼·������HalfEdge �����Ҷ�Ӧ��ʹ�Գ����߱�־��isPaired = 1������������Ѿ�ƥ����ô����
	
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
{/*�����ߣ�����ֵ��Ӧ�������㣬�����������*/

	Edge e1;
	Edge e2;
	Edge e3;
	for (int i = 0; i < fnum; ++i)
	{
		file.ffaces[i].v[0];//�������������
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

		//if(findExistEdge(e1,edges_list)==0)//��֤�������ظ���
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
��������FindBro
	������			list ��߼���fn ��ĸ���
	�ڲ�������		e����������ǰ�ı�
					Ptr����ǰָ��ָ��İ��
					Pair����ǰ�߶�Ӧ�İ��
	���ú�����		getPair(),�жϵ�ǰ����û�����öԱ�
					setPair(),���õ�ǰ���Ѿ������
					setBrother(),���õ�ǰ�ߵĶԱ�

	[Date��5/4]���⣺		setBrother()�����д�����thisָ��Ϊnullptr�����

*/



Solid :: ~Solid() {

}
void Solid::draw()
{
	/*unsigned int findex;
	//glBegin(GL_QUADS);
	glBegin(GL_LINES);
	//�Ե����������
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
		//cout <<findex<<"�� "<< " e1x: " << faces_list[findex].e1->vertex1->getX() << " e1y: " << faces_list[findex].e1->vertex1->getY() << " e1z: " << faces_list[findex].e1->vertex1->getZ() << endl;
		//glColor3f(1.0, 0.0, 0.0);
		glVertex3f(faces_list[findex].e1->vertex2->getX(), faces_list[findex].e2->vertex1->getY(), faces_list[findex].e2->vertex1->getZ());
		//cout <<findex<<"�� "<< " e1x: " << faces_list[findex].e1->vertex1->getX() << " e1y: " << faces_list[findex].e1->vertex1->getY() << " e1z: " << faces_list[findex].e1->vertex1->getZ() << endl;
		
		
		//glColor3f(1.0, 0.0, 0.0);
		glVertex3f(faces_list[findex].e2->vertex1->getX(), faces_list[findex].e2->vertex1->getY(), faces_list[findex].e2->vertex1->getZ());
		//cout << findex << "�� " << " e1x: " << faces_list[findex].e2->vertex1->getX() << " e1y: " << faces_list[findex].e2->vertex1->getY() << " e1z: " << faces_list[findex].e2->vertex1->getZ() << endl;
		//glColor3f(1.0, 0.0, 0.0);
		glVertex3f(faces_list[findex].e2->vertex2->getX(), faces_list[findex].e2->vertex2->getY(), faces_list[findex].e2->vertex2->getZ());
		//cout << findex << "�� " << " e1x: " << faces_list[findex].e2->vertex1->getX() << " e1y: " << faces_list[findex].e2->vertex1->getY() << " e1z: " << faces_list[findex].e2->vertex1->getZ() << endl;
		
		
		//glColor3f(1.0, 0.0, 0.0);
		glVertex3f(faces_list[findex].e3->vertex2->getX(), faces_list[findex].e3->vertex2->getY(), faces_list[findex].e3->vertex2->getZ());
		//cout << findex << "�� " << " e1x: " << faces_list[findex].e3->vertex1->getX() << " e1y: " << faces_list[findex].e3->vertex1->getY() << " e1z: " << faces_list[findex].e3->vertex1->getZ() << endl;
		//glColor3f(1.0, 0.0, 0.0);
		glVertex3f(faces_list[findex].e3->vertex2->getX(), faces_list[findex].e3->vertex2->getY(), faces_list[findex].e3->vertex2->getZ());
		//cout << findex << "�� " << " e1x: " << faces_list[findex].e3->vertex1->getX() << " e1y: " << faces_list[findex].e3->vertex1->getY() << " e1z: " << faces_list[findex].e3->vertex1->getZ() << endl;

	}

	
	glEnd();*/
}