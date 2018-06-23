#include "prefix.h"
#include "He_edge.h"


//���ڷ���ʵ��
/*����ʵ��ĺ�����
	ӵ��һ��㼯���漯

*/
void Solid::make_Solid()
{
	readFile Solid_5obj; //����ļ����룬ӵ�е����������
	this->fnum = Solid_5obj.Fnum;
	this->vnum = Solid_5obj.Vnum;

	//-��ʼ���㼯-----------Start
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
	//-��ʼ���㼯-----------End

	//��ʼ���߼�


	//-��ʼ����---------Start
	load_Face(Solid_5obj, fnum);
	//-��ʼ����---------End


	//-��ʼ�����--------Start
	//����˼·����һ�����еİ����Ϊ�������Ŀռ䣬��ͬ��֮��������������
	
	for (int i = 0; i < fnum; ++i)
	{
		//1.***
		HalfEdge *newHalfEdge = new HalfEdge;
		HalfEdge * HalfCur1 = newHalfEdge;//����һ���µĽڵ�

		HalfCur1->setStartV((faces_list+i)->e1->vertex1);//���ð���б��ͷ�������
		HalfCur1->setEndV((faces_list + i)->e1->vertex2);//�����յ�
		HalfCur1->setEdge((faces_list + i)->e1);
		//HalfCur1->setBrother(); ������Pair��Ӧ�������е�HalfEdgeʵ����������ƥ�䣿����
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

		//������ߵ����ӹ�ϵ
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
	
	//Ϊ���ƥ��Ա�://HalfCur3->setBrother();
	//˼·������HalfEdge �����Ҷ�Ӧ��ʹ�Գ����߱�־��isPaired = 1������������Ѿ�ƥ����ô����
	
	//===================================
	FindBro(halfedges_list,fnum); 
	//��������д���this = nullptr������

	//-��ʼ�����--------End
	
	//���ư�����ݽṹ�ж�����Ҫ��ָ�룬eg1:��ָ���
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
void Solid::FindBro(vector<HalfEdge*>& list,int fn)
{//�ҵ���߶�Ӧ�İ��
	Edge* e;
	HalfEdge* Ptr;
	HalfEdge* pair;
	
	for (int i = 0; i < fnum * 3; ++i)
	{
		Ptr = list[i];
		
		//һ������Ԫ�ش����������
		if (Ptr->getPair() == NOTPAIRED)
		{
			e = Ptr->getEdge();
			Ptr->setPair();//Pair���ж��Ƿ��Ѿ��жԱߵ�
			pair = findPair(e, fn, list);  
			//cout <<i <<"��Ptr ";
			Ptr->setBrother(pair);//Bro��Ϊ�����öԱ�
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