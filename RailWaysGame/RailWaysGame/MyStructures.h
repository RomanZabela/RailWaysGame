#pragma once
#include <Windows.h>

struct Train {
	int headX;
	int headY;
	int tailX;
	int tailY;
	int blockX;
	int blockY;
	int preBlockX;
	int preBlockY;
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
	int xBlock;
	int yBlock;
	struct Road road;
};

struct City {
	int xBlock;
	int yBlock;
	COLORREF Color; //BGR
};