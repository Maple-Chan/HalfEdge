#include "LoopAlgorithm.h"



//����ʵ��
bool SetEvenVertex(Vertex v)
//short even; //1,2,3�ֱ��������ż�㣬������㣬δ����
{
	if (1 == isBelongEven(v))return true;
	else if (2 == isBelongEven(v))return false;
	else
	{//����δ���䣬������з���
		evenGroup.push_back(v);//v����ż�㼯����
		//��Ҫ��Ŀ�����ҵ�ÿ�����ڵ㣡����������
		Edge* e = v.edge;
		Face* f = e->face;
		HalfEdge* half = e->inhalfedge;

		half = half->getBrother();
		half = half->next;
		for (;;)
		{
			cout << half->getEndV()->id_v << endl;///
			oddGroup.push_back( *half->getEndV() );

			half = half->getBrother();
			half = half->next;
		}


		for (int i = 0; i < 6; ++i) 
		{//���ڵ������㼯��


			//��va����
			//SetEvenVertex(va)
		}

	}
}
short isBelongEven(Vertex v)
{
	short even=0; //1,2,3�ֱ��������ż�㣬������㣬δ����
	/*����even �� odd ������ ���޸�flag even*/
	for (size_t i = 0; i < evenGroup.size(); ++i)
	{
		if (v.id_v == evenGroup[i].id_v)even = 1;
	}
	for (size_t i = 0; i < oddGroup.size(); ++i)
	{
		if (v.id_v == oddGroup[i].id_v)even = 2;
	}

	if (1==even)return 1;
	else if(2==even)
	{
		return 2;
	}
	else return 3;
}
