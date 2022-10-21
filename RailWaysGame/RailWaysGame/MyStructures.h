#pragma once
#include <Windows.h>

struct Train {
	POINT head;
	POINT tail;
	POINT block;
	POINT preBlock;
	int Destination;
	COLORREF Color;
};

struct Road {
	int leftBottom;
	int bottomRight;
	int topRight;
	int leftTop;
	int horizontal;
	int vertical;
	byte isRoad;
};

struct NewRoad
{
	/*int xBlock;
	int yBlock;*/
	POINT block;
	struct Road road;
};

struct City {
	/*int xBlock;
	int yBlock;*/
	POINT block;
	COLORREF Color; //BGR
};