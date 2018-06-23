#include "ReadFile.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

readFile::readFile()
{
	ifstream infile;
	infile.open("teapot2.obj");
	if (!infile.is_open())
	{
		cerr << "文件读取失败" << endl;
		return;
	}
	string first;
	char temp[20];
	while (infile >> first)
	{
		if (first == "nv")
		{
			infile >> temp;
			Vnum = atoi(temp);
			cout << Vnum << endl;
		}
		infile >> first;
		if (first == "nf")
		{
			infile >> temp;
			Fnum = atoi(temp);
			cout << Fnum;
			break;
		}
	}
		
	fverts = new vert[Vnum];
	ffaces = new face[Fnum];
	int i = 0;
	while (i<Vnum)
	{//一直读取到文件结尾
		infile >> first;
		if (first == "v")
		{
			infile >> temp;
			//cout << temp << "temp\n";
			fverts[i].x = atof(temp);
			//cout << "verts[i].x:" << verts[i].x << " ";
			infile >> temp;
			fverts[i].y = atof(temp);
			//cout << "verts[i].y:" << verts[i].y << " ";
			infile >> temp;
			fverts[i].z = atof(temp);
			//cout << "verts[i].z:" << verts[i].z << endl;
		}

		i++;
	}
	i = 0;
	while (i<Fnum)
	{
		infile >> first;
		if (first == "f")
		{
			infile >> temp;
			ffaces[i].v[0] = atoi(temp);
			infile >> temp;
			ffaces[i].v[1] = atoi(temp);
			infile >> temp;
			ffaces[i].v[2] = atoi(temp);
		}
		i++;
	}

	/*for (int j = 0; j < Vnum; ++j)
	{
	cout << "v " << verts[j].x << " " << verts[j].y << " " << verts[j].z << " " << endl;
	}
	for (int j = 0; j < Fnum; ++j)
	{
	cout << "f " << faces[j].v[0] << " " << faces[j].v[1] << " " << faces[j].v[2] << " " << endl;
	}*/
}

readFile::~readFile()
{

}