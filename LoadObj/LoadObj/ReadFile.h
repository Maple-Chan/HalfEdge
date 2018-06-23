#pragma once
#ifndef READFILE_H
#define READFILE_H

struct vert
{
	double x;
	double y;
	double z;
};
struct face
{
	int v[3];
};

class readFile
{
public:
	int Vnum;
	int Fnum;
	vert* fverts;
	face* ffaces;
	readFile();
	~readFile();
};

#endif // !READFILE_H