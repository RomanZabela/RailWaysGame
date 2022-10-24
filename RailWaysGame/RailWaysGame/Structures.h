#pragma once
#include <Windows.h>

struct Train {
	POINT head;
	POINT tail;
	POINT block;
	POINT preBlock;
	int Destination;
	COLORREF Color;
	byte Stop;
	byte MouseStop;
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

struct NewRoad {
	POINT block;
	struct Road road;
};

struct City {
	POINT block;
	COLORREF Color; //BGR
};