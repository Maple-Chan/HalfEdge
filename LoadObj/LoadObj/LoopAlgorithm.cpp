#include "LoopAlgorithm.h"



//函数实现
bool SetEvenVertex(Vertex v)
//short even; //1,2,3分别代表属于偶点，属于奇点，未分配
{
	if (1 == isBelongEven(v))return true;
	else if (2 == isBelongEven(v))return false;
	else
	{//处于未分配，下面进行分配
		evenGroup.push_back(v);//v放入偶点集合中
		//重要的目标是找到每个相邻点！！！！！！
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
		{//相邻点放入奇点集中


			//找va相邻
			//SetEvenVertex(va)
		}

	}
}
short isBelongEven(Vertex v)
{
	short even=0; //1,2,3分别代表属于偶点，属于奇点，未分配
	/*遍历even 与 odd 数据组 ，修改flag even*/
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
