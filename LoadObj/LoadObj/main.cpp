#include "He_edge.h"
#include "prefix.h"
#include "LoopAlgorithm.h"
#include <vector>

//全局变量
float angle = 0.0;
Solid _50bj;
GLfloat sca[3] = { 0.5,0.5,0.5 };
vector<Vertex> evenGroup;
vector<Vertex> oddGroup;

//main中的函数
void init();
void keyboard(unsigned char key, int x, int y);
void idle();
void render();
void reshape(int w, int h);





int main(int argc, char * argv[])
{
	//建立物体

	_50bj.make_Solid();
	//下一步：4/24对建立的物体进行测试
	cout << "\n物体点的个数：" << _50bj.vnum << endl;
	cout << "物体面的个数：" << _50bj.fnum << endl;
	//测试如何看物体是否构建正确

	//写LOOP压缩算法
	//1.写定点分裂函数
	//		1)划分为奇点集和偶点集  TODO：5/28
	//2.奇点预测
	//		1)LOOP algorithm
	//		2)计算预测偏差
	//		3)删除奇点的信息  （奇异点需要保留，所以如果是就设为偶点）
	//3.重新生成网格






	//下面是OpenGL初始化函数============================================
	//this is init function
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(1024, 1024);
	
	glutCreateWindow("3D Tech- GLUT Tutorial");
	init();
	glutDisplayFunc(render);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	
	glutMainLoop();
	return 0;
}


/*方便显示的函数定义---------Start*/
void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-w/2,w/2,-h/2,h/2,-w/2,w/2);
	glOrtho(-2, 2, -2, 2, -2, 2);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void render()
{
	//cout << "render:" << endl;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	angle += 0.1;
	//angle = 90;///For CYF
	glRotatef(angle, 2.0, 0.5, 0.0);///For CYF
									//glRotatef(angle, 1.0, 1.0, 0.0);
									//glScalef(200,200,200);
									//glPolygonMode(GL_FRONT,GL_FILL);
									//user->draw();
	glScalef(sca[0], sca[1], sca[2]);
	_50bj.draw();


	glFlush();
	glutSwapBuffers();
}
void init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);


	////glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
}
void idle()
{
	glutPostRedisplay();
}
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q':
	case 'Q':
		exit(0);
		break;
	case 'r': case 'R':
		glutPostRedisplay();
		break;

	}
}
/*方便显示的函数定义---------Ended*/


//=============================================
/*for (int i = 0; i < _50bj.fnum; ++i)
{//测试：半边匹配完成
cout << "Origin: ";
_50bj.halfedges_list[i]->getStartV()->printCondition(); cout << endl;
cout  << "Origin: ";
_50bj.halfedges_list[i]->getEndV()->printCondition();; cout << endl;

cout << "Brother: ";
_50bj.halfedges_list[i]->getBrother()->getStartV()->printCondition();; cout << endl;
cout << "Brother: ";
_50bj.halfedges_list[i]->getBrother()->getEndV()->printCondition();; cout << endl;
}*/

/*
bool buildSolid(Solid newSolid)
{
	int veNum = fileobj.Vnum;
	int feNum = fileobj.Fnum;
	//初始化物体之前的准备
	//点集的构建
	Vertex *verts = new Vertex [veNum];
	for (int i = 0; i < veNum; ++i)
	{
		verts[i].id = i;
		verts[i].coordinate[0] = fileobj.fverts[i].x;
		verts[i].coordinate[1] = fileobj.fverts[i].y;
		verts[i].coordinate[2] = fileobj.fverts[i].z;
		if(i != 0)
			verts[i].pre = verts + i - 1;//指向上一个节点,如果是第一个节点那么头结点上节点为pre
		else {
			verts[i].pre = verts + veNum - 1;
		}
		verts[i].next = verts + i + 1;//指向下一个节点
	}

	//
	//半边集的建立；
	HalfEdge *halfedges = new HalfEdge[3 * feNum * 2];
	Face * faces = new Face[feNum];
	Edge * edges = new Edge[3 * feNum];
	for (int i = 0; i < feNum; ++i)
	{//按面的顺序构建半边，一个面三条边，六条半边，由于是三维立体图像所以不存在单个半边的情况
		//feNum+1的时候 应该多6个半边，在面的坐标下  +0...+5
		halfedges[i].sv = verts + fileobj.ffaces[i].v[0];
		halfedges[i].ev= verts + fileobj.ffaces[i].v[1];
		halfedges[i + 1].sv = verts + fileobj.ffaces[i].v[1];
		halfedges[i + 1].ev = verts + fileobj.ffaces[i].v[2];
		halfedges[i + 2].sv = verts + fileobj.ffaces[i].v[2];
		halfedges[i + 2].ev = verts + fileobj.ffaces[i].v[0];
	//反向半边
		halfedges[i + 3].sv = verts + fileobj.ffaces[i].v[1];
		halfedges[i + 3].ev = verts + fileobj.ffaces[i].v[0];
		halfedges[i + 1 + 3].sv = verts + fileobj.ffaces[i].v[2];
		halfedges[i + 1 + 3].ev = verts + fileobj.ffaces[i].v[1];
		halfedges[i + 2 + 3].sv = verts + fileobj.ffaces[i].v[0];
		halfedges[i + 2 + 3].ev = verts + fileobj.ffaces[i].v[2];
	//临边指针
		//前pre
		halfedges[i].pre = halfedges + i + 5;
		halfedges[i + 1].pre = halfedges + i;
		halfedges[i + 2].pre = halfedges + i + 1;
		halfedges[i + 3].pre = halfedges + i + 2;
		halfedges[i + 4].pre = halfedges + i + 3;
		halfedges[i + 5].pre = halfedges + i + 4;
		//后next
		halfedges[i].next = halfedges + i + 1;
		halfedges[i + 1].next = halfedges + i + 2;
		halfedges[i + 2].next = halfedges + i + 3;
		halfedges[i + 3].next = halfedges + i + 4;
		halfedges[i + 4].next = halfedges + i + 5;
		halfedges[i + 5].next = halfedges + i;
	//对边指针
		halfedges[i].brother = halfedges + i + 3;
		halfedges[i + 1].brother = halfedges + i + 4;
		halfedges[i + 2].brother = halfedges + i + 5;
		halfedges[i + 3].brother = halfedges + i;
		halfedges[i + 4].brother = halfedges + i + 1;
		halfedges[i + 5].brother = halfedges + i + 2;

	//构建edge:
		edges[i].half_l = halfedges + i;
		edges[i].half_r = halfedges + i + 3;
		edges[i].pre = edges + i + 2;//
		edges[i].next = edges + i + 1;
		edges[i + 1].half_l = halfedges + i +1;
		edges[i + 1].half_r = halfedges + i + 4;
		edges[i + 1].pre = edges + i ;
		edges[i + 1].next = edges + i + 2;
		edges[i + 2].half_l = halfedges + i + 2;
		edges[i + 2].half_r = halfedges + i + 5;
		edges[i + 2].pre = edges + i + 1;
		edges[i + 2].next = edges + i;
	//构建Loop：一个Loop由三条边组成


		
	//构建face
		faces[i].id = i;


		if (i == 0)
			faces[i].pre = faces + i + feNum - 1;
		else {
			faces[i].pre = faces + i;
		}
		if (i == feNum - 1)
			faces[i].next = faces;
		else {
			faces[i].next = faces + i + 1; //
		}
		
		//halfedges[i + 2].sv = verts + fileobj.ffaces[i].v[2];
	}

	//初始化新建的物体
	newSolid.fnum = fileobj.Fnum;
	newSolid.vnum = fileobj.Vnum;
	newSolid.id = 0;
	newSolid.edges_list = edges;
	newSolid.faces_list = faces;

	return 1;
}
*/